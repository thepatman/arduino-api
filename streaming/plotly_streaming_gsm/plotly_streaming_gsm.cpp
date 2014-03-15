#include "Arduino.h"
#include <GSM.h>
#include "plotly_streaming_gsm.h"

#include <avr/dtostrf.h>
#include <avr/pgmspace.h>


plotly::plotly(char *username, char *api_key, char *stream_token, char *filename)
  {
    floatWidth_ = 10;
    floatPrec_ = 5;
    LOG_LEVEL = 0;  // 0 = Debugging, 1 = Informational, 2 = Status, 3 = Errors, 4 = Quiet (Serial Off)
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
    if(DRY_RUN && LOG_LEVEL < 3){ Serial.println(F("... This is a dry run, we are not connecting to plotly's servers...")); }
    else{
      if(LOG_LEVEL < 3) { Serial.println(F("... Attempting to connect to plotly's REST servers...")); }
      while ( !client.connect("plot.ly", 80) ) {
        if(LOG_LEVEL < 4){ Serial.println(F("... Couldn\'t connect to plotly's REST servers... trying again!")); }
        delay(1000);
      }
    }
    if(LOG_LEVEL < 3) Serial.println(F("... Connected to plotly's REST servers"));
    if(LOG_LEVEL < 3) Serial.println(F("... Sending HTTP Post to plotly"));
    print_(F("POST /clientresp HTTP/1.1\r\n"));
    print_(F("Host: 107.21.214.199\r\n"));
    print_(F("User-Agent: Arduino/0.5.1\r\n"));

    print_(F("Content-Length: "));
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
    print_(F("\r\n\r\n"));

    print_(F("version=0.2&origin=plot&platform=arduino&un="));
    print_(username_);
    print_(F("&key="));
    print_(api_key_);
    print_(F("&args=[{\"y\": [], \"x\": [], \"type\": \"scatter\", \"stream\": {\"token\": \""));
    print_(stream_token_);
    print_(F("\", \"maxpoints\": "));
    print_(maxpoints);
    print_(F("}}]&kwargs={\"fileopt\": \"overwrite\", \"filename\": \""));
    print_(filename_);
    print_(F("\"}"));
    // final newline to terminate the post
    print_(F("\r\n"));

    /*
     * Wait for a response
     * Parse the response for the "All Streams Go!" and proceed to streaming
     * if we find it
    */
    char allStreamsGo[] = "All Streams Go!";
    int asgCnt = 0; // asg stands for All Streams Go
    char url[] = "\"url\": \"http://107.21.214.199/~";
    char fid[4];
    int fidCnt = 0;
    int urlCnt = 0;
    int usernameCnt = 0;
    int urlLower = 0;
    int urlUpper = 0;
    bool proceed = false;
    bool fidMatched = false;

    if(LOG_LEVEL < 3) Serial.println(F("... Sent message, plotly's response:"));
    if(!DRY_RUN){
        while(client.connected()){
            if(client.available()){
                char c = client.read();
                if(LOG_LEVEL < 2) Serial.print(c);

                /*
                 * Attempt to read the "All streams go" msg if it exists
                 * by comparing characters as they roll in
                */
                if(asgCnt == len_(allStreamsGo) && !proceed){
                    proceed = true;
                }
                else if(allStreamsGo[asgCnt]==c){
                    asgCnt += 1;
                } else if(asgCnt > 0){
                    // reset counter
                    asgCnt = 0;
                }

                /*
                 * Extract the last bit of the URL from the response
                 * The url is in the form http://107.21.214.199/~USERNAME/FID
                 * We'll character-count up through char url[] and through username_, then start 
                 * filling in characters into fid
                */
                if(LOG_LEVEL < 3){
                    if(url[urlCnt]==c && urlCnt < len_(url)){
                        urlCnt += 1;
                    } else if(urlCnt > 0 && urlCnt < len_(url)){
                        // Reset counter
                        urlCnt = 0;
                    }
                    if(urlCnt == len_(url) && fidCnt < 4 && !fidMatched){
                        // We've counted through the url, start counting through the username
                        if(usernameCnt < len_(username_)+2){
                            usernameCnt += 1;
                        } else {
                            // the url ends with "
                            if(c != '"'){
                                fid[fidCnt] = c;
                                fidCnt += 1;
                            } else if(fidCnt>0){
                                fidMatched = true;
                            }
                            
                        }
                    }
                }
            }
        }
        client.stop();
    }    

    if(!proceed && LOG_LEVEL < 4){ 
        Serial.print(F("... Error initializing stream, aborting. Try again or get in touch with Chris at chris@plot.ly"));
        return;
    }

    if(LOG_LEVEL < 3){
        Serial.println(F("... A-ok from plotly, All Streams Go!"));
        if(fidMatched){
            Serial.print(F("... View your streaming plot here: https://plot.ly/~"));
            Serial.print(username_);
            Serial.print(F("/"));
            for(int i=0; i<fidCnt; i++){
                Serial.print(fid[i]);
            }
            Serial.println(F(""));
        }
    }


    /*
     * Start request to stream servers
    */    
    if(LOG_LEVEL < 3) Serial.println(F("... Connecting to plotly's streaming servers..."));
    char server[] = "stream.plot.ly";
    int port = 80;
    while ( !client.connect(server, port) ) {
        if(LOG_LEVEL < 4) Serial.println(F("... Couldn\'t connect to servers.... trying again!"));
        delay(1000);
    }
    if(LOG_LEVEL < 3) Serial.println(F("... Connected to plotly's streaming servers\n... Initializing stream"));

    print_(F("POST / HTTP/1.1\r\n"));
    print_(F("Host: 127.0.0.1\r\n"));
    print_(F("User-Agent: Python\r\n"));
    print_(F("Transfer-Encoding: chunked\r\n"));
    print_(F("Connection: close\r\n"));
    print_(F("plotly-streamtoken: "));
    print_(stream_token_);
    print_(F("\r\nplotly-convertTimestamp: America/Montreal"));
    print_(F("\r\n\r\n"));

    if(LOG_LEVEL < 3) Serial.println(F("... Done initializing, ready to stream!"));

}

void plotly::stop(){
    print_(F("0\r\n\r\n"));
    client.stop();
}

void plotly::jsonStart(int i){
    if(LOG_LEVEL<2) Serial.print(i+15, HEX);    // 15 char for the json that wraps the data: {"x": , "y": }\n
    if(!DRY_RUN) client.print(i+15, HEX);
    print_(F("\r\n{\"x\": "));
}
void plotly::jsonMiddle(){
    print_(F(", \"y\": "));
}
void plotly::jsonEnd(){
    print_(F("}\n\r\n"));
}

int plotly::len_(int i){
    // int range: -32,768 to 32,767
    if(i > 9999) return 5;
    else if(i > 999) return 4;
    else if(i > 99) return 3;
    else if(i > 9) return 2;
    else if(i > -1) return 1;
    else if(i > -10) return 2;
    else if(i > -100) return 3;
    else if(i > -1000) return 4;
    else if(i > -10000) return 5;
    else return 6;
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
    if(LOG_LEVEL < 2) Serial.print(d);
    if(!DRY_RUN) client.print(d);
}
void plotly::print_(float d){
    char s_[floatWidth_];
    dtostrf(d,floatWidth_,floatPrec_,s_);
    print_(s_);
}
void plotly::print_(String d){
    if(LOG_LEVEL < 2) Serial.print(d);
    if(!DRY_RUN) client.print(d);
}
void plotly::print_(unsigned long d){
    if(LOG_LEVEL < 2) Serial.print(d);
    if(!DRY_RUN) client.print(d);
}
void plotly::print_(char *d){
    if(LOG_LEVEL < 2) Serial.print(d);
    if(!DRY_RUN) client.print(d);
}
void plotly::print_(const __FlashStringHelper* d){
    if(LOG_LEVEL < 2) Serial.print(d);
    if(!DRY_RUN) client.print(d);
}