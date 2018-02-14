#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <LiquidCrystal_I2C.h>

#define LASER_RECIEVER 2
#define Pi 7
#define Pi_Input 8

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
LiquidCrystal_I2C lcd(0x3F, 16, 2);


void setup() {
  nfc.begin();
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(Pi_Input, INPUT);
  pinMode(LASER_RECIEVER, INPUT);
  pinMode(Pi, OUTPUT);
  digitalWrite(Pi, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(12, HIGH);
  attachInterrupt(digitalPinToInterrupt(LASER_RECIEVER), blik, HIGH);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor ( 0, 1 );
  lcd.print("Booting...");
  while(digitalRead(Pi_Input)==LOW);
  lcd.clear();
}

void loop() {
  nearfield();
  digitalWrite(5, HIGH);
  delay(10);  
  }

void nearfield(){
  lcd.print("Kartu prosim");
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    if (tag.hasNdefMessage())
    {

      NdefMessage message = tag.getNdefMessage();
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        NdefRecord record = message.getRecord(i);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);


        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        if(payloadAsString=="en512"||payloadAsString=="en666"){
          digitalWrite(5, LOW);
          lcd.clear();
          if(payloadAsString=="en512"){
            lcd.print("Ufoga");
            digitalWrite(4, HIGH);
            delay(1000);
            digitalWrite(4, LOW);
          }
          if(payloadAsString=="en666"){
            lcd.print("Angoosh");
            digitalWrite(3, HIGH);
            delay(1000);
            digitalWrite(3, LOW);
          }
        }
        else;
        }
      }
      else; 
    }
    lcd.clear();
}
  
void blik(){
  digitalWrite(Pi, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(5000);
  digitalWrite(Pi, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  delay(2000);
}

