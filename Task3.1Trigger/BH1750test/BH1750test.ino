#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;
int buzzerPin = 12;
int green = 11;
int yellow = 9;
int orange = 7;

int buzzer = 12;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(orange, OUTPUT);

  pinMode(buzzer, OUTPUT);

  lightMeter.begin();
}

void loop() {
  int lux = lightMeter.readLightLevel();

  if (lux < 50) {
    digitalWrite(green, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(orange, LOW);
  }  
  else if (lux >= 50 && lux < 100) {
    digitalWrite(yellow, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(orange, LOW);
  }
  else if (lux >= 100) {
    digitalWrite(orange, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);

    tone(buzzer, 500, 200);
  } 
    
  Serial.println(lux);
  delay(3000);

}
