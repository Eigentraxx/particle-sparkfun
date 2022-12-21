/*
    gps functions using sparkfun ublox libraries
    get data and publish.
    all published data will go thorough gps file for now

*/

#include "gps.h"

#include "SparkFun_Ublox_Arduino_Library.h"

#include <ArduinoJson.h>

#include <Wire.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

char GUID[40];
int t = 0;
char * szTemp = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
char * szHex = "0123456789abcdef-";
int nLen = strlen(szTemp);

SFE_UBLOX_GPS myGPS;

char * company = "device";

const char * PUBLISH_EVENT_NAME = "test1data";


// atom deprec. sublime test to github
// used in setup() startgps polling
void setupGpsModule() {
  Wire.begin();

  if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
  {
    Serial.println(F("Ublox GPS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGPS.saveConfiguration(); //Save the current settings to flash and BBR

}

void getGpsData(char * dbchange) {
  StaticJsonDocument < 300 > doc;
  doc["dblocation"] = String(dbchange);
  doc["sensor1"] = "gps";
  long latitude = myGPS.getLatitude();
  doc["latitude"] = latitude;
  long longitude = myGPS.getLongitude();
  doc["longitude"] = longitude;
  long altitude = myGPS.getAltitude();
  doc["altitude"] = altitude;
  byte SIV = myGPS.getSIV();
  doc["SIV"] = SIV;
  long speed = myGPS.getGroundSpeed();
  doc["speed"] = speed;
  long heading = myGPS.getHeading();
  doc["heading"] = heading;
  int pDOP = myGPS.getPDOP();
  doc["pdop"] = (pDOP / 100.0, 2);
  // javascript has ruined me
  doc["time"] = String(myGPS.getYear()) + "-" + String(myGPS.getMonth()) + "-" + String(myGPS.getDay()) + " :: " + String(myGPS.getHour()) + ":" + String(myGPS.getMinute()) + ":" + String(myGPS.getSecond());
  byte fixType = myGPS.getFixType();
  doc["fix"] = fixType;
  byte RTK = myGPS.getCarrierSolutionType();
  doc["RTK"] = RTK;
  doc["device_id"] = "eigen_one";
  String s = String(GUID);

  doc["guid"] = s;

  String output; // string for publish
  serializeJson(doc, output);
  serializeJsonPretty(doc, Serial);
  Particle.publish(company, output, PRIVATE); // push this call webhook to push to firebase

  // using the adruino json library and string library c & c++ purists will not be happy
  // but i'm tired of formating json strings and program is fat but happy like me.
  //char buf[256];
  //snprintf(buf, sizeof(buf), "{\"latitude\":%d,\"longitude\":%d,\"SIV\":%d,\"altitude\":%d,\"fix\":%d}", latitude, longitude, SIV, altitude, fixType);
  // Particle.publish(PUBLISH_EVENT_NAME, buf, PRIVATE);
  //Serial.println(buf);
}

/**
 * [guidFunc description]:  used in setup() to generate guid. used internally for firbase database no need to be all that secure just unique.
 */
void guidFunc() {
  srand((unsigned int) mills());

  for (t = 0; t < nLen + 1; t++) {
    int r = rand() % 16;
    char c = ' ';

    switch (szTemp[t]) {
    case 'x':
      {
        c = szHex[r];
      }
      break;
    case 'y':
      {
        c = szHex[r & 0x03 | 0x08];
      }
      break;
    case '-':
      {
        c = '-';
      }
      break;
    case '4':
      {
        c = '4';
      }
      break;
    }

    GUID[t] = (t < nLen) ? c : 0x00;
  }
}
