#include "Arduino.h"
#include <WiFi.h>
#include "plotly_wifi_streaming.h"

#include <avr/dtostrf.h>

plotly::plotly(char *username, char *api_key, char *stream_token, char *filename){
    floatWidth_ = 10;
    floatPrec_ = 5;
    VERBOSE = true;
    DRY_RUN = false;
    username_ = username;
    api_key_ = api_key;
    stream_token_ = stream_token;
    filename_ = filename;
}

void plotly::begin(unsigned long maxpoints){
    /* 
    *  Validate a stream with a REST post to plotly 
    */
    if(DRY_RUN){ Serial.println("This is a dry run, we are not connecting to plotly's servers..."); }
    else{
      if(VERBOSE) { Serial.println("Attempting to connect to plotly's servers..."); }
      while ( !client.connect("plot.ly", 80) ) {
        if(VERBOSE){ Serial.println("Couldn\'t connect to servers.... trying again!"); }
        delay(1000);
      }
    }
    if(VERBOSE) Serial.println("Connected to plotly's servers");
    if(VERBOSE) Serial.println("\n== Sending HTTP Post to plotly ==");
    print_("POST /clientresp HTTP/1.1\r\n");
    print_("Host: 107.21.214.199\r\n");
    print_("User-Agent: Arduino/0.5.1\r\n");

    print_("Content-Length: ");
    int contentLength = 202 + len_(username_) + len_(maxpoints) + len_(filename_);
    print_(contentLength);
    /* contentLength = 
    *   44  // first part of querystring below
    * + len_(username)  // upper bound on username length
    * + 5   // &key=
    * + 10  // api_key length
    * + 66  // &args=[...
    * + 10  // stream_token length 
    * + 16  // "\", \"maxpoints\": "
    * + len_(maxpoints)  
    * + 49  // }}]&kwargs={\"fileopt\": \"overwrite\", \"filename\": \"
    * + len_(filename)
    * + 1   // closing "}
    *------
    * 202 + len_(username) + len_(maxpoints) + len_(filename)
    */ 
    print_("\r\n\r\n");

    print_("version=0.2&origin=plot&platform=arduino&un=");
    print_(username_);
    print_("&key=");
    print_(api_key_);
    print_("&args=[{\"y\": [], \"x\": [], \"type\": \"scatter\", \"stream\": {\"token\": \"");
    print_(stream_token_);
    print_("\", \"maxpoints\": ");
    print_(maxpoints);
    print_("}}]&kwargs={\"fileopt\": \"overwrite\", \"filename\": \"");
    print_(filename_);
    print_("\"}");

    /*
     * Wait for a response ...
    */
    // final newline to terminate the post
    print_("\r\n");
    if(VERBOSE) Serial.println("== Sent message, waiting for plotly's response ==");
    if(!DRY_RUN && VERBOSE){
        while(client.connected()){
            if(client.available()){
                char c = client.read();
                Serial.print(c);
            }
        }
        client.stop();
    }    

    /*
     * Assume we're good to go, and initialize request to stream servers
     * TODO: search for "All Streams Go!"
    */

    Serial.print("\nConnecting to plotly's servers...");
    char server[] = "stream.plot.ly"; //"10.0.1.116"; 
    int port = 80; // 8080;
    while ( !client.connect(server, port) ) {
        Serial.println("Couldn\'t connect to servers.... trying again!");
        delay(1000);
    }

    Serial.println("Connected to plotly's servers");

    print_("POST / HTTP/1.1\r\n");
    print_("Host: 127.0.0.1\r\n");
    print_("User-Agent: Python\r\n");
    print_("Transfer-Encoding: chunked\r\n");
    print_("Connection: close\r\n");
    print_("plotly-streamtoken: ");
    print_(stream_token_);
    print_("\r\nplotly-convertTimestamp: America/Montreal");
    print_("\r\n\r\n");
}

void plotly::stop(){
    print_("0\r\n");
    print_("\r\n");
}

void plotly::jsonStart(int i){
    if(VERBOSE) Serial.print(i+15, HEX);    // 15 char for the json that wraps the data: {"x": , "y": }\n
    if(!DRY_RUN) client.print(i+15, HEX);

    print_("\r\n{\"x\": ");
}
void plotly::jsonMiddle(){
    print_(", \"y\": ");
}
void plotly::jsonEnd(){
    print_("}\n\r\n");
}

int plotly::len_(int i){
    if(i > 9999) return 5;
    else if(i > 999) return 4;
    else if(i > 99) return 3;
    else if(i > 9) return 2;
    else return 1;
}
int plotly::len_(unsigned long i){
    // max length of unsigned long: 4294967295
    if(i > 999999999) return 10;
    else if(i > 99999999) return 9;
    else if(i > 9999999) return 8;
    else if(i > 999999) return 7;
    else if(i > 99999) return 6;
    else if(i > 9999) return 5;
    else if(i > 999) return 4;
    else if(i > 99) return 3;
    else if(i > 9) return 2;
    else return 1;
}
int plotly::len_(float i){
    return floatWidth_;
}
int plotly::len_(char *i){
    return strlen(i);
}
int plotly::len_(String i){
    return i.length();
}

void plotly::plot(int x, int y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}
void plotly::plot(int x, float y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}
void plotly::plot(unsigned long x, int y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}
void plotly::plot(unsigned long x, float y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}
void plotly::plot(float x, int y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}
void plotly::plot(float x, float y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}
void plotly::plot(char *x, int y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}
void plotly::plot(char *x, float y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}
void plotly::plot(String x, int y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}
void plotly::plot(String x, float y){
    jsonStart(len_(x)+len_(y));
    print_(x);
    jsonMiddle();
    print_(y);
    jsonEnd();
}

void plotly::print_(int d){
    if(VERBOSE) Serial.print(d);
    if(!DRY_RUN) client.print(d);
}
void plotly::print_(float d){
    char s_[floatWidth_];
    dtostrf(d,floatWidth_,floatPrec_,s_);
    print_(s_);
}
void plotly::print_(String d){
    if(VERBOSE) Serial.print(d);
    if(!DRY_RUN) client.print(d);
}
void plotly::print_(unsigned long d){
    if(VERBOSE) Serial.print(d);
    if(!DRY_RUN) client.print(d);
}
void plotly::print_(char *d){
    if(VERBOSE) Serial.print(d);
    if(!DRY_RUN) client.print(d);
}
