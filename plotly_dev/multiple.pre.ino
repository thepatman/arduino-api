% if lib=="ethernet":
#include <SPI.h>
#include <Ethernet.h>
#include "plotly_streaming_ethernet.h"
% elif lib=="wifi":
#include <WiFi.h>
#include "plotly_streaming_wifi.h"
% elif lib=="cc3000":
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <plotly_streaming_cc3000.h>

#define WLAN_SSID       "Your_WiFi_Network_Name"
#define WLAN_PASS       "Your_WiFi_Network_Password"
#define WLAN_SECURITY   WLAN_SEC_WPA2
% elif lib=="gsm":
#include <GSM.h>
#include "plotly_streaming_gsm.h"
% endif

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 6
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"8xdfnkq1nb", "xtndiqvpol", "9hacultmgo", "twb02gfo7c", "nr4c87j7tq", "6hkp0pnj0q"};
// arguments: username, api key, streaming token, filename
plotly graph("plotly_username", "plotly_api_key", tokens, "your_filename", nTraces);

% if lib=="wifi":
char ssid[] = "Your_WiFi_Network_Name"; //  your network SSID (name) 
char pass[] = "Your_WiFi_Network_Password"; // // your network password

void wifi_connect(){
    int status = WL_IDLE_STATUS;     // the Wifi radio's status
    // attempt to connect using WPA2 encryption:
    Serial.println("... Attempting to connect to WPA network...");
    status = WiFi.begin(ssid, pass);
    // if you're not connected, stop here:
    if ( status != WL_CONNECTED) { 
      Serial.println("... Couldn't get a WiFi connection, trying again");
      wifi_connect();
    } 
    // if you are connected, print out info about the connection:
    else {
      Serial.println("... Connected to network");
    }
}
% elif lib=="ethernet":
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte my_ip[] = { 199, 168, 222, 18 }; // google will tell you: "public ip address"

void startEthernet(){
    Serial.println("... Initializing ethernet");
    if(Ethernet.begin(mac) == 0){
        Serial.println("... Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, my_ip);
    }
    Serial.println("... Done initializing ethernet");
    delay(1000);
}

% elif lib=="cc3000":
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

% elif lib=="gsm":
void gsm_connect(){
  // ...
}
% endif

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  % if lib=="wifi":
  wifi_connect();
  % elif lib=="ethernet":
  startEthernet();
  % elif lib=="cc3000":
  wifi_connect();
  %elif lib=="gsm":
  gsm_connect();
  % endif

  graph.init();
  graph.openStream();
}

float y0, y1, y2, y3, y4, y5;
void loop() {
  y0 = analogRead(A0);
  graph.plot(millis(), y0, tokens[0]);
  y1 = analogRead(A1);
  graph.plot(millis(), y1, tokens[1]);
  y2 = analogRead(A2);
  graph.plot(millis(), y2, tokens[2]);
  y3 = analogRead(A3);
  graph.plot(millis(), y3, tokens[3]);
  y4 = analogRead(A4);
  graph.plot(millis(), y4, tokens[4]);
  y5 = analogRead(A5);
  graph.plot(millis(), y5, tokens[5]);
}
