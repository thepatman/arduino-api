// Connect to the internet, connect to Plotly, and 
// send data to Plotly to get rendered as an interactive graph
#include <SPI.h>
#include <Ethernet.h>
#include <plotly_ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
IPAddress ip(192,168,0,177);

void setup() {
  Serial.begin(9600);   // initialize serial:

  if(Ethernet.begin(mac) == 0){
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }

  // give the Ethernet shield a second to initialize:
  delay(1000);  
 
  plotly plotly;                    // initialize plotly object
  String filename = "Arduino-Plotly Stream"; // name of the plot inside your Plotly account
  String username = "anna.lyst";    // your plotly username 
  String api_key = "xxxxxx";       // your plotly api_key. display your api_key on the api docs page: https://plot.ly/api

  int i; int j;
  int N = 30;      // N number of points in each burst of data to plotly. Your graph will update every tranmission of N points
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
