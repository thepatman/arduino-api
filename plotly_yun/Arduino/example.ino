#include <PlotlyYun.h>
#include <YunMessenger.h>
#include <Bridge.h>
#include <Console.h>

// Initialize plotly "plotters" with your stream tokens.
// Find your stream tokens in your plotly account here: https://plot.ly/settings .
// Initialize as many plotters as you want! Each plotter will send
// data to the same plot as a separate line.
// Make sure that you place these exact stream tokens in your `config.json` file
// on the Linino.

//plotly plotter1("put_your_10_character_stream_token_here");
//plotly plotter2("put_another_token_here");
//plotly plotter3("and_another!");

plotly plotter1("3ab1qn3z23");
plotly plotter2("9x4gkihdlx");
plotly plotter3("xi83ke94lw");


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

    plotter1.timezone = "America/Montreal";
    plotter2.timezone = "America/Montreal";
    plotter3.timezone = "America/Montreal";

} 

void loop() { 
    // Graph data! Place the "y" value that you want to plot
    // the "x" value will automatically be a time-stamped value
    // Each "plotter" will send data to the same plot as separate lines
    plotter1.plot( analogRead(A0) );
    plotter2.plot( analogRead(A1) );
    plotter3.plot( analogRead(A2) );

    // You can also set the "x" value manually 
    // by placing a pair of points in the plot function:
//    plotter1.plot( millis(), analogRead(A0) );
//    plotter2.plot( millis(), analogRead(A1) );
//    plotter3.plot( millis(), analogRead(A2) );    
    delay(100);
}
