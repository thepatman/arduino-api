#include <SPI.h>
#include <Ethernet.h>
#include "plotly_streaming_ethernet.h"

char *tokens[] = {"abcdefghij","mnopqrstuv"};
plotly graph("username", "api_key", tokens, "filename");

unsigned long x;
int y;

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


void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  startEthernet();

  graph.dry_run=true;
  graph.begin();

  x = millis();
  y = analogRead(A0);
  graph.plot(x, y, tokens[0]);
  graph.plot(x, y, tokens[1]);
}


void loop() {

}
