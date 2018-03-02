#include <Adafruit_NeoPixel.h> // NeoPixel
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Adafruit_GPS.h> // GPS
#include <Wire.h> // for I2C communication

#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);
     
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

// Neopixel Defines
#define NEOPIN  6
#define NUMPIXELS 3
#define ONBOARDLED 8 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel onboardled = Adafruit_NeoPixel(1, ONBOARDLED, NEO_GRB + NEO_KHZ800);

uint32_t timer = millis();

/* Defining struct for receiving color data from Feather */
struct colorData {
  int red;
  int green;
  int blue;  
};
typedef struct colorData ColorData;
ColorData colori2cPacket; // declare one ColorData struct

///////////////////////// Main Variable SETUP /////////////////////////
  //color of leds
  int red = 0;
  int blue = 0;
  int green = 50;
  // bus size
  int busSize = 2;
  //last pixel written to
  int lastHead = NUMPIXELS - 1;
  int lastTail = lastHead - busSize;

void setup()
{
  ///////////////////////// GPS SETUP /////////////////////////
  
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready
  
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
     
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
     
  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  
  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);


  ///////////////////////// LED NEOPIXEL SETUP /////////////////////////
  pixels.begin(); // inits neopixel library
  onboardled.begin();


  ///////////////////////// I2C or USB w/ Ble Feather SETUP /////////////////////////
  Wire.begin(9); // Start I2C slave on address 9
  Wire.onReceive(updateColor);
}

void updateColor(int bytes) {
  colori2cPacket = Wire.read();
  Serial.print("red: "); 
  Serial.println(colori2cPacket.red); 
  Serial.print("green: "); 
  Serial.println(colori2cPacket.green); 
  Serial.print("blue: "); 
  Serial.println(colori2cPacket.blue); 
}

void loop() // run over and over again
{
  ///////////////////////// Get New Color (if new color set) from BLE Feather /////////////////////////
  




  
  ///////////////////////// Get GPS Data (location, speed) /////////////////////////
  // if gps fix is lost, use last speed used
  
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();
     
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }

  ///////////////////////// Write to next LED /////////////////////////
  // Delay writing to next color based on current speed (high speed = low delay)
  // instead of delaying the whole execution, I could create a counting variable that kept increasing on every cycle, and once it hit a certain point
  // high point for slow, low point for fast, it would write to the next led
  pixels.setPixelColor(lastTail,pixels.Color(0,0,0));
  pixels.setPixelColor(lastHead,pixels.Color(red,green,blue));
  pixels.show();
  lastHead = (lastHead + 1) % NUMPIXELS;
  lastTail = (lastTail + 1) % NUMPIXELS;
  delay(250);
}
