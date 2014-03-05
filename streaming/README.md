## super easy!

```C
#include <plotly_wifi_streaming.h>

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  wifi_connect();       // connect to wifi

  logger.begin("stream_token");

}

int i = 0;
int y;
void loop() {

  y = analogRead(A0);
  logger.plot(millis(), y);
  delay(2000);
  i++;

}


```
