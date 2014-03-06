#include <WiFi.h>

#include <plotly_wifi_streaming.h>

int status = WL_IDLE_STATUS;     // the Wifi radio's status
char ssid[] = ""; //  your network SSID (name) 
char pass[] = ""; // // your network password

void wifi_connect(){
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

// arguments: username, api key, streaming token, filename
plotly logger(username, api key, streaming token, filename);

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  wifi_connect();       // connect to wifi

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
