// This example generates this graph: http://plot.ly/~public_arduino/14/
// The Arduino IDE 1.0.5 library is known to have some problems with the <WiFi.h> library. 
// Its recommended that you use the Arduino IDE 1.0.3 which you can download from here: http://arduino.cc/en/Main/OldSoftwareReleases

#include <WiFi.h>

int status = WL_IDLE_STATUS;     // the Wifi radio's status
char ssid[] = "StationC"; //  your network SSID (name) 
char pass[] = "stationcgaspe"; // // your network password

void wifi_connect(){
    // attempt to connect using WPA2 encryption:
    Serial.println("Attempting to connect to WPA network...");
    status = WiFi.begin(ssid, pass);
    // if you're not connected, stop here:
    if ( status != WL_CONNECTED) { 
      Serial.println("Couldn't get a WiFi connection, trying again");
      wifi_connect();
    } 
    // if you are connected, print out info about the connection:
    else {
      Serial.println("Connected to network");
    }
}

WiFiClient client1;
WiFiClient client2;

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  wifi_connect();       // connect to wifi


  char server[] = "stream.plot.ly";
  int port = 80;

  // client 1
  while ( !client1.connect(server, port) ) {
    Serial.println("Couldn\'t connect to servers.... trying again!");
    delay(1000);
  }
  while ( !client2.connect(server, port) ) {
    Serial.println("Couldn\'t connect to servers.... trying again!");
    delay(1000);
  }  
  Serial.println("Connected to plotly's servers");
  client1.print("POST / HTTP/1.1\r\n");
  client2.print("POST / HTTP/1.1\r\n");
  client1.print("Host: 127.0.0.1\r\n");
  client2.print("Host: 127.0.0.1\r\n");
  client1.print("User-Agent: Arduino\r\n");
  client2.print("User-Agent: Arduino\r\n");
  client1.print("Transfer-Encoding: chunked\r\n");
  client2.print("Transfer-Encoding: chunked\r\n");
  client1.print("Connection: close\r\n");
  client2.print("Connection: close\r\n");
  client1.print("plotly-streamtoken: seeetd4j75");
  client2.print("plotly-streamtoken: sjrxmb3ejg");
  client1.print("\r\n\r\n");  
  client2.print("\r\n\r\n");  

  client1.print("11\r\n");
  client2.print("11\r\n");
  client1.print("{\"x\": 1, \"y\": 1}\n\r\n");
  client2.print("{\"x\": 1, \"y\": 2}\n\r\n");
  client1.print("11\r\n");
  client2.print("11\r\n");
  client1.print("{\"x\": 2, \"y\": 2}\n\r\n");
  client2.print("{\"x\": 2, \"y\": 3}\n\r\n");
  client1.print("11\r\n");
  client2.print("11\r\n");
  client1.print("{\"x\": 3, \"y\": 3}\n\r\n");
  client2.print("{\"x\": 3, \"y\": 4}\n\r\n");
  client1.print("0\r\n");
  client2.print("0\r\n");
  client1.print("\r\n");
  client2.print("\r\n");

  Serial.println("all done");
}


void loop() { 

}
