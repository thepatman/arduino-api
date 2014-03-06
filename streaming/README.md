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
  x = millis();
  y = analogRead(A0);
  logger.plot(x, y);
  delay(500);
}
```
