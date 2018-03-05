#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <EasyTransferI2C.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

EasyTransferI2C ET;

struct SEND_DATA_STRUCTURE{
  uint8_t red;
  uint8_t green;
  uint8_t blue; 
};

SEND_DATA_STRUCTURE mydata;

#define I2C_SLAVE_ADDRESS 9

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint8_t mypacket[], uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];

/* The service information */
int32_t colorServiceId;
int32_t colorCharId;


void setup(void)
{
//  while (!Serial); // required for Flora & Micro
  delay(500);

  boolean success;

  Serial.begin(115200);
  Serial.println(F("Getting Data POC"));
  Serial.println(F("---------------------------------------------------"));

  randomSeed(micros());

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Perform a factory reset to make sure everything is in a known state */
  Serial.println(F("Performing a factory reset: "));
  if (! ble.factoryReset() ){
       error(F("Couldn't factory reset"));
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  // this line is particularly required for Flora, but is a good idea
  // anyways for the super long lines ahead!
  // ble.setInterCharWriteDelay(5); // 5 ms
  ble.verbose(false);

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'BLE Jacket': "));

  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=BLE Jacket")) ) {
    error(F("Could not set device name?"));
  }

  /* Add the Color Service definition */
  /* Service ID should be 1 */
  Serial.println(F("Adding the Color Service definition (UUID = 0x3214): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID=0x3214"), &colorServiceId);
  if (! success) {
    error(F("Could not add Color service"));
  }

  /* Add the Color characteristic */
  /* Chars ID for Measurement should be 1 */
  Serial.println(F("Adding the Color characteristic (UUID = 0x1234): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x1234, PROPERTIES=0x08, MIN_LEN=3, MAX_LEN=3, VALUE=255-255-255"), &colorCharId);
    if (! success) {
    error(F("Could not add Color characteristic"));
  }

  /* Reset the device for the new service setting changes to take effect */
  Serial.print(F("Performing a SW reset (service changes require a reset): "));
  ble.reset();

  /* Print Bluefruit information */
  Serial.print("Printing bluetooth information");
  ble.info();

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }
  Serial.print(F("Received Connection!!!!!!!!"));


  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println();

  Wire.begin();
  ET.begin(details(mydata),&Wire);
  Serial.print("Began i2c master side");
}

void loop(void)
{
  uint8_t mypacket[20];
  /* Wait for new data to arrive */
  uint8_t len = readPacket(&ble, mypacket, BLE_READPACKET_TIMEOUT);
  if (len == 0) return;

  Serial.print("Got a packet!");

//  /* Got a packet! */
//  printHex(packetbuffer, len);

  for (int i = 0; i < len; i++) {
    Serial.print("index ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(mypacket[i]);
  }

  // Color
  if (mypacket[1] == 1) {
    uint8_t red = mypacket[2];
    uint8_t green = mypacket[3];
    uint8_t blue = mypacket[4];
    Serial.print ("RGB #");
    if (red < 0x10) Serial.print("0");
    Serial.print(red, HEX);
    if (green < 0x10) Serial.print("0");
    Serial.print(green, HEX);
    if (blue < 0x10) Serial.print("0");
    Serial.println(blue, HEX);

    mydata.red = red;
    mydata.green = green;
    mydata.blue = blue;

    Serial.print("about to send: ");
    Serial.println(mydata.red);
    Serial.println(mydata.green);
    Serial.println(mydata.blue);
    ET.sendData(I2C_SLAVE_ADDRESS);
  }  


































  

  /* Delay before next measurement update */
  delay(1000);
}
