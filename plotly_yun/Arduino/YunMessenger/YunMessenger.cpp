#include "YunMessenger.h"
#include <Bridge.h>
#include <Console.h>

#include "Arduino.h"

Messenger::Messenger(){    
}

static void Messenger::send(char *subscriber, char *message){
    Console.buffer(64);
    Console.print(char(29));
    Console.print(subscriber);
    Console.print(char(30));
    Console.print(message);
    Console.print(char(31));
    Console.flush();
}

static void Messenger::open(char *subscriber){
    Console.buffer(64);
    Console.print(char(29));
    Console.print(subscriber);
    Console.print(char(30));
}

static void Messenger::close(){
    Console.print(char(30));
    Console.flush();
}