#include "Arduino.h"
#include <WiFi.h>
#include "plotly_wifi.h"
#include <stdlib.h>

plotly::plotly(){
    VERBOSE = true;
}

void plotly::open_stream(int N, int M, String filename, String username, String api_key){
    ni_ = 0; // number of integers transmitted
    mi_ = 0;
    N_ = N;  // number sets of data sent
    M_ = M; // number of traces * 2
    nChar_ = 0; // number of characters transmitted
    if(VERBOSE) Serial.println("connecting to plotly's servers");
    char server[] = "plot.ly";
    while ( !client.connect(server, 80) ) {
            if(VERBOSE) Serial.println("couldn\'t connect to server. trying again.");
            delay(1000);
        }

    if(VERBOSE) Serial.println("connected to plotly's servers");

    // HTTP Meta
    client.println("POST /clientresp HTTP/1.1");
    client.println("Host: 107.21.214.199"); 
    client.println("User-Agent: Arduino/2.0");

    // start the post string
    header = "version=0.2&origin=plot&platform=arduino&un=";
    header += username;
    header += "&key=";
    header += api_key;
    header += "&kwargs={\"filename\": \"";
    header += filename;
    header += "\", \"fileopt\": \"extend\", \"transpose\": True}&args=";
    /*
        [[x1, y1, x2, y2],
         [x1, y1, x2, y2],
         [x1, y1, x2, y2]]
    */


    // compute an upper bound on the post body size
    upper_ = header.length() // header length ...
        + (N_*M_-1)*2   // + 2-chars for comma and space for all but the last numbers ... 
        + (N_-1)*4  // + 4-chars for square brackets, comma, space for n-1 set of points [], 
        + 2         // + 2-chars for start 'n finish square braces
        + 0         // + [6-chars for the largest int (-32767)] * each int ...
        + 56*N_*M_  // + [(48-chars for largest float integer left of the decimal (-3.4028235E+38) )+(1 decimal pt)+(7 digits of precision right of the decimal)=56-chars]*each float
        + 0;         // + n-chars for upper bound of String * each string ...
    
    s = "Content-Length: "; 
    s += upper_;
    client.println(s);

    // send the header to plotly
    client.println();
    client.print(header);
    nChar_ += header.length();
}

// overloaded permutations, how fun!
plotly::post(int y){
    send(""+y);
}
plotly::post(int x, int y){
    send(""+x);
    send(""+y);
}
plotly::post(int x, float y){
    send(""+x);
    send(dtostrf(y,1,1,1));
}
plotly::post(float x, int y){
    send(dtostrf(x,1,1,1));
    send(""+y);
}
plotly::post(float x, float y){
    send(dtostrf(x,1,1,1));
    send(""+y);
}
plotly::post(String x, int y){
    send(x);
    send(""+y);
}
plotly::post(String x, float y){
    send(x);
    send(dtostrf(x,1,1,1));
}

plotly::send_(String s){
    // prints [x1, y1, x2, y2, ...] N x M
    mi_ += 1;
    if(mi_ == 1){
        ni_ += 1;
        s = "[";
    } else if(mi_ == M_ && ni_ < N_){
        s = s + "], ";
        mi_ = 0;
    } else if(mi_ == M_ && ni_ == N_){
        s = s + "]]";
    }
    if(VERBOSE) Serial.println(s);
    nChar_ += s.length();
    client.print(s);
    if(mi_ == M_ && ni_ == N_){ close_stream(); }
}

void plotly::close_stream(int data){
    // fill the remainder of the post with white space
    for(i=nChar_; i<upper_; i++){
        client.print(" ");
    }
    // final newline to terminate the post
    client.println();
    client.stop();
    if(VERBOSE) Serial.println("closed connection with plotly's servers");
    return;
}