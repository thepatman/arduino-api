#include <Bridge.h>
#include <Console.h>
// #include <YunMessenger.h>

void setup() { 


    // start-up the bridge
    Bridge.begin();

    delay(2000);
    Console.begin();
    while (!Console) {
      ; // wait for Console port to connect.
    }
    
    Console.buffer(64);
    delay(2000);    
    Console.print(char(29));
    Console.print("plotly");
    Console.print(char(30));
    Console.print("{\"x\": 5, \"y\": 5}");
    Console.print(char(31));
    Console.flush();

    /*
    YunMessenger ym;
    ym.send("plotly", "{\"x\": 5, \"y\": 5}");
    */
} 


void loop() { 
}