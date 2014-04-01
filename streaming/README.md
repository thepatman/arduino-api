<p align="center">
<a href="http://vimeo.com/89003132">
<img src="http://new.tinygrab.com/c751bc2ee24505ea0425b28e9c8f2d1f8aa81ad395.png" />
</a>
</p>



## Super easy!

```C
#include <WiFi.h>
#include <plotly_wifi_streaming.h>

#define num_traces 2
char *streaming_tokens[num_traces] = {"your_plotly_stream_token", "another_plotly_stream_token"};
plotly graph("your_plotly_username", 
            "your_plotly_api_key", 
            streaming_tokens, 
            "your_plotly_filename", 
            num_traces);

void setup() {

  Serial.begin(9600);

  wifi_connect();

  // Initialize a streaming graph in your plotly account
  graph.init();
  // Initialize plotly's streaming service
  graph.openStream(); 
}

void loop() {
  // now let's stream-data to plotly, giddyup!
  graph.plot(millis(), analogRead(A0), tokens[0]);
  graph.plot(millis(), analogRead(A1), tokens[1]);
}
```
