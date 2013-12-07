About
--------
The Plotly-Arduino library provides Arduino users with an interface to their Plotly account. Through this interface, an internet connected Arduino (with either WiFi or Ethernet) can send data to Plotly servers and a graph of the data can be viewed inside their Plotly account inside their browser.


Contents
------------
The directory of this folder looks like:
plotly/
	README
	examples/
		plotly_wifi_example.ino
        plotly_ethernet_example.ino
	plotly_wifi/
		plotly_wifi.cpp
		plotly_wifi.h
    plotly_ethernet/
        plotly_ethernet.cpp
        plotly_ethernet.h

To use, place the plotly_wifi or the plotly_ethernet folder (with the .cpp and .h files) into your libraries folder. See https://plot.ly/api/arduino for more details.

Documentation
--------------------
See https://plot.ly/api/arduino for the docs and examples.

Feedback
-------------
This project is in active development. If you have any thoughts, comments, suggestions, or need any help getting started please drop me a line! You can reach me at chris@plot.ly.