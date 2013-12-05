#ifndef plotly_wifi_h
#define plotly_wifi_h

#include "Arduino.h"
#include <WiFi.h>


class plotly
{
	public:
		plotly();
                WiFiClient client;
		void open_stream(int N, int M, String filename, String username, String api_key);
		void post(int y);
		void post(int x, int y);
		void post(int x, float y);
		void post(float x, int y);
		void post(float x, float y);
		void post(String x, int y);
		void post(String x, float y);
		void close_stream(int data);
		bool VERBOSE;
	private:
		void send_(String s)
		int M_;	 	// number of traces * 2
		int mi_; 	// counter of M_
		int N_; 	// number of series points
		int ni_; 	// counter of N_
		int nChar_; // incrementing character count of the post body
		int upper_; // upper bound on character count of the post body

		String s;
		String header;
		int i;
};

#endif
