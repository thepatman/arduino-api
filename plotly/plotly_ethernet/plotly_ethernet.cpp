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
    timezone = "America/Montreal";
}

void plotly::open_stream(int N, int M, String filename_, String username, String api_key){
    //s_[max(maxStringLength,width_+prec_+1)];
    ni_ = 0; // number of integers transmitted
    mi_ = 0; 
    N_ = N;  // number sets of data sent = number of rows in plotly-json data matrix
    M_ = (M*2); // number of traces * 2 = number of columns in plotly-json data matrix
    nChar_ = 0; // counter of number of characters transmitted
    filename=filename_;
    delay(1000);
    if(DRY_RUN){ Serial.println("This is a dry run, we are not connecting to plotly's servers..."); }
    else{
      int try_connect = 0;
      if(VERBOSE) { Serial.println("Attempting to connect to plotly's servers..."); }
      byte plotly_ip[] = { 107, 21, 214, 199 };
      while ( !client.connect(plotly_ip, 80) && try_connect < 20) {
        if(VERBOSE){ Serial.println("Couldn\'t connect to servers.... trying again!"); }
        delay(1000);
        try_connect += 1;
      }
      if(try_connect==20){
        if(VERBOSE){ Serial.println("Attempting to connect via IP, instead"); }
        char server[] = "plot.ly";
        while ( !client.connect(server, 80) ){
          if(VERBOSE){ Serial.println("Couldn\'t connect to servers.... trying again!");}
          delay(1000);
        }          
      }
      if(VERBOSE) Serial.println("Connected to plotly's servers");
    }
    if(VERBOSE) Serial.println("\n== Sending HTTP Post to plotly ==");

    // HTTP Meta
    println_("POST /clientresp HTTP/1.1", 0);
    println_("Host: 107.21.214.199", 0);
    println_("User-Agent: Arduino/2.0", 0);

    // compute an upper bound on the post body size
    upper_ = 273+layout.length()+((N_*M_-1)*2)+((N_-1)*4)+(max(20,maxStringLength)*N_*M_);
      /* Computation composition: 
        44 // First part of querystring below
        + 30 // Upper limit on username length
        + 5  // "&key="
        + 10 // api key length
        + 6  // "&args="
        + 22 // "&kwargs={\"filename\": \""
        + 100 // upper bound on filename
        + 53 // "\", \"fileopt\": \"extend\", \"transpose\": true, \"layout\": "
        + layout.length()
        + 1 // closing }
        + (N_*M_-1)*2   // + 2-chars for comma and space for all but the last numbers ... 
        + (N_-1)*4  // + 4-chars for square brackets, comma, space for n-1 set of points [], 
        + 2         // + 2-chars for start 'n finish square braces
        + max(20,maxStringLength)*N_*M_;  // + max character buffer in converting floats to strings. NOTE: The largest float is 56 chars... should the buffer be this big? [(48-chars for largest float integer left of the decimal (-3.4028235E+38) )+(1 decimal pt)+(6 digits of precision right of the decimal)=56-chars]*each float
     */   

    if(timestamp){
      upper_ += 122;
      /* Computation composition: 
      upper_ += 26 // \"convertTimestamp\": true"
        + 41 // ", \"convertTimestamp\": true, \"timezone\": \""
        + 30 // upper bound on timezones string
        + 15 // "\", \"sentTime\": "
        + 10; // max length of unsigned long for sentTime: 4,294,967,295
      */
    }

    // send the header to plotly
    print_("Content-Length: ", 0);
    println_(String(upper_), 0);
    println_("", 0);

    // start the post string
    print_("version=0.2&origin=plot&platform=arduino&un=", 44);
    print_(username);
    print_("&key=", 5);
    print_(api_key);
    print_("&args=", 6);
}

void plotly::send_prepad_(){
  mi_ += 1;
  if(mi_ == 1){
      ni_ += 1;
      if(ni_==1){
          print_("[[",2);
      } else{
          print_("[",1);
      }
  }
}
void plotly::send_postpad_(){
  if(mi_ == 1){
      if(ni_==1){
          print_(", ", 2);
      } else{
          print_(", ", 2);
      }
  } else if(mi_ == M_ && ni_ < N_){
      print_("], ", 3);
      mi_ = 0;
  } else if(mi_ == M_ && ni_ == N_){
      print_("]]", 2);
  } else{
      print_(", ", 2);
  }
  if(mi_ == M_ && ni_ == N_){ 
      close_stream(); 
  }
}

void plotly::close_stream(){
    print_( "&kwargs={\"filename\": \"", 22 );
    print_( filename );
    print_( "\", \"fileopt\": \"extend\", \"transpose\": true, \"layout\": ", 53);    

    print_( layout, layout.length() );

    if(timestamp){
      print_( ", \"convertTimestamp\": true", 26 );
      print_( ", \"timezone\": \"", 15 );
      print_(timezone);
      print_("\"", 1);
      print_( ", \"sentTime\": ", 14 );
      String sT = String((int)millis());
      print_( sT );
      print_( "}", 1 );
    } else{
      print_( "}", 1);
    }

    // fill the remainder of the post with white space
    if(nChar_>=upper_){ 
      Serial.print("Error: Content-Length upper bound is too small. Upper bound was: ");
      Serial.print(upper_); Serial.print(", and we printed "); Serial.print(nChar_); Serial.println(" characters.");
      Serial.println("Message will not transmit");
    }
    for(int i=nChar_; i<upper_; i++){
        if(!DRY_RUN) client.print(" ");
    }
    // final newline to terminate the post
    println_("", 0);
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

void plotly::post(int x, int y){     sendString_(x); sendString_(y); }
void plotly::post(int x, float y){   sendString_(x); sendString_(y); }
void plotly::post(float x, int y){   sendString_(x); sendString_(y); }
void plotly::post(float x, float y){ sendString_(x); sendString_(y); }
void plotly::post(String x, int y){  sendString_(x); sendString_(y); }
void plotly::post(String x, float y){sendString_(x); sendString_(y); }

void plotly::sendString_(String s){
  send_prepad_();
  print_("\"",1); print_(s); print_("\"",1);
  send_postpad_();
}
void plotly::sendString_(float d){
    send_prepad_();
    dtostrf(d,width_,prec_,s_);
    print_(String(s_));  // TODO: anyway to print all 20 characters always? that way we can get rid of this String casting
    send_postpad_();
}
void plotly::sendString_(int d){
  send_prepad_();
  print_(String(d)); 
  send_postpad_();
}

void plotly::print_(char *s, int nChar){
  if(VERBOSE){ Serial.print(s); }
  if(!DRY_RUN) { client.print(s); }
  nChar_ += nChar;
}
void plotly::print_(String s, int nChar){
  if(VERBOSE){ Serial.print(s); }
  if(!DRY_RUN) { client.print(s); }
  nChar_ += nChar;
}
void plotly::print_(String s){
  if(VERBOSE){ Serial.print(s); }
  if(!DRY_RUN) { client.print(s); }
  nChar_ += s.length();
}
void plotly::println_(char *s, int nChar){
  if(VERBOSE){ Serial.println(s); }
  if(!DRY_RUN) { client.println(s); }
  nChar_ += nChar;
}
void plotly::println_(String s, int nChar){
  if(VERBOSE){ Serial.println(s); }
  if(!DRY_RUN) { client.println(s); }  
  nChar_ += nChar;
}
void plotly::println_(String s){
  if(VERBOSE){ Serial.println(s); }
  if(!DRY_RUN) { client.println(s); }
  nChar_ += s.length();
}
