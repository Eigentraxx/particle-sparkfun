// This #include statement was automatically added by the Particle IDE.
#include <ArduinoJson.h>

// This #include statement was automatically added by the Particle IDE.

#include "SparkFun_Ublox_Arduino_Library.h"

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "gps.h"

long lastTime = 0; //Simple local timer.
int httpCmd(String command);
int httpdbSet(String cmd); // change database location, i.e. firebase reference

int httpStat = 0;

char dbLoc[100] = "default"; // always default to start
char * message = "eigen_one"; // particle boron identifier corresponding to firebase db

void setup() {
  Serial.begin(9600);
  guidFunc();
  setupGpsModule();
  Particle.function("httpCmd", httpCmd);
  Particle.function("httpdbSet", httpdbSet);
  Particle.variable("id", message, STRING);

}

void loop() {
  // every 60 seconds for testing
  if (millis() - lastTime > 60000) {
    lastTime = millis(); //Update the timer
    getGpsData(dbLoc);
    if (httpStat == 1) { //  poll stat end when true
      while (1) {} // end program
    }
  }

}

/**
 * [httpdbSet description]: set database location from http request
 * @param  cmd [description] : new string from http used by web hook when data is sent to firebase
 * @return integer  [description] : just any number for now.
 */
int httpdbSet(String cmd) {
  strcpy(dbLoc, cmd);
  // dbLoc = cmd;
  return 4;

}

/**
 * [httpCmd description] : string from http request
 * @param  command [description]: request String off sleep gps
 * @return integer      [description] integer still need to assign to webapp
 */
int httpCmd(String command) {

  if (command == "off") {
    httpStat = 1;
    return 1;
  }
  if (command == "sleep") {
    // this doesn't work.....
    System.sleep({}, {}, 60);

  }
  if (command == "callgps") {
    getGpsData(dbLoc);
    return 8;
  } else return -1;
}
