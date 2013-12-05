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
        void open_stream(int N, int M, char *filename, char *username, char *api_key);
		void post(int y);
		void post(int x, int y);
		void post(int x, float y);
		void post(float x, int y);
		void post(float x, float y);
		void post(String x, int y);
		void post(String x, float y);
		void close_stream();
		char *layout;
		int maxStringLength;
		bool DEBUG;
		bool VERBOSE;
		bool DRY_RUN;
	private:
		void sendString_(int d);
		void sendString_(float d);
		void sendString_(String d);
		void send_(String s);
		//char *dtostrf(double val, signed char width, unsigned char prec, char *sout);

		// void println_(String s);
		// void print_(String s);
		int M_;	 	// number of traces * 2
		int mi_; 	// counter of M_
		int N_; 	// number of series points
		int ni_; 	// counter of N_
		int nChar_;
		int upper_; 
		unsigned char prec_;
		signed char width_;
		char s_[20];
};

#endif
