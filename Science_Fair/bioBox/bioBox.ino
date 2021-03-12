int led = 13;

int temp_1_pin = A0;
int temp_2_pin = A1;
int temp_3_pin = A2;
int temp_4_pin = A3;
int temp_5_pin = A4;

#include "DHT.h"
#define dht_type DHT11
#define dht_1_pin 8
#define dht_2_pin 9
#define dht_3_pin 10
DHT dht_1(dht_1_pin, dht_type);
DHT dht_2(dht_2_pin, dht_type);
DHT dht_3(dht_3_pin, dht_type);

int relay_1_pin = 2;
int relay_2_pin = 3;
int relay_3_pin = 4;
int relay_4_pin = 5;

float reference_voltage = 5;

//readings for smoothing
const int num_readings = 10;
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

float temp_4_readings[num_readings];
float temp_4_total = 0;
float temp_4_average = 0;

float temp_5_readings[num_readings];
float temp_5_total = 0;
float temp_5_average = 0;

int max_temp_1 = 32;
int min_temp_1 = 28;

String sep = "\t";

void setup() {
  pinMode(led, OUTPUT);
  
  pinMode(temp_1_pin, INPUT);
  pinMode(temp_2_pin, INPUT);
  pinMode(temp_3_pin, INPUT);
  pinMode(temp_4_pin, INPUT);
  pinMode(temp_5_pin, INPUT);

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
    temp_4_readings[thisReading] = 0;
    temp_5_readings[thisReading] = 0;
  }

  dht_1.begin();
  dht_2.begin();
  dht_3.begin();

  Serial.begin(9600);

  Serial.print("temp_1_smoothed");
  Serial.print(sep);
  Serial.print("temp_1");
  Serial.print(sep);
  Serial.print("temp_2_smoothed");
  Serial.print(sep);
  Serial.print("temp_2");
  Serial.print(sep);
  Serial.print("temp_3_smoothed");
  Serial.print(sep);
  Serial.print("temp_3");
  Serial.print(sep);
  Serial.print("temp_4_smoothed");
  Serial.print(sep);
  Serial.print("temp_4");
  Serial.print(sep);
  Serial.print("temp_5_smoothed");
  Serial.print(sep);
  Serial.print("temp_5");
  Serial.print(sep);
  Serial.print("dht_1_h");
  Serial.print(sep);
  Serial.print("dht_2_h");
  Serial.print(sep);
  Serial.print("dht_3_h");
  Serial.print(sep);
  Serial.print("dht_1_t");
  Serial.print(sep);
  Serial.print("dht_2_t");
  Serial.print(sep);
  Serial.print("dht_3_t");
  Serial.println();
}

void loop() {
  //convert pin values to voltage readings
  float voltage_1 = reference_voltage * analogRead(temp_1_pin) / 1023;
  float voltage_2 = reference_voltage * analogRead(temp_2_pin) / 1023;
  float voltage_3 = reference_voltage * analogRead(temp_3_pin) / 1023;
  float voltage_4 = reference_voltage * analogRead(temp_4_pin) / 1023;
  float voltage_5 = reference_voltage * analogRead(temp_5_pin) / 1023;

  //convert voltage readings to degrees Celsius
  float temp_1 = 100 * voltage_1 - 50;
  float temp_2 = 100 * voltage_2 - 50;
  float temp_3 = 100 * voltage_3 - 50;  
  float temp_4 = 100 * voltage_4 - 50;
  float temp_5 = 100 * voltage_5 - 50;  

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

  temp_4_total -= temp_4_readings[read_index];
  temp_4_readings[read_index] = temp_4;
  temp_4_total += temp_4_readings[read_index];
  temp_4_average = temp_4_total / num_readings;  

  temp_5_total -= temp_5_readings[read_index];
  temp_5_readings[read_index] = temp_5;
  temp_5_total += temp_5_readings[read_index];
  temp_5_average = temp_5_total / num_readings;

  //index for smoothing array
  read_index ++;
  if (read_index >= num_readings) {read_index = 0;}

  int dht_1_t = dht_1.readTemperature();
  int dht_2_t = dht_2.readTemperature();
  int dht_3_t = dht_3.readTemperature();
  int dht_1_h = dht_1.readHumidity();
  int dht_2_h = dht_2.readHumidity();
  int dht_3_h = dht_3.readHumidity();

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

  Serial.print(temp_1_average);
  Serial.print(sep);
  Serial.print(temp_1);
  Serial.print(sep);
  Serial.print(temp_2_average);
  Serial.print(sep);
  Serial.print(temp_2);
  Serial.print(sep);
  Serial.print(temp_3_average);
  Serial.print(sep);
  Serial.print(temp_3);
  Serial.print(sep);
  Serial.print(temp_4_average);
  Serial.print(sep);
  Serial.print(temp_4);
  Serial.print(sep);
  Serial.print(temp_5_average);
  Serial.print(sep);
  Serial.print(temp_5);
  Serial.print(sep);
  Serial.print(dht_1_h);
  Serial.print(sep);
  Serial.print(dht_2_h);
  Serial.print(sep);
  Serial.print(dht_3_h);
  Serial.print(sep);
  Serial.print(dht_1_t);
  Serial.print(sep);
  Serial.print(dht_2_t);
  Serial.print(sep);
  Serial.print(dht_3_t);
  Serial.println();

  delay(100);
}
