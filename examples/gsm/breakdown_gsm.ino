#include <GSM.h>
#include "plotly_streaming_gsm.h"


// arguments: username, api key, streaming token, filename
// e.g. plotly graph("my_username", "abcdefghij", "ABCDEFGHIJ", "My plotly filename"); 
// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
char *tokens[] = {"8xdfnkq1nb","xtndiqvpol"};
plotly graph("streaming-demos", "3yyglqsi85", tokens, "filename");


void gsm_connect(){
  // ...
}

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  gsm_connect();

  graph.init();
  graph.openStream();

}

unsigned long x;
unsigned long y = 0;
unsigned long i = millis();
void loop() {
  if(!graph.client.connected()){
    Serial.println(F("... Lost connection, going to connect again"));
    graph.closeStream();
    graph.openStream();
    delay(1000);
  }

  x = millis();
  y += 1;
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();

  graph.jsonStart(graph.len_(x)+graph.len_(y));
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();

  graph.print_(x);
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();

  graph.jsonMiddle();
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();


  graph.print_(y);
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();

  graph.jsonEnd(tokens[0]);
  // Serial.println(millis()-i);
  i = millis();
//  delay(250);

////////////////////////////////
  x = millis();
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();

  graph.jsonStart(graph.len_(x)+graph.len_(y));
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();

  graph.print_(x);
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();

  graph.jsonMiddle();
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();


  graph.print_(y);
  // Serial.print(millis()-i);
  // Serial.print("\t");
  i = millis();

  graph.jsonEnd(tokens[1]);
  // Serial.println(millis()-i);
  i = millis();
//  delay(250);
}
