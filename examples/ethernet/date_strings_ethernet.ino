// An example of how to format strings to be interpreted as dates.
// This example generated this graph: http://plot.ly/~public_arduino/16/


#include <SPI.h>
#include <Ethernet.h>
#include "plotly_ethernet.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte my_ip[] = { 199, 168, 222, 18 }; // google will tell you: "public ip address"

void startEthernet(){
    Serial.println("Initializing ethernet");
    if(Ethernet.begin(mac) == 0){
        Serial.println("Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, my_ip);
    }
    Serial.println("Done initializing ethernet");
    delay(1000);
}


void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  startEthernet();

  plotly plotly;
  plotly.VERBOSE = true; // turn to false to suppress printing over serial
  plotly.DRY_RUN = false; // turn to false when you want to connect to plotly's servers
  plotly.username = "public_arduino"; // your plotly username -- sign up at https://plot.ly/ssu or feel free to use this public account. password of the account is "password"
  plotly.api_key = "4z1r78yx9a"; // "public_arduino"'s api_key -- char api_key[10]
  int nTraces=2; // number of traces/series of the graph
  int nPoints=3; // number of points to transmit for each trace -- all traces must transmit the same number of points
  char layout[]="{}"; // a JSON formatted object that describes the layout, axes, annotations, and title. See layout.ino for some examples
  char filename[] = "date_strings_example"; // name of the plot that will be saved in your plotly account -- resaving to the same filename will simply extend the existing traces with new data

  plotly.open_stream(nPoints, nTraces, filename, layout);
  
  int reading; float freading;
  reading = analogRead(2);
  freading = 1.1*reading;
  // To manually time stamp the data, the dates must be in the format YYYY-MM-DD HH:MM:SS
  plotly.post("2013-12-05 22:33:00",reading); // trace 1 (implicitly defined by order)
  plotly.post("2013-12-05 22:33:10", freading); // trace 2
  delay(500);
  reading = analogRead(2);
  freading = 1.1*reading;
  plotly.post("2013-12-05 22:34:00",reading); // back to trace 1
  plotly.post("2013-12-05 22:35:10", freading); // trace 2
  delay(500);
  reading = analogRead(2);
  freading = 1.1*reading;
  plotly.post("2013-12-05 22:34:30",reading); // back to trace 1
  plotly.post("2013-12-05 22:35:50", freading); // trace 2
  while(true){;}
}

void loop() { }
