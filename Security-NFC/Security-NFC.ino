//knihovny

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <LiquidCrystal_I2C.h>

//definovani nejdulezitejsich funkci
#define LASER_RECIEVER 2
#define Pi 7
#define Pi_Input 8

//definovani a vytvareni objektu
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//globalni promenne
int ta = 10;  //delka aktivniho pulzu pro RPi
int propusteni = 5; //jak dlouho muze clovek prochazet (s)

//setup
void setup() {
  Serial.begin(9600);   //debug linka, pro konecny projekt se nepouziva
  nfc.begin();          //zapnuti NFC
  //definice pinu pro RGB led
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);   //GND
  //definice pinu pro komunikaci s raspberry a laserem
  pinMode(Pi_Input, INPUT);
  pinMode(LASER_RECIEVER, INPUT_PULLUP);
  pinMode(Pi, OUTPUT);
  //prvotni nastaveni statusu pinu pro led a rpi
  digitalWrite(Pi, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(12, HIGH); //uz nevim proc, se uvidi
  //nastaveni interruptu na punu pro laserovy prijimac
  attachInterrupt(digitalPinToInterrupt(LASER_RECIEVER), blik, HIGH);
  //zapnuti LCD
  lcd.begin();
  lcd.backlight();
  lcd.setCursor ( 0, 1 );
  lcd.print("Booting...");
  Serial.println("Booting...");   //debug
  while(digitalRead(Pi_Input)==LOW);  //signal od raspberry ze je zapnute a ze program bezi
  lcd.clear();  //vycisti display
}

void loop() {
  nearfield();  //funkce obstaravajici nfc
  digitalWrite(5, HIGH);  //asi debug?
  delay(10);  
  }

void nearfield(){
  lcd.print("Kartu prosim");  //proseni o kartu :D
  if (nfc.tagPresent())   //kontrola jestli je prilozena karta
  {
    NfcTag tag = nfc.read();    //cteni karty
    if (tag.hasNdefMessage())
    {

      NdefMessage message = tag.getNdefMessage(); //kdyz obsauje nejaky obsah, tak ho precti
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        NdefRecord record = message.getRecord(i);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);


        String payloadAsString = "";    //chci obsah jako string
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        if(payloadAsString=="en512"||payloadAsString=="en666"||payloadAsString=="enAngoosh"||payloadAsString=="enUfoga"){ //kdyz se obsah rovna temto dvema hodnotam (provereni uzivatele)
          digitalWrite(5, LOW);
          lcd.clear();
          //rozpoznavani uzivatelu
          if(payloadAsString=="en512"){ 
            lcd.print("Ufoga");
            digitalWrite(4, HIGH);
            delay(propusteni*1000);
            digitalWrite(4, LOW);
          }
          if(payloadAsString=="en666"){
            lcd.print("Angoosh");
            digitalWrite(3, HIGH);
            delay(propusteni*1000);
            digitalWrite(3, LOW);
          }
        }
        //kdyz si pokusi "pipnout" nedo jiny, tak ho to sejme taky
        else{
          lcd.clear();
          digitalWrite(Pi, HIGH);
          delay(ta);
          digitalWrite(Pi, LOW);
          lcd.print("Unknown");
          digitalWrite(5, LOW);
          digitalWrite(4, LOW);
          digitalWrite(3, LOW);
          delay(1000);
        }
      }
    }
      else; 
    }
    lcd.clear();
}
  
void blik(){
  noInterrupts();
  digitalWrite(Pi, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  Serial.println("ON");   //debug
  //delay(5000);
  int tb = 0;
  while(tb<ta){
    delay(1);
    tb++;
  }
  Serial.println("OFF");  //debug
  digitalWrite(Pi, LOW);
  digitalWrite(5, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  //delay(2000);
  interrupts();
}
