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
} 


void loop() { 

    // Write to the plotly_0 subscriber, analogRead(A0)
    Console.print(char(29));
    Console.print("plotly_0");
    Console.print(char(30));
    Console.print("{\"y\": ");
    Console.print(analogRead(A0));
    Console.print("}");
    Console.print(char(31));
    Console.flush();

    // Write to the plotly_1 subscriber, analogRead(A1)
    Console.print(char(29));
    Console.print("plotly_1");
    Console.print(char(30));
    Console.print("{\"y\": ");
    Console.print(analogRead(A1));
    Console.print("}");
    Console.print(char(31));
    Console.flush();
    delay(100);    
    

}
