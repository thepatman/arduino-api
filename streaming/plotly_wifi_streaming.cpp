#include "Arduino.h"
#include <WiFi.h>
#include "plotly_wifi_streaming.h"

#include <avr/dtostrf.h>

plotly::plotly(){
    floatWidth_ = 10;
    floatPrec_ = 5;
    VERBOSE = true;
    DRY_RUN = false;
}

void plotly::begin(char *stream_token){

    Serial.print("Connecting to plotly's servers...");
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
    print_(stream_token);
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
