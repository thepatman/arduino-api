// Automatic date-time conversion and data logging
// While logging, supply the milliseconds since program start along with your data point
// Plotly will check the time when you made the request, and convert your millisecond counter
// into a human-readable date-formatted graph
// This example made this graph: http://plot.ly/~public_arduino/18/

// The Arduino IDE 1.0.5 library is known to have some problems with the <WiFi.h> library. 
// It's recommended that you use the Arduino IDE 1.0.3 which you can download from here: http://arduino.cc/en/Main/OldSoftwareReleases

#include <WiFi.h>
#include "plotly_wifi.h"

plotly plotly; // initialize a plotly object, named plotly

int status = WL_IDLE_STATUS;     // the Wifi radio's status
char ssid[] = "wifi_network_name"; //  your network SSID (name) 
char pass[] = "wifi_password"; // // your network password

void wifi_connect(){
    // attempt to connect using WPA2 encryption:
    Serial.println("Attempting to connect to WPA network...");
    status = WiFi.begin(ssid, pass);
    // if you're not connected, stop here:
    if ( status != WL_CONNECTED) { 
      Serial.println("Couldn't get a WiFi connection");
      while(true);
    } 
    // if you are connected, print out info about the connection:
    else {
      Serial.println("Connected to network");
    }
}

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  wifi_connect();

  plotly.VERBOSE = true; // turn to false to suppress printing over serial
  plotly.DRY_RUN = false; // turn to false when you want to connect to plotly's servers
  plotly.username = "public_arduino"; // your plotly username -- sign up at https://plot.ly/ssu or feel free to use this public account. password of the account is "password"
  plotly.api_key = "4z1r78yx9a"; // "public_arduino"'s api_key -- char api_key[10]
  int nTraces=2; // number of traces/series of the graph
  int nPoints=50; // number of points to transmit for each trace -- all traces must transmit the same number of points
  char layout[]="{}"; // a JSON formatted object that describes the layout, axes, annotations, and title. See layout.ino for some examples
  char filename[] = "millisecond_timestamper"; // name of the plot that will be saved in your plotly account -- resaving to the same filename will simply extend the existing traces with new data

  plotly.timestamp = true; // tell plotly that you're stamping your data with a millisecond counter and that you want plotly to convert it into a date-formatted graph
  plotly.timezone = "America/Montreal"; // full list of timezones is here:

  plotly.open_stream(nPoints, nTraces, filename, layout);
  
  int reading; float freading;
  for(int i=0; i<nPoints; i++){
    reading = analogRead(2);
    freading = 1.1*reading;
    plotly.post(millis(),reading); // trace 1 (implicitly defined by order)
    delay(750);
    plotly.post(millis(), freading); // trace 2
    delay(500);
  }
  // after sending nPoints, the call to plotly is implicitly finished,
  // your data is saved and rendered as a graph in your plotly account and at a unique url
  while(true) { ; }

}

void loop() {
}
