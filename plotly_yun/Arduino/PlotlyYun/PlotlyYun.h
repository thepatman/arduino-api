#ifndef PLOTLYMESSENGER_H
#define PLOTLYMESSENGER_H

#include <YunMessenger.h>

class Plotly {
    public:
        Plotly(char *stream_token);
        void plot(int x, int y);

    private:
        char *stream_token_;
};

#endif