// code for EEP8266/Nodemcu
//AR RoboTics
//https://www.youtube.com/@ARRoboTics

#include <ESP8266WiFi.h>
#include <Wire.h>
#include "ThingSpeak.h"
#include <DHT.h>

const char* ssid = "AR RoboTics 4g";   // your network SSID (name) 
const char* pass = "#25Grana";

WiFiClient  client;
DHT dht(D5, DHT11);

int Raw       = A0;      //Analog channel A0 as used to measure temperature
int threshold = 13; 

unsigned long myChannelNumber = 1794812;
const char * myWriteAPIKey = "30WGXPGJKSU9X20N";

char buf[16];
char n[3]  ;
char p[3]  ;
char kk[3]  ;
char ph[3]  ;
void setup() {
  Wire.begin(D1, D2);      // join i2c bus (address optional for master)
  Serial.begin(9600); 
  dht.begin();
   pinMode(threshold,INPUT_PULLUP);
  // start serial for output
Serial.println("Connecting to ");
       Serial.println(ssid); 
 
       WiFi.begin(ssid, pass); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected"); 

ThingSpeak.begin(client); 

}

  String response = "";
void loop() {
  Wire.requestFrom(8, 16);    // request 8 bytes from slave device #8

  
  while (Wire.available()) {
      char b = Wire.read();
      response += b;

  }
  // Print to Serial Monitor
  //Serial.println(response);
  cal();
 
  
}


void cal(){
  float h = dht.readHumidity();
  Serial.print("humidity = ");
  Serial.println(h);
  float t = dht.readTemperature();
  Serial.print("temperature = ");
  Serial.println(t);
  float percentage = 0.0;
  float percentage2 = 0.0;
  float reading = analogRead(Raw);     
       percentage = (reading/1024) * 100;
       percentage2 = 100-percentage;
       Serial.print("moisture = "); 
        Serial.println(percentage2);
response.toCharArray(buf,17);
//Serial.println(buf);
int l = strlen(buf);
//Serial.println(l);

 char l1 = buf[l-4];
 int f1 = l1 - '0';
 //Serial.println(f1);
 char l2 = buf[l-3];
 int f2 = l2 - '0';
 //Serial.println(f2);
 char l3 = buf[l-2];
 int f3 = l3 - '0';
 //Serial.println(f3);
  char l4 = buf[l-1];
 int f4 = l4 - '0';

 
 int i =0;
 int j = 0;
 
for(i =0;i<f1;i++){
  n[i] = buf[i];
}n[f1] = '\0';
Serial.print("nitrogen = ");
Serial.println(n);
 int t1 = atoi(n);
for(j =0;j<f2;j++){
  p[j] = buf[i]; i++;
  }p[f2] = '\0';
  Serial.print("phosphorus = ");
  Serial.println(p);
  int t2 = atoi(p);
  
 // 
for(int k = 0;k<f3;k++){
  kk[k] = buf[i];
  i++;
 }kk[f3] = '\0';
 Serial.print("potassium = ");
Serial.println(kk);
int t3 = atoi(kk);

 // 
for(int r = 0;r<f4;r++){
  ph[r] = buf[i];
  i++;
 }ph[f4] = '\0';
 Serial.print("ph = ");
Serial.println(ph);
int t4 = atof(ph);

 ThingSpeak.setField(7,ph);
 ThingSpeak.setField(4,t1);
 ThingSpeak.setField(5,t2);
 ThingSpeak.setField(6,t3);
 ThingSpeak.setField(2,h);
 ThingSpeak.setField(1,t);
 ThingSpeak.setField(3,percentage2);
  int http_code = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
  if (http_code == 200){
  Serial.println(" data chla gya :) ");
}
else {
  Serial.println("ye "+ String(http_code)+"wali gadbad he :( ");
}
delay(22000);
}
