
#include "DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT11
#define moist1_sen1 A5
#define moist1_sen2 A4

DHT dht(DHTPIN, DHTTYPE);

const uint8_t PUMP_OFF = HIGH;
const uint8_t PUMP_ON = LOW;

float humidity = 0;
float airTemp = 0;

float moistVal1 = 0;
int moist2_sen1 = A1;
int moistVal2 = 0;
int pump1 = 9;
int pump2 = 6;
char incoming_value = 0;
bool Watering1 = false;
bool Watering2 = false;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  digitalWrite(pump2, PUMP_OFF);
  digitalWrite(pump1, PUMP_OFF);
}

// Getting the moisture value for seconde station
float getMoisture1() {
  float val = 0;
  for (int i = 0; i <= 100; i++) {
    val = val + analogRead(moist1_sen1) + analogRead(moist1_sen2);
    delay(1);
  }
  val = val / 200;
  return val;
}

// Controlling pumps from serial input
void controlPumps(char input) {
  if (input == '0') {
    Watering1 = false;
    digitalWrite(pump1, PUMP_OFF);
  } else if (input == '1') {
    Watering1 = true;
  } else if (input == '2') {
    digitalWrite(pump1, PUMP_ON);
    delay(2000);
    if (!Watering1) {
      digitalWrite(pump1, PUMP_OFF);
    }
  } else if (input == '3') {
    Watering2 = false;
    digitalWrite(pump2, PUMP_OFF);
  } else if (input == '4') {
    Watering2 = true;
  } else if (input == '5') {
    digitalWrite(pump2, PUMP_ON);
    delay(2000);
    if (!Watering2) {
      digitalWrite(pump2, PUMP_OFF);
    }
  }
}

// Auto watering based on system status and monitored conditions
void autoWater() {
  if (Watering1) {
    if (moistVal2 > 200) {
      digitalWrite(pump1, PUMP_OFF);
    } else {
      digitalWrite(pump1, PUMP_ON);
    }
  }

  if (Watering2) {
    if (moistVal1 < 700) {
      digitalWrite(pump2, PUMP_OFF);
    } else {
      digitalWrite(pump2, PUMP_ON);
    }
  }
}

// Printing to client
void printString(){
  Serial.println(String(moistVal2) + "," + humidity + "%," + airTemp + "°C," + moistVal1 + "|");
}

void loop() {
  moistVal1 = getMoisture1();
  delay(100);
  moistVal2 = analogRead(moist2_sen1);
  humidity = dht.readHumidity();
  airTemp = dht.readTemperature();

  if (Serial.available() > 0) {
    incoming_value = Serial.read();
    controlPumps(incoming_value);
  }
  if (Watering1 || Watering2) {
    autoWater();
  }

  printString();
}
