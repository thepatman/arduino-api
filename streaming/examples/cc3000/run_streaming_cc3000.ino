#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include <plotly_streaming_cc3000.h>

#define WLAN_SSID       "yourSSID"
#define WLAN_PASS       "yourPassword"
#define WLAN_SECURITY   WLAN_SEC_WPA2


// arguments: username, api key, streaming token, filename
// e.g. logger("my_username", "abcdefghij", "ABCDEFGHIJ", "My plotly filename"); 
// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
plotly logger(username, api key, streaming token, filename);


void wifi_connect(){
  /* Initialise the module */
  Serial.println(F("\n... Initializing..."));
  if (!logger.cc3000.begin())
  {
    Serial.println(F("... Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  // Optional SSID scan
  // listSSIDResults();
  
  if (!logger.cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  Serial.println(F("... Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("... Request DHCP"));
  while (!logger.cc3000.checkDHCP())
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

  logger.begin(50);     // show 50 points in the plot

}

unsigned long x;
int y;
void loop() {
  x = millis();
  y = analogRead(A0);
  logger.plot(x, y);
}
