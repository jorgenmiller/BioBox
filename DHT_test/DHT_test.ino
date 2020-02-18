#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 8

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();

  Serial.begin(9600);
}

void loop() {
  int data = dht.readHumidity();
  Serial.print(0);
  Serial.print("\t");
  Serial.print(100);
  Serial.print("\t");
  Serial.print(data);
  Serial.println();

  delay(500);
}
