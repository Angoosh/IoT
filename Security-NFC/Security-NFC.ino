#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);


void setup() {
  nfc.begin();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void loop() {
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
          digitalWrite(4, LOW);
          if(payloadAsString=="en512"){
            digitalWrite(3, HIGH);
            delay(1000);
            digitalWrite(3, LOW);
          }
          if(payloadAsString=="en666"){
            digitalWrite(2, HIGH);
            delay(1000);
            digitalWrite(2, LOW);
          }
        }
        else;
        }
      }
      else;
    }
   digitalWrite(4, HIGH);
   delay(10); 
   
  }
  

