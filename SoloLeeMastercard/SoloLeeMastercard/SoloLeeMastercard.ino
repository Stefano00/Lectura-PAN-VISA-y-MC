#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"
  
PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);

String ANS = "";         // String Para la respuesta del APDU

#define  uchar unsigned char
#define uint  unsigned int

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int inByte = 0;                  // incoming serial byte

boolean success;

//Maximum length of the array
#define MAX_LEN 16

void setup() {                
  Serial.begin(9600);           // RFID reader SOUT pin connected to Serial RX pin at 9600bps 

  // start the SPI library:
  SPI.begin();
  
  nfc.begin();

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
}

void loop()
{
    //uchar i, tmp, checksum1;
  //uchar status;
        //uchar str[MAX_LEN];
        //uchar RC_size;
        //uchar blockAddr;  //Selection operation block address 0 to 63
        //String mynum = "";
        int x;

        
//*******************Lectura PAN*******************
   // Wait for an ISO14443A type cards (Mifare, etc.).
x=0;
do 
{
  x++;
  success = nfc.inListPassiveTarget();       

  if (success) 
  {
    //Hay tarjeta presente
   // Serial.println("Tarjeta Contactada !!! ");

    uint8_t response[255];
    memset(response, 0, sizeof(response));
    uint8_t responseLength = sizeof(response);  

    //Seleciona el AID de Visa
    //00 a4 04 00 07 a0 00 00 00 03 10 10 00 -> Select VISA AID
    //uint8_t selectVisa[] = {0x00, 0xA4, 0x04, 0x00, 0x07, 0xA0, 0x00, 0x00,0x00, 0x03, 0x10, 0x10, 0x00};

    //Seleciona el AID de MC
    //00 a4 04 00 07 a0 00 00 00 04 10 10 00 -> Select MC AID
    uint8_t selectVisa[] = {0x00, 0xA4, 0x04, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x04, 0x10, 0x10, 0x00};

    success = nfc.inDataExchange(selectVisa, sizeof(selectVisa), response, &responseLength);
    if(success)
    {
      ANS="";
      for (int i= responseLength - 2 ; i <= responseLength - 1; i++)
      {
        if (response[i]<0x10)
        {
          ANS += "0";
        }
        ANS += String(response[i],HEX);
      }
      if (ANS=="9000")
      {
      //Serial.print("AID OK ");
      //Serial.println(ANS);
      }
      else
      {
      //Serial.print("AID NO OK ");
      //Serial.println(ANS);
      }
    }

        //El comando se ejecuta correctamente y el AID de Visa se encuentra
        //Se busca ahora el SFI 1 recid 3
        //00 b2 01 1c 00 -> Read Record 3 SFID 1
        //uint8_t ReadSFI[] = {0x00, 0xB2, 0x01, 0x1C, 0x00};

        //El comando se ejecuta correctamente y el AID de MC se encuentra
        //lee GPO
        //uint8_t GPO[] = {0x80, 0xA8, 0x00, 0x00, 0x02, 0x83, 0x00, 0x00};
        //success = nfc.inDataExchange(GPO, sizeof(GPO), response, &responseLength);
        //if(success) 
        //{
        //  Serial.print("READ GPO OK ");
        //  Serial.println(ANS);
        //}

        //Se busca ahora el SFI 2 recid 1
        //00 b2 01 14 00 -> Read Record 2 SFID 2 

        //Pruebas 11-01-2021
        //00 B2 01 1C 00     VISA
        uint8_t ReadSFI[] = {0x00, 0xB2, 0x02, 0x14, 0x00}; //MASTERCARD
        //uint8_t ReadSFI[] = {0x00, 0xB2, 0x01, 0x1C, 0x00}; // VISA
        
        success = nfc.inDataExchange(ReadSFI, sizeof(ReadSFI), response, &responseLength);
        if(success) 
        {
          ANS="";
          for (int i= responseLength - 2 ; i <= responseLength - 1; i++)
          {
            if (response[i]<0x10)
            {
              ANS += "0";
            }
            ANS += String(response[i],HEX);
          }
          //Serial.print("READ SFI OK ");
          //Serial.println(ANS);

            //Encuentra SFI 1 record 3 (Visa)
            // 0  1  2  3  4  5  6  7  8  9 10 11
            //70 3C 5A 08 48 50 29 00 00 00 00 26 5F 34 01 00 5F 24 03 .....
            //Serial.print("PAN : ");
            
            /*for (int i=4; i <= 11; i++)
            {
              if (response[i]<0x10)
              {
                Serial.print("0");
              }
              Serial.print (response[i],HEX);
            }*/
            
            //Encuentra SFI 2 record 1 (MC)
            // 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
            //70 81 91 9f 42 02 01 52 5f 25 03 19 04 16 5f 24 03 24 04 30 5a 08 55 59 12 00 04 93 11 27 5f 34 01 00 .....

            for (int i=4; i <= 11; i++)
            {
              if (response[i]<0x10)
              {
                Serial.print("0");
              }
              Serial.print (response[i],HEX);
            }
            
            Serial.print(char(13)); //0d hexa -> 13
            Serial.print(char(10)); //0a hexa -> 10
            delay(50);
            //hay que sacarlo
            x=10;
        }
      else
      {
         // Serial.print("READ SFI NO OK ");
         // Serial.println(ANS);        
      }
        
      
 
    //Wait 1 second before continuing
    delay(50);
//*******************Lectura PAN*******************
        
  }
} while (x<10);
  }

  


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
*/

void serialEvent() 
{
  while (Serial.available()) 
  {
    // get incoming byte:
    inByte = Serial.read();

    //Serial.println(inByte,HEX);

    // add it to the inputString:
    inputString += inByte;

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    //Serial.println(inputString);
    //if (inByte == 13) 
    if (inByte == 46)  // 2e hex -> 46 dec -> . char ultimo carater enviado por el PC a la pistola
    {
       //Serial.println("un enter");
       stringComplete = true;
    }
    
  }
}
