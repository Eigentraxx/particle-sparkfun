// This #include statement was automatically added by the Particle IDE.
#include <ArduinoJson.h>

// This #include statement was automatically added by the Particle IDE.
#include "gps.h"
#include "SparkFun_Ublox_Arduino_Library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


long lastTime = 0; //Simple local timer.
int httpCmd(String command);

int httpStat = 0;

void setup()
{
  Serial.begin(9600);
    guidFunc();
    setupGpsModule();
    Particle.function("httpCmd", httpCmd);
}

void loop()
{
  // every 30 seconds for testing
  if (millis() - lastTime > 60000)
  {
    lastTime = millis(); //Update the timer
    getGpsData();
    if (httpStat == 1){    //  poll stat end when true
       while(1) { } // end program
    }
  }
}
http function
/**
 * call back on particle function
 * @param  command - string from http ; currently 'callgps' & 'off'
 * @return     returns an integer 1 === off 
 */
 int httpCmd(String command)
{
  // look for the matching argument "coffee" <-- max of 64 characters long
  if(command == "off")
  {
      httpStat = 1;
    return 1;
  }
   if(command == "callgps")
  {
    getGpsData();
    return 8;
  }
  else return -1;
}
