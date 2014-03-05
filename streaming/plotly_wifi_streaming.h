#ifndef plotly_wifi_streaming_h
#define plotly_wifi_streaming_h

#include "Arduino.h"

#include <WiFi.h>

class plotly
{
    public:
        plotly();
        WiFiClient client;
        void begin(char *stream_token);
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
