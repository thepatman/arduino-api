// This example generates this graph: http://plot.ly/~public_arduino/14/

#include <GSM.h>
#include "plotly_gsm.h"

plotly plotly;

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

  plotly.VERBOSE = true; // turn to false to suppress printing over serial
  plotly.DRY_RUN = false; // turn to false when you want to connect to plotly's servers
  plotly.username = "public_arduino"; // your plotly username -- sign up at https://plot.ly/ssu or feel free to use this public account. password of the account is "password"
  plotly.api_key = "4z1r78yx9a"; // "public_arduino"'s api_key -- char api_key[10]
  int nTraces=3; // number of traces/series of the graph
  int nPoints=2; // number of points to transmit for each trace -- all traces must transmit the same number of points
  char layout[]="{}"; // a JSON formatted object that describes the layout, axes, annotations, and title. See layout.ino for some examples
  char filename[] = "simple_example"; // name of the plot that will be saved in your plotly account -- resaving to the same filename will simply extend the existing traces with new data

  plotly.open_stream(nPoints, nTraces, filename, layout);

  // plotly graphs are constructed by logging a single point consecutively of the nTraces,
  // in other words, the order of the calls is important and all of the traces have the same number of points  
  plotly.post(1, 10); // log first point on trace 0 with x=1, y=10
  plotly.post((float)1.5, 20); // log first point on trace 1 with x=1.5, y=20
  plotly.post((float)1.25, 30); // log first point on trace 2 with x=1.25, y=30

  plotly.post(2, 15); // log second point on trace 0 with x=1, y=15
  plotly.post((float)2.5, 25); // log second point on trace 1 with x=1.5, y=25
  plotly.post((float)3.25, 35); // log second point on trace 2 with x=1.25, y=15
  // Now, after posting the 6 points (3 traces, 2 points per trace), the call is implicitly finished, 
  // and plotly saves your data and makes renders an interactive graph of the data in your browser

  while(true){ ; }
}

void loop() { }
