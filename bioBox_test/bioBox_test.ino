int led = 13;

int temp_1_pin = A0;
int temp_2_pin = A1;
int temp_3_pin = A2;

int relay_1_pin = 2;
int relay_2_pin = 3;
int relay_3_pin = 4;
int relay_4_pin = 5;

float reference_voltage = 5;

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
}

void loop() {
  digitalWrite(relay_4_pin, LOW);
  Serial.println("HIGH");
  digitalWrite(led, HIGH);
  delay(100); 
  
  digitalWrite(relay_4_pin, HIGH);
  Serial.println("LOW");
  digitalWrite(led, LOW);
  delay(500);
}
