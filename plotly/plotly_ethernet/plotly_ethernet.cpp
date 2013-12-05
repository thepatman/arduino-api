#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#include "plotly_ethernet.h"
#include <avr/dtostrf.h>

plotly::plotly(){
    width_ = 8;
    prec_ = 6;
    DEBUG = false;
    VERBOSE = true;
    DRY_RUN = true;
    maxStringLength = 0;
    layout = "{}";
    timestamp = false;
    timezone = "UTC";
}

void plotly::open_stream(int N, int M, String filename, char *username, char *api_key){
    //s_[max(maxStringLength,width_+prec_+1)];
    ni_ = 0; // number of integers transmitted
    mi_ = 0; 
    N_ = N;  // number sets of data sent
    M_ = M*2; // number of traces
    nChar_ = 0; // number of characters transmitted
    //if(VERBOSE) Serial.println("connecting to plotly's servers");
    filename_=filename;
    delay(1000);
    if(DRY_RUN) Serial.println("This is a dry run, we are not connecting to plotly's servers...");
    else{
        if(VERBOSE) Serial.println("Attempting to connect to plotly's servers...");
        // char server[] = "plot.ly";
        byte server[] = { 54, 227, 53, 246 }; // plotly dev
        while ( !client.connect(server, 80) ) {
                if(VERBOSE) Serial.println("Couldn\'t connect to servers.... trying again!");
                delay(1000);
            }
        if(VERBOSE) Serial.println("Connected to plotly's servers");

    }
    if(VERBOSE) Serial.println("\n== Sending HTTP Post to plotly ==");
    // HTTP Meta
    if(VERBOSE){ Serial.println("POST /clientresp HTTP/1.1"); }
    if(!DRY_RUN) { client.println("POST /clientresp HTTP/1.1"); }
    if(VERBOSE){ Serial.println("Host: 107.21.214.199");  }
    if(!DRY_RUN) { client.println("Host: 107.21.214.199");  }
    if(VERBOSE){ Serial.println("User-Agent: Arduino/2.0"); }
    if(!DRY_RUN) { client.println("User-Agent: Arduino/2.0"); }

    // start the post string
    String querystring = "version=0.2&origin=plot&platform=arduino&un=";
    querystring += username;
    querystring += "&key=";
    querystring += api_key;
    querystring += "&args=";
    /*
        [[x1, y1, x2, y2],
         [x1, y1, x2, y2],
         [x1, y1, x2, y2]]
    */
    /*
    kwargs_ = "&kwargs={\"filename\": \"";
    kwargs_ += filename;
    kwargs_ += "\", \"fileopt\": \"extend\", \"transpose\": true, \"layout\": ";
    kwargs_ += layout;
    if(timestamp){
      kwargs_ += ", \"convertTimestamp\": true";
      kwargs_ += ", \"timezone\": \"";
      kwargs_ += timezone;
      kwargs_ += "\"";
      kwargs_ += ", \"sentTime\": ";
    } else{
      kwargs_ += "}";
    }
    */

    // compute an upper bound on the post body size
    upper_ = querystring.length()+200 // querystring length + an upper bound on kwargs...
        + (N_*M_-1)*2   // + 2-chars for comma and space for all but the last numbers ... 
        + (N_-1)*4  // + 4-chars for square brackets, comma, space for n-1 set of points [], 
        + 2         // + 2-chars for start 'n finish square braces
        + 0         // + [6-chars for the largest int (-32767)] * each int ...
        + max(56,maxStringLength)*N_*M_  // + [(48-chars for largest float integer left of the decimal (-3.4028235E+38) )+(1 decimal pt)+(7 digits of precision right of the decimal)=56-chars]*each float
        + 0;         // + n-chars for upper bound of String * each string ...
    String s = "Content-Length: ";
    s += upper_;

    // send the header to plotly
    if(VERBOSE){ Serial.println(s); }
    if(!DRY_RUN) { client.println(s); }
    if(VERBOSE){ Serial.println(""); }
    if(!DRY_RUN) { client.println(""); }
    if(VERBOSE) {Serial.print(querystring);}
    if(!DRY_RUN) {client.print(querystring);}
    nChar_ += querystring.length();    
}

void plotly::send_(String s){
  if(DEBUG){ Serial.print("in send_: "); Serial.println(s); }
 // prints [x1, y1, x2, y2, ...] N x M
  mi_ += 1;
  if(mi_ == 1){
      ni_ += 1;
      if(ni_==1){
          s = "[["+s;
          s = s+", ";
      } else{
          s = "["+s;
          s = s+", ";
      }
  } else if(mi_ == M_ && ni_ < N_){
      s = s+"], ";
      mi_ = 0;
  } else if(mi_ == M_ && ni_ == N_){
      s = s+"]]";
  } else{
      s = s+", ";
  }
  nChar_ += s.length();
  if(VERBOSE) Serial.print(s);
  if(!DRY_RUN) client.print(s);
  if(DEBUG){ Serial.print("\nout send_: "); Serial.println(s); }
  if(mi_ == M_ && ni_ == N_){ 
      close_stream(); 
  }
}


void plotly::sendString_(String s){
  if(DEBUG){ Serial.print("in sendString_: "); Serial.println(s); }

  s = "\""+s;
  s = s+"\"";

  if(DEBUG){ Serial.print("out sendString_: "); Serial.println(s); }
  send_(s);
}
void plotly::sendString_(float d){
    dtostrf(d,width_,prec_,s_);
    send_(String(s_)); 
}
void plotly::sendString_(int d){ 
  send_(String(d)); 
}


void plotly::post(int x, int y){        sendString_(x); sendString_(y); }
void plotly::post(int x, float y){      sendString_(x); sendString_(y); }
void plotly::post(float x, int y){      sendString_(x); sendString_(y); }
void plotly::post(float x, float y){    sendString_(x); sendString_(y); }
void plotly::post(String x, int y){     
  if(DEBUG){ Serial.print("\n\n\nin post: "); Serial.println(x); }
  sendString_(x); sendString_(y); 
}

void plotly::post(String x, float y){   
  if(DEBUG){ Serial.print("\n\n\nin post: "); Serial.println(x); }
  sendString_(x); sendString_(y); 
}

void plotly::close_stream(){
    Serial.print( "&kwargs={\"filename\": \"" );
    client.print( "&kwargs={\"filename\": \"" );
    nChar_+=22;
    
    Serial.print(filename_);
    client.print(filename_);
    nChar_+=filename_.length();
    
    Serial.print("\", \"fileopt\": \"extend\", \"transpose\": true, \"layout\": ");
    client.print("\", \"fileopt\": \"extend\", \"transpose\": true, \"layout\": ");
    nChar_+=53;
    
    Serial.print(layout);
    client.print(layout);
    nChar_+=String(layout).length();

    if(timestamp){
      Serial.print(", \"convertTimestamp\": true");
      client.print(", \"convertTimestamp\": true");
      nChar_+=26;
      
      Serial.print(", \"timezone\": \"");
      client.print(", \"timezone\": \"");
      nChar_+=15;
      
      Serial.print(timezone);
      client.print(timezone);
      nChar_+=String(timezone).length();
      
      Serial.print("\"");
      client.print("\"");
      nChar_+=1;
      
      Serial.print(", \"sentTime\": ");
      client.print(", \"sentTime\": ");
      nChar_+=13;

      String sT = String((int)millis());
      Serial.print(sT);
      client.print(sT);
      nChar_+=sT.length();

      Serial.print("}");
      client.print("}");
      nChar_+=1;      
    } else{
      Serial.print("}");
      client.print("}");
      nChar_+=1;
    }

    if(VERBOSE){ Serial.println(kwargs_); }
    if(!DEBUG) { client.println(kwargs_); }
    // fill the remainder of the post with white space
    if(nChar_>=upper_){ 
      Serial.println("upperbound was wrong!");
      Serial.println(nChar_); Serial.println(upper_);
    }
    for(int i=nChar_; i<upper_; i++){
        if(!DRY_RUN) client.print(" ");
    }
    // final newline to terminate the post
    if(VERBOSE){ Serial.println(""); }
    if(!DEBUG) { client.println(""); }
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
    return;
}

