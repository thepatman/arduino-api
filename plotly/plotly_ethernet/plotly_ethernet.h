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
        void open_stream(int N, int M, String filename, String username, String api_key);
		void post(int x, int y);
		void post(int x, float y);
		void post(float x, int y);
		void post(float x, float y);
		void post(String x, int y);
		void post(String x, float y);
		void close_stream();
		int maxStringLength;
		bool DEBUG;
		bool VERBOSE;
		bool DRY_RUN;
		bool timestamp;
		String timezone;
		String layout;
		String filename;
	private:
		void sendString_(int d);
		void sendString_(float d);
		void sendString_(String d);
		void send_prepad_();
		void send_postpad_();
		int M_;	 	// number of traces * 2
		int mi_; 	// counter of M_
		int N_; 	// number of series points
		int ni_; 	// counter of N_
		int nChar_;
		int upper_;
		unsigned char prec_;
		signed char width_;
		char s_[20];
		void print_(char *s, int nChar);
		void print_(String s, int nChar);
		void print_(String s);
		void println_(char *s, int nChar);
		void println_(String s, int nChar);
		void println_(String s);
};
#endif