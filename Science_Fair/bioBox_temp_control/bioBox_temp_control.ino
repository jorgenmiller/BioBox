int led = 13;

int temp_1_pin = A0;
int temp_2_pin = A1;
int temp_3_pin = A2;

int relay_1_pin = 2;
int relay_2_pin = 3;
int relay_3_pin = 4;
int relay_4_pin = 5;

float reference_voltage = 5;

float max_temp = 50;
float min_temp = 45;

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

  Serial.begin(9600);

  String temps = "temp_1" + sep + "temp_2" + sep + "temp_3";
  Serial.println(temps);
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

  String temps = temp_1 + sep + temp_2 + sep + temp_3;
  Serial.println(temps);

  //regulate relay_2 based on temp_1
  if (temp_3 < min_temp) {
    digitalWrite(relay_1_pin, LOW);
    digitalWrite(relay_3_pin, LOW);
    digitalWrite(led, HIGH);
  } if (temp_3 > max_temp) {
    digitalWrite(relay_1_pin, LOW);
    digitalWrite(relay_3_pin, HIGH);
    digitalWrite(led, LOW);
  }
  delay(100);
}
