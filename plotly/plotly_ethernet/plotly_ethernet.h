#ifndef plotly_ethernet_h
#define plotly_ethernet_h

#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>


class plotly
{
	public:
		plotly();
                EthernetClient client;
		void open_stream(int N, String filename, String username, String api_key);

		void post(int data);

		void close_stream(int data);

		bool VERBOSE;
	private:
		int _N; 	// number of data points in a post
		int _nChar; // incrementing character count of the post body
		int _upper; // upper bound on character count of the post body
		int _ni; 	// number of data elements
		String s;
		String header;
		int i;
};

#endif
