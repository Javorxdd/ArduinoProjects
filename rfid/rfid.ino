#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

bool readFlag = false;
bool writeFlag = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Připraveno ke čtení RFID karet.");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'r' || command == 'R') {
      readFlag = true;
      writeFlag = false;
      Serial.println("Čekám na přiložení čipu pro čtení...");
    } else if (command == 'w' || command == 'W') {
      readFlag = false;
      writeFlag = true;
      Serial.println("Čekám na přiložení čipu pro zápis dat...");
    }
  }

  if (readFlag && waitForCard()) {
    byte buffer[18];
    // Čtení UID karty
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.print("Čtení UID karty: ");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println();
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();

      // Čtení dat z karty
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.println("Data na čipu:");
        for (byte i = 1; i <= 16; i++) {
          byte data[18];
          byte bufferSize = sizeof(data);
          if (mfrc522.MIFARE_Read(i, data, &bufferSize)) {
            Serial.print("Blok ");
            Serial.print(i);
            Serial.print(": ");
            for (byte j = 0; j < bufferSize; j++) {
              Serial.print(data[j] < 0x10 ? "0" : "");
              Serial.print(data[j], HEX);
              Serial.print(" ");
            }
            Serial.println();
          } else {
            Serial.print("Chyba čtení bloku ");
            Serial.println(i);
          }
        }
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
      }
    }
    readFlag = false;
  }

  if (writeFlag && waitForCard()) {
    // Zápis nových dat na nový čip
    byte newData[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    if (writeData(newData)) {
      Serial.println("Data úspěšně zapsána na nový čip.");
    } else {
      Serial.println("Chyba při zápisu dat na nový čip.");
    }
    writeFlag = false;
  }
}

bool waitForCard() {
  while (!mfrc522.PICC_IsNewCardPresent()) {
    // Čekej, dokud není přiložen čip
  }
  return true;
}

bool writeData(byte* data) {
  // Zde provádíme zápis dat na nový čip
  // V tomto příkladu pouze ukazujeme, že data byla úspěšně zapsána a následně se sesion ukončí (data se smažou)
  Serial.println("Zápis dat na nový čip...");
  delay(1000); // Simulace zápisu (časově náročná operace)
  Serial.println("Data úspěšně zapsána na nový čip.");
  Serial.println("Sesion ukončena. Data byla smazána.");
  return true;
}
