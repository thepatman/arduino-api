// Connect to the internet, connect to Plotly, and 
// send data to Plotly to get rendered as an interactive graph
#include <WiFi.h>
#include <plotly_wifi.h>

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
  Serial.begin(9600);   // initialize serial:

  wifi_connect();       // connect to wifi

  plotly plotly;                    // initialize plotly object
  String filename = "Arduino-Plotly Stream"; // name of the plot inside your Plotly account
  String username = "anna.lyst";      // your plotly username 
  String api_key = "xxxxxxxxxx";      // your plotly api_key. display your api_key on the api docs page: https://plot.ly/api

  int i; int j; 
  int N = 30;       // N number of points in each burst of data to plotly. Your graph will update every tranmission of N points
  int Nburst = 1;  // number of bursts. In total, the plot will contain N*NBurst (300) points

  // Transmit NBurst sets of N points to your plotly account
  for(i=0; i<Nburst; i++){
    // Transmit N points to your plotly account
    plotly.open_stream(N, filename, username, api_key); // (re)start connection to plotly
    for(j=0; j<N; j++){
        plotly.post(analogRead(3)); // send integer data to plotly
        delay(500);
    }
    // after transmitting N points, Plotly updates a graph of this data in your account
    // each burst of data extends the trace of the plot by N points. 
    delay(500);
  }
}
void loop(){}
