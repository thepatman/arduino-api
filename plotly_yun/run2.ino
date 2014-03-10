#include <Bridge.h>
#include <Console.h>
// #include <YunMessenger.h>

void setup() { 

    // start the serial port
    Serial.begin(57600);

    // for debugging, wait until a serial console is connected
    delay(4000);
    while (!Serial) { ; }

    // start-up the bridge
    Bridge.begin();

    delay(2000);
    Console.begin();
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