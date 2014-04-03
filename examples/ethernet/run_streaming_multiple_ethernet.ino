#include <SPI.h>
#include <Ethernet.h>
#include "plotly_streaming_ethernet.h"

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 6
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"8xdfnkq1nb", "xtndiqvpol", "9hacultmgo", "twb02gfo7c", "nr4c87j7tq", "6hkp0pnj0q"};
// arguments: username, api key, streaming token, filename
plotly graph("streaming-demos", "3yyglqsi85", tokens, "filename", nTraces);

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
