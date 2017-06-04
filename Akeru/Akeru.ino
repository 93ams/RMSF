/**
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 *
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 *
 * Released into the public domain.
 * ----------------------------------------------------------------------------
 * This sample shows how to read and write data blocks on a MIFARE Classic PICC
 * (= card/tag).
 *
 * BEWARE: Data will be written to the PICC, in sector #1 (blocks #4 to #7).
 *
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

bool keyA_OK , keyB_OK;
bool isSecurityBreached;
int magneticSensor = 6;
const int buzzer = 2;
int masterAlarmON;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

#include <Akeru.h>

// TD1208 Sigfox module IO definition
/*   Snootlab device | TX | RX
               Akeru | D4 | D5
               Akene | D5 | D4
            Breakout | your pick */
#define TX 4
#define RX 5

// Sigfox instance management
Akeru akeru(RX, TX);

int i = 69;
bool first;

/**
 * Initialize.
 */
void setup() {
    Serial.begin(9600); // Initialize serial communications with the PC
    while(!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card
    first = true;       //I use this to WakeUpCards after the first round

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    pinMode(magneticSensor, INPUT);

    digitalWrite(magneticSensor, HIGH);
    pinMode(buzzer, OUTPUT);

    Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
    Serial.print(F("Using key (for A and B):"));
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();

    masterAlarmON = 0;
    isSecurityBreached = false;

    // Check TD1208 communication
    if (!akeru.begin())
    {
      Serial.println("TD1208 KO");
      while(1);
    }

    String data = "";
    if (akeru.receive(&data))
    {
      Serial.print("Data received : 0x");
      Serial.println(data);
    } else {
      Serial.print("No data received");
    }
}

/**
 * Main loop.
 */
void loop() {
    byte sector         = 0;
    byte blockAddr      = 0;
    byte trailerBlock   = 4;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);

  switch(masterAlarmON){

    case 0://masterAlarmON = false -> People are home
      // Look for new cards
      if(isSecurityBreached){
        Serial.println("Security has been breached while you were away!");//log file? Can Arduino have this?
        isSecurityBreached = false;
      }
      Serial.println("Start 0 (people are home)");
      while (!initcard(buffer, size)); // Detecting new card
      Serial.println("CardFound");

      keyA_OK = false;
      keyB_OK = false;

      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F("Card UID:"));
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
      Serial.println();

      // In this sample we use the second sector,
      // that is: sector #1, covering block #4 up to and including block #7

      // Authenticate using key A
      Serial.println(F("Authenticating using key A..."));
      status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
      }else{
        Serial.println("keyA OK");
        keyA_OK = true;
      }

      // Show the whole sector as it currently is
      Serial.println(F("Current data in sector:"));
      mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, 0);
      Serial.println();

      // Read data from the block
      Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
      Serial.println(F(" ..."));
      status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Read() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
      }
      Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
      dump_byte_array(buffer, 16); Serial.println();
      Serial.println();
     /* // Halt PICC
      mfrc522.PICC_HaltA();
      // Stop encryption on PCD
      mfrc522.PCD_StopCrypto1();
      Serial.println("Halted");*/
      // Authenticate using key B
      Serial.println(F("Authenticating again using key B..."));
      status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
      }else{
        Serial.println("keyB OK");
        keyB_OK = true;
      }
      // Halt PICC
      mfrc522.PICC_HaltA();
      // Stop encryption on PCD
      mfrc522.PCD_StopCrypto1();
      Serial.println("Halted twice");

      if(keyA_OK && keyB_OK){
        Serial.println("O Zé saiu e activou o alarme!!!!");
        delay(5000);
        masterAlarmON = 1;
        return;
      }

      mfrc522.PICC_WakeupA(buffer,	size);


////////////////////////////////////////////////////////////


    case 1://masterAlarmON = true! -> People left, looking out!
      Serial.println("Start 1 (people left. We on patrol boys!)");
      //Check Sensors - LOW means door is closed / HIGH means door is open
      if(digitalRead(magneticSensor) == LOW){

        Serial.println("magneticSensor = LOW");

        Serial.println("If my TRUE OWNER comes, I should stfu");

        tone(buzzer, 100); // Send 100Hz sound signal...
        delay(1000);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        delay(1000);       
        //this means the door is stil closed so we're fine!

      }else{
        Serial.println("magneticSensor = HIGH");
        tone(buzzer, 10000); // Send 10KHz sound signal...
        delay(1000);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        delay(1000);        
        masterAlarmON = 3;//INTRUDER ALERT

        int sensorValue = i;

        // Trace on serial console
        Serial.println(sensorValue);

        // convert to hexadecimal before sending
        String data = akeru.toHex(sensorValue);

        // Send in the mighty internet!
        // akeru.sendPayload() returns 0 if message failed.
        if (akeru.sendPayload(data))
        {
          Serial.println("Message sent to SigFox!");
        }
        i++;
        return;
      }

/////////
      mfrc522.PICC_WakeupA(buffer, size);

      Serial.print("we checking ?    ");
      // Look for cards
      if ( mfrc522.PICC_IsNewCardPresent()){
        Serial.println(F("Found a card"));
      }else{
          return;
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial())
          return;

      keyA_OK = false;
      keyB_OK = false;

      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F("Card UID:"));
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
      Serial.println();


      // Authenticate using key A
      Serial.println(F("Authenticating using key A..."));
      status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
      }else{
        Serial.println("keyA OK");
        keyA_OK = true;
      }

      // Show the whole sector as it currently is
      Serial.println(F("Current data in sector:"));
      mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, 0);
      Serial.println();

      // Read data from the block
      Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
      Serial.println(F(" ..."));
      status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Read() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
      }
      Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
      dump_byte_array(buffer, 16); Serial.println();
      Serial.println();
      // Halt PICC
      mfrc522.PICC_HaltA();
      // Stop encryption on PCD
      mfrc522.PCD_StopCrypto1();
      Serial.println("Halted again (1)");

      // Authenticate using key B
      Serial.println(F("Authenticating again using key B..."));
      status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {

          Serial.print(F("PCD_Authenticate() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
      }else{
        Serial.println("keyB OK");
        keyB_OK = true;
      }
      // Halt PICC
      mfrc522.PICC_HaltA();
      // Stop encryption on PCD
      mfrc522.PCD_StopCrypto1();
      Serial.println("Halted twice v2");

      if(keyA_OK && keyB_OK){
        Serial.println("Authorized access.");
        if(isSecurityBreached = true){
          Serial.println("Security was breached while you were away!");
          isSecurityBreached = false;
        }
        delay(5000);
        masterAlarmON = 0;
        return;
      }

      case 3://rebentou
        Serial.println("this should mean we have an intruder");
        masterAlarmON = 1;
        isSecurityBreached = true;
        //mfrc522.PICC_WakeupA(  buffer, size);
        return;

      default:
        Serial.println("default. what the heck ?");
  }
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte size) {
    for (byte i = 0; i < size; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}



bool initcard(byte *buffer, byte size)
{
  if(!first){ mfrc522.PICC_WakeupA(buffer, size); }
  first = false;
  
  // Look for new cards (in case you wonder what PICC means: proximity integrated circuit card)
  if (!mfrc522.PICC_IsNewCardPresent()) {//if PICC_IsNewCardPresent returns 1, a new card has been found and we continue
    return false; //if it did not find a new card is returns a '0' and we return to the start of the loop
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {//if PICC_ReadCardSerial returns 1, the "uid" struct (see MFRC522.h lines 238-45)) contains the ID of the read card.
    return false; //if it returns a '0' something went wrong and we return to the start of the loop
  }

  return true;
}
