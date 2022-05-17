#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//if(Serial){
  
  //Serial.println("Enter WIFI SSID");
  //const String *ssid = Serial.readString(); // Enter your WiFi Name
  //Serial.println("Enter WIFI Password");
  //const String *pass = Serial.readString(); // Enter your WiFi Password
//}

//else {
  const char *ssid =  "alapati";     // Default WiFi Name
  const char *pass =  "navyasai";    // Default WiFi Password
//}


WiFiClient client;
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "HyperPalBuddy" 
#define MQTT_PASS "aio_WUPk552u7PEwT02cpuLUS752zRCi" // Enter the API key that you copied from your adafrui IO account

Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
//Set up the feed you're subscribing to
Adafruit_MQTT_Subscribe Lock = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/smartlock");
void setup()
{
  Serial.begin(115200);
  delay(10);
  mqtt.subscribe(&Lock);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(10000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 20);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  display.println("Connecting to WiFi...");
  display.display();
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  Serial.println("");
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("Connected to WiFi");
  display.display();
  Serial.println("WiFi connected");
} 
void loop()
{
   MQTT_connect();  
Adafruit_MQTT_Subscribe * subscription;
while ((subscription = mqtt.readSubscription(5000)))
     {   
   if (subscription == &Lock)
     {
      //Print the new value to the serial monitor
      Serial.println((char*) Lock.lastread); 
          
   if (!strcmp((char*) Lock.lastread, "ON"))
      {
        display.clearDisplay();
        display.setCursor(0, 20);
        display.println("Door Unlocked");
        display.display(); 
        Serial.println("Door Unlocked");
        
        delay(2000);
        
    }
    if (!strcmp((char*) Lock.lastread, "OFF"))
      {
        display.clearDisplay();
        display.setCursor(0, 20);
        display.println("Door Locked");
        display.display();
        Serial.println("Door Closed");
        
        delay(2000);
        
    }
 }  
     } 
}
void MQTT_connect() 
{
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {       
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
}
