
#include "DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT11
#define testPin A5
#define testPin2 A4

float testValue = 0;

DHT dht(DHTPIN, DHTTYPE);

int sensorPin = A1;
int sensorValue = 0;

int motorPin1 = 9;
int motorPin2 = 6;

char Incoming_value = 0;

bool Watering1 = false;
bool Watering2 = false;

void setup() {
  
  Serial.begin(9600);
  dht.begin();
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin1, HIGH);
}

void loop() {
 
  for(int i = 0; i <= 100; i++){
    testValue = testValue + analogRead(testPin) + analogRead(testPin2);
    delay(1);
    
  }
  testValue = testValue/200;

  sensorValue = analogRead(sensorPin);
  if (Serial.available() > 0) {
    Incoming_value = Serial.read();
     // Serial.print(Incoming_value); ---Fjernet denne!
     // Serial.print("\n"); ---Fjernet denne!
    if (Incoming_value == '0') {
      Watering1 = false;
      digitalWrite(motorPin1, HIGH);
    } else if (Incoming_value == '1') {
      //digitalWrite(motorPin1, LOW);
      Watering1 = true;
    } else if (Incoming_value == '2') {
      digitalWrite(motorPin1, LOW);
      delay(2000);
      if(!Watering1){
        digitalWrite(motorPin1, HIGH);
      }

      
    } else if (Incoming_value == '3') {
      Watering2 = false;
      digitalWrite(motorPin2, HIGH);
    } else if (Incoming_value == '4') {
      Watering2 = true;
    } else if (Incoming_value == '5') {
      digitalWrite(motorPin2, LOW);
      delay(2000);
      if(!Watering2){
        digitalWrite(motorPin2, HIGH);
        }
      }
    }

   if(Watering1){
    if (sensorValue > 200) {
      digitalWrite(motorPin1, HIGH);
    } else {
      digitalWrite(motorPin1, LOW);
    }
   }

   if(Watering2){
    if (testValue < 700){
      digitalWrite(motorPin2, HIGH);
    } else {
      digitalWrite(motorPin2, LOW);
    }
   }
  
   float humidity = dht.readHumidity();
   float airTemp = dht.readTemperature();
   
   Serial.println(String(sensorValue) + "," + humidity + "%," + airTemp + "°C" + "|");

}
