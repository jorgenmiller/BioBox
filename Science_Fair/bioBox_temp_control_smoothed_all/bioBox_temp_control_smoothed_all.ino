int led = 13;

int temp_1_pin = A0;
int temp_2_pin = A1;
int temp_3_pin = A2;

int relay_1_pin = 2;
int relay_2_pin = 3;
int relay_3_pin = 4;
int relay_4_pin = 5;

float reference_voltage = 5;

//readings for smoothing
const int num_readings = 100;
int read_index = 0;

float temp_1_readings[num_readings];
float temp_1_total = 0;
float temp_1_average = 0;

float temp_2_readings[num_readings];
float temp_2_total = 0;
float temp_2_average = 0;

float temp_3_readings[num_readings];
float temp_3_total = 0;
float temp_3_average = 0;

float max_temp_1 = 32;
float min_temp_1 = 28;

String sep = "\t";

void setup() {
  pinMode(led, OUTPUT);
  
  pinMode(temp_1_pin, INPUT);
  pinMode(temp_2_pin, INPUT);
  pinMode(temp_3_pin, INPUT);

  pinMode(relay_1_pin, OUTPUT);
  pinMode(relay_2_pin, OUTPUT);
  pinMode(relay_3_pin, OUTPUT);
  pinMode(relay_4_pin, OUTPUT);

  digitalWrite(relay_1_pin, LOW); //fan on
  digitalWrite(relay_2_pin, HIGH); //heat lamp off
  digitalWrite(relay_3_pin, HIGH); //solenoid off
  digitalWrite(relay_4_pin, HIGH); //kill switch off

  for (int thisReading = 0; thisReading < num_readings; thisReading++) {
    temp_1_readings[thisReading] = 0;
    temp_2_readings[thisReading] = 0;
    temp_3_readings[thisReading] = 0;
  }

  Serial.begin(9600);

  String temps = "temp_1" + sep + "temp_2" + sep + "temp_3";
  String data = "temp_1_smoothed" + sep + "temp_1" + sep + "temp_2_smoothed" + sep + "temp_2" + sep + "temp_3_smoothed" + sep + "temp_3";
  Serial.println(data);
}

void loop() {
  //convert pin values to voltage readings
  float voltage_1 = reference_voltage * analogRead(temp_1_pin) / 1023;
  float voltage_2 = reference_voltage * analogRead(temp_2_pin) / 1023;
  float voltage_3 = reference_voltage * analogRead(temp_3_pin) / 1023;

  //convert voltage readings to degrees Celsius
  float temp_1 = 100 * voltage_1 - 50;
  float temp_2 = 100 * voltage_2 - 50;
  float temp_3 = 100 * voltage_3 - 50;  

  //Smoothing
  temp_1_total -= temp_1_readings[read_index];
  temp_1_readings[read_index] = temp_1;
  temp_1_total += temp_1_readings[read_index];
  temp_1_average = temp_1_total / num_readings;
  
  temp_2_total -= temp_2_readings[read_index];
  temp_2_readings[read_index] = temp_2;
  temp_2_total += temp_2_readings[read_index];
  temp_2_average = temp_2_total / num_readings;
  
  temp_3_total -= temp_3_readings[read_index];
  temp_3_readings[read_index] = temp_3;
  temp_3_total += temp_3_readings[read_index];
  temp_3_average = temp_3_total / num_readings;

  //index for smoothing array
  read_index ++;
  if (read_index >= num_readings) {read_index = 0;}

  String data = temp_1_average + sep + temp_1 + sep + temp_2_average + sep + temp_2 + sep + temp_3_average + sep + temp_3;
  String temps = temp_1_average + sep + temp_2_average + sep + temp_3_average;
  Serial.println(data);

  //regulate relay_2 based on temp_1
  if (temp_2_average < min_temp_1) {
    digitalWrite(relay_1_pin, LOW);
    digitalWrite(relay_3_pin, LOW);
    digitalWrite(led, HIGH);
  } if (temp_2_average > max_temp_1) {
    digitalWrite(relay_1_pin, LOW);
    digitalWrite(relay_3_pin, HIGH);
    digitalWrite(led, LOW);
  }
  delay(100);
}
