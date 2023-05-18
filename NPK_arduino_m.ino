// code for arduino
//AR RoboTics
//https://www.youtube.com/@ARRoboTics

#include <SoftwareSerial.h>
#include <Wire.h>

#define RE 8
#define DE 7
byte val1,val2,val3,val4;
 #define ANSWERSIZE 17
//const byte code[]= {0x01, 0x03, 0x00, 0x1e, 0x00, 0x03, 0x65, 0xCD};
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
 
byte values[11];
SoftwareSerial mod(10,11);
 String m1 = "";
String m2 = "";
String m3 = "";
String m4 = "";
String msg = "";
      
float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
      
void setup() {
  Serial.begin(9600);
  mod.begin(9600);
Wire.begin(8);

// Wire.onReceive(receiveEvent);/* join i2c bus with address 8 */
  /* register receive event */
 Wire.onRequest(requestEvent);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  

}
 
void loop() {
  
  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);
  val4 = phcal();
  delay(250);

 m1 = String(val1);
 m2 = String(val2);
 m3 = String(val3);
 m4 = String(val4);

 msg = String(m1)+String(m2)+String(m3)+String(m4)+String( m1.length())+String( m2.length())+String( m3.length())+String( m4.length());
  Serial.print("Nitrogen: ");
  Serial.print(val1);
  Serial.println(" mg/kg");
  Serial.print("Phosphorous: ");
  Serial.print(val2);
  Serial.println(" mg/kg");
  Serial.print("Potassium: ");
  Serial.print(val3);
  Serial.println(" mg/kg");
  delay(2000);
 
}
 
byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(nitro,sizeof(nitro))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}
 
byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(phos,sizeof(phos))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}
 
byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(pota,sizeof(pota))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}

float phcal(){
 for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A2);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;
 Serial.println(ph_act);
 delay(1000);
return ph_act;
}



void requestEvent() {
 byte response[ANSWERSIZE];
 
  // Format answer as array
  for (byte i=0;i<ANSWERSIZE;i++) {
   response[i] = (byte)msg.charAt(i);
   Serial.print(response[i]);
    }    
  // Send response back to Master
  Wire.write(response,sizeof(response));
 
  // Print to Serial Monitor
  Serial.println("Request event");
}
