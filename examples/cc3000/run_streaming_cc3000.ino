#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include <plotly_streaming_cc3000.h>

#define WLAN_SSID       "StationC"
#define WLAN_PASS       "stationcgaspe"
#define WLAN_SECURITY   WLAN_SEC_WPA2

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 6
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"8xdfnkq1nb", "xtndiqvpol", "9hacultmgo", "twb02gfo7c", "nr4c87j7tq", "6hkp0pnj0q"};
// arguments: username, api key, streaming token, filename
plotly graph("streaming-demos", "3yyglqsi85", tokens, "filename", nTraces);

void wifi_connect(){
  /* Initialise the module */
  Serial.println(F("\n... Initializing..."));
  if (!graph.cc3000.begin())
  {
    Serial.println(F("... Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  // Optional SSID scan
  // listSSIDResults();
  
  if (!graph.cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  Serial.println(F("... Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("... Request DHCP"));
  while (!graph.cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }
}


void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  wifi_connect();


  graph.init();
  graph.openStream();
}

unsigned long x;
int y;

void loop() {
  graph.plot(millis(), analogRead(A0), tokens[0]);
  graph.plot(millis(), analogRead(A1), tokens[1]);
  graph.plot(millis(), analogRead(A2), tokens[2]);
  graph.plot(millis(), analogRead(A3), tokens[3]);
  graph.plot(millis(), analogRead(A4), tokens[4]);
  graph.plot(millis(), analogRead(A5), tokens[5]);
}
