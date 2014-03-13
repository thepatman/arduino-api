#include "PlotlyYun.h"
#include "YunMessenger.h"

Plotly::Plotly(char *stream_token){
    stream_token_ = stream_token;
}

Plotly::plot(int x, int y){
    /*
     * Send a plotly-formatted JSON object
     * to the plotly subscriber on the Linino
     *
     * Example object:
     * {"x": 1, "y": 10}
    */
    Messenger.open("plotly");
    Console.print("{\"x\": ");
    Console.print(x);
    Console.print(", \"y\": ");
    Console.print(y);
    Console.print("}, \"token\": \"");
    Console.print(self.stream_token_);
    Console.print("\"}");
    Messenger.close();
}