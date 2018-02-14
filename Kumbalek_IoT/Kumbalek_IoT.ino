#include <Servo.h>
#include <Process.h>
#include "U8glib.h"

#define SERVO 5
#define LED 13
#define AIR A1
#define PT100 A0

U8GLIB_SSD1306_128X64 OLED(U8G_I2C_OPT_NONE);

Process date;
int hours, minutes, seconds;
int lastSecond = -1;
Servo lights;

const int numReadings = 100;

long int refresh = 0;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;
int temp;
int air;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  lights.attach(SERVO);
  digitalWrite(LED, HIGH);
  Bridge.begin();
  digitalWrite(LED, LOW);

  if(!date.running()){
    date.begin("date");
    date.addParameter("+%T");
    date.run();
  }
  
  for(int thisReading = 0; thisReading<numReadings; thisReading++){
    readings[thisReading] = 0;
  }
}

void loop() {
  Lights();
  Temperature();
}

void Time(){
  //OLED.setFont(u8g_font_unifont);
  OLED.setPrintPos(0, 64);
  if (lastSecond != seconds) { // if a second has passed
    // print the time:
    if (hours <= 9) {
      OLED.print("0");  // adjust for 0-9
    }
    OLED.print(hours);
    OLED.print(":");
    if (minutes <= 9) {
      OLED.print("0");  // adjust for 0-9
    }
    OLED.print(minutes);
    OLED.print(":");
    if (seconds <= 9) {
      OLED.print("0");  // adjust for 0-9
    }
    OLED.println(seconds);
    
  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%T");
    date.run();
  }  
  
  while(date.available() > 0){
    String timeString = date.readString();
    int firstColon = timeString.indexOf(":");
    int secondColon = timeString.lastIndexOf(":");
    String hourString = timeString.substring(0, firstColon);
    String minString = timeString.substring(firstColon + 1, secondColon);
    String secString = timeString.substring(secondColon + 1);

    hours = hourString.toInt();
    minutes = minString.toInt();
    lastSecond = seconds;          // save to do a time comparison
    seconds = secString.toInt();    
  }
  
}
}

void Lights(){
  if(hours == 18&&minutes == 45){
    lights.write(60);
  }
  if(hours == 6&&minutes == 30){
    lights.write(0);
  }
}

void Aires(){
  air = analogRead(AIR);
  OLED.setFont(u8g_font_unifont);
  OLED.setPrintPos(0, 25);
  OLED.print(air);
  if(air<100){
    OLED.print(" OK");
    digitalWrite(LED,LOW);
  }
  else{
    OLED.print(" VENT!");
    digitalWrite(LED,HIGH);
  }
}

void Temperature(){
  if (millis()-refresh > 100) {
    OLED.firstPage();
    do {
      Temps();
      Aires();
      //Time();
    } while( OLED.nextPage() );
    refresh = millis();
  }
  temp = analogRead(PT100);
  temp = map(temp, 923, 900, 25 ,47);
  total = total - readings[readIndex];
  readings[readIndex] = temp;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;
  Serial.print(average);
  Serial.print("------");
  Serial.print(analogRead(PT100));
  Serial.println("");
  delay(1);
}

void Temps() {
  //OLED.setFont(u8g_font_unifont);
  OLED.setPrintPos(0, 10);
  OLED.print(average);
  OLED.print(" C");
}



