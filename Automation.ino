#include <Servo.h>
#include <Process.h>
#include <U8glib.h>

#define SERVO 5
#define PLUG1 A1
#define PLUG2 A0
#define LED 13
#define GAS A3
#define THERMISTOR A2

U8GLIB_SSD1306_128X64 LCD(U8G_I2C_OPT_NONE);
Process date;
int hours, minutes, seconds;
int lastSecond = -1;
Servo lights;

void setup() {
  lights.attach(SERVO);
  digitalWrite(LED, HIGH);
  Bridge.begin();
  digitalWrite(LED, LOW);

  if(!date.running()){
    date.begin("date");
    date.addParameter("+%T");
    date.run();
  }

}

void loop() {
  Lights();
  Time();

}

void Time(){
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
  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%T");
    date.run();
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

void Plugs(){
  
}

void Gas(){
  
}

void Lcd(){
  
}

void Sd(){
  
}

