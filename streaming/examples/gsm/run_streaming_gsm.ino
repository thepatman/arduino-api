#include <GSM.h>
#include "plotly_streaming_gsm.h"


// arguments: username, api key, streaming token, filename
// e.g. logger("my_username", "abcdefghij", "ABCDEFGHIJ", "My plotly filename"); 
// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
plotly logger(username, api key, streaming token, filename);


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

  logger.begin(50);     // show 50 points in the plot

}

unsigned long x;
int y;
void loop() {
  x = millis();
  y = analogRead(A0);
  logger.plot(x, y);
  delay(50);
}
