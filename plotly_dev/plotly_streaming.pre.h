% if lib=="ethernet":
#ifndef plotly_streaming_ethernet_h
#define plotly_streaming_ethernet_h
% elif lib=="wifi":
#ifndef plotly_streaming_wifi_h
#define plotly_streaming_wifi_h
% elif lib=="cc3000":
#ifndef plotly_streaming_cc3000_h
#define plotly_streaming_cc3000_h
% elif lib=="gsm":
#ifndef plotly_streaming_GSM_h
#define plotly_streaming_GSM_h
% endif

#include "Arduino.h"

% if lib=="ethernet":
#include <SPI.h>
#include <Ethernet.h>
% elif lib=="wifi":
#include <WiFi.h>
% elif lib=="cc3000":
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
% elif lib=="gsm":
#include <GSM.h>
% endif

class plotly
{
    public:
        plotly(char *username, char *api_key, char *stream_token, char *filename);
        % if lib=="ethernet":
        EthernetClient client;
        % elif lib=="wifi":
        WiFiClient client;
        % elif lib=="cc3000":
        Adafruit_CC3000 cc3000;
        Adafruit_CC3000_Client client;
        % elif lib=="gsm":
        GSMClient client;
        % endif
        void begin(unsigned long maxpoints);
        void stop();
        void jsonStart(int i);
        void jsonMiddle();
        void jsonEnd();
        void plot(int x, int y);
        void plot(int x, float y);
        void plot(unsigned long x, int y);
        void plot(unsigned long x, float y);
        void plot(float x, int y);
        void plot(float x, float y);
        void plot(char *x, int y);
        void plot(char *x, float y);
        void plot(String x, int y);
        void plot(String x, float y);
        int LOG_LEVEL;
        bool DRY_RUN;


    private:
        unsigned char floatPrec_;
        signed char floatWidth_;

        char *username_;
        char *api_key_;
        char *stream_token_;
        char *filename_;

        void print_(int d);
        void print_(float d);
        void print_(String d);
        void print_(unsigned long d);
        void print_(char *d);

        int len_(int i);
        int len_(unsigned long i);
        int len_(float i);
        int len_(char *i);
        int len_(String i);

};
#endif
