#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#include "plotly_ethernet.h"

plotly::plotly(){
	VERBOSE = true;
}

void plotly::open_stream(int N, String filename, String username, String api_key){
	ni_ = 0; // number of integers transmitted
    N_ = N;  // number of integers in a burst
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
    client.println("User-Agent: Arduino/1.0");

    // start the post string    
    header = "version=0.2&origin=plot&platform=arduino&un=";
    header += username;
    header += "&key=";
    header += api_key;
    header += "&kwargs={\"filename\": \"";
    header += filename;
    header += "\", \"fileopt\": \"extend\"}&args=[{\"y\": [" ;

    // compute an upper bound on the post body size
   	upper_ = header.length() + N*2 + N*6 + 3 - 2; // header length + 2-chars for comma and space for each number  + 6 chars for the largest int (-32767) + 3 for termination chars: ]}] - 2 since the last number doesnt have a comma
	
	s = "Content-Length: "; 
	s += upper_;
    client.println(s);

    // send the header to plotly
    client.println();
    client.print(header);
    nChar_ += header.length();
}

void plotly::post(int data){
    ni_ += 1;				// number of data elements
    if(ni_ == N_){			// i.e. the last number
    	close_stream(data);
    }
    else{
		s = "";
	    s = s + data + ", ";
	    nChar_ += s.length();	// number of chars
	    client.print(s);
    }
    return;
}


void plotly::close_stream(int data){
	s = "";
	s = s + data + "]}]";
	client.print(s);

	nChar_ += s.length();

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
