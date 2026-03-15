#include <BH1750.h>
#include <Wire.h>
#include <DHT11.h>

#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = "private";   // your network SSID (name) 
char pass[] = "private";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 3299883;
const char * myWriteAPIKey = "UZVLKIODL4T9XKW8";

String myStatus = "";

DHT11 dht11(2);
BH1750 lightMeter;  // SCL - A5
                    // SDA - A4

int time = 30; // 30sec

void setup() {
  delay(1000); // Give sensors a second to wake up
  Serial.begin(115200);  // Initialize serial
 
  Wire.begin();
  lightMeter.begin();

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
    
  ThingSpeak.begin(client);  //Initialize ThingSpeak
}

void loop() {

  int temperature = dht11.readTemperature();
  float lux = lightMeter.readLightLevel();

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      for (int i=0; i < 5;i++) {
        Serial.print(".");
        delay(1000); 
      }  
    } 
    Serial.println("\nConnected.");
    delayUpdate(30, false);
  }

  // Serial print
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT) {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" °C");
  } else {
      Serial.println(DHT11::getErrorString(temperature));
  }

  // set the fields with the values
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, lux);

  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    delayUpdate(time, false);
    Serial.println("Channel update successful.");
  }
  else{
    delayUpdate(time, false);
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delayUpdate(time, true);
  Serial.println("");
}

void delayUpdate(int time, bool delayConfirm) {
  for (int i=0; i<time;i++) {
    Serial.print("=");
    if (delayConfirm) {
      delay(1000);
    }
  }
  Serial.println("");
}
