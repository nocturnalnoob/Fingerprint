#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#define RELAY 8
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
const int rs=2  , en=3  , d4=4  , d5=5  , d6=6  ,d7=7  ;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
lcd.begin(16,2);
lcd.setCursor(0,0);
pinMode (RELAY,OUTPUT);
Serial.begin(9600);   // Initiate a serial communication
SPI.begin();      // Initiate  SPI bus
mfrc522.PCD_Init();   // Initiate MFRC522
Serial.println("Approximate your card to the reader...");
Serial.println();

}
void loop()
{
// Look for new cards
if ( ! mfrc522.PICC_IsNewCardPresent())
{
return;
}
// Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial())
{
return;
}
//Show UID on serial monitor
Serial.print("UID tag :");
String content= "";
byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++)
{
Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
Serial.print(mfrc522.uid.uidByte[i], HEX);
content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
content.concat(String(mfrc522.uid.uidByte[i], HEX));
}
Serial.println();
Serial.print("Message : ");
content.toUpperCase();

if (content.substring(1) == "C9 8A 2D E5" || content.substring(1) == "C9 3D D6 A2") //change here the UID of the card/cards that you want to give access
{
Serial.println("Welcome");
Serial.println("Project IPC-149");
Serial.println();
Serial.println("Access Granted");
Serial.print("\n");
delay(1000);

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Welcome");
lcd.setCursor(0,1);
lcd.print("Project IPC-149");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Access Granted");
}

else
{
  Serial.print("\n");
  Serial.println("Access Denied");
  Serial.print("\n");
  delay(1000);
}



}
