#ifndef plotly_wifi_streaming_h
#define plotly_wifi_streaming_h

#include "Arduino.h"

#include <WiFi.h>

class plotly
{
    public:
        plotly(char *username, char *api_key, char *stream_token, char *filename);
        WiFiClient client;
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
        bool VERBOSE;
        bool DRY_RUN;


    private:
        unsigned long upper_; // upper bound on length of querystring, set to the messages content-length
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
