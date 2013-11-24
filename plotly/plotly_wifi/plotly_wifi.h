#ifndef plotly_wifi_h
#define plotly_wifi_h

#include "Arduino.h"
#include <WiFi.h>


class plotly
{
	public:
		plotly();
                WiFiClient client;
		void open_stream(int N, String filename, String username, String api_key);

		void post(int data);

		void close_stream(int data);

		bool VERBOSE;
	private:
		int N_; 	// number of data points in a post
		int nChar_; // incrementing character count of the post body
		int upper_; // upper bound on character count of the post body
		int ni_; 	// number of data elements
		String s;
		String header;
		int i;
};

#endif
