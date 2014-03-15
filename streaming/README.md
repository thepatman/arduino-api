## The Gist

<p align="center">
<a href="http://vimeo.com/89003132">
<img src="http://new.tinygrab.com/c751bc2ee24505ea0425b28e9c8f2d1f8aa81ad395.png" />
</a>
</p>



## Super easy!

```C
#include <WiFi.h>
#include <plotly_wifi_streaming.h>

// arguments: username, api key, streaming token, filename
plotly logger(username, api_key, streaming_token, filename);

void setup() {

  Serial.begin(9600);

  wifi_connect();

  // start connection to plotly's servers
  logger.begin(50);     // show 50 points in the plot

}

unsigned long x;
int y;
void loop() {
  // now let's stream to plotly, giddyup!
  x = millis();
  y = analogRead(A0);
  logger.plot(x, y);    // plot (x, y) values
  delay(500);
}
```
