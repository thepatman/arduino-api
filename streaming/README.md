## super easy!

```C
#include <plotly_wifi_streaming.h>

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  wifi_connect();       // connect to wifi

  logger.begin("stream_token");

}

void loop() {

  logger.plot(millis(), analogRead(A0));
  delay(2000);
  
}


```
