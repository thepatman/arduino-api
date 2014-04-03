## Real-time Graphing and Data Logging

#### The easiest and fastest way to plot and share data on the Arduino. 

<p align="center">
<a href="http://vimeo.com/89003132">
<img src="http://new.tinygrab.com/c751bc2ee24505ea0425b28e9c8f2d1f8aa81ad395.png" />
</a>
</p>

Plotly's Arduino libraries connects Arduinos to plotly's beautiful online graphing tool for streaming, real-time, interactive data logging and graphing. It's free, open source, and your graphs and data are entirely online.

Here is an example of a real-time graph: [http://plot.ly/~streaming-demos/6/](http://plot.ly/~streaming-demos/6/)

## Super easy!

```C
#include <WiFi.h>
#include <plotly_wifi_streaming.h>

#define num_traces 2
// Sign up to plotly here: https://plot.ly
// View your API key and stream tokens in your settings: https://plot.ly/settings
char *streaming_tokens[num_traces] = {"your_plotly_stream_token", "another_plotly_stream_token"};
plotly graph("your_plotly_username", "your_plotly_api_key", streaming_tokens, "your_plotly_filename", num_traces);

void setup() {

  Serial.begin(9600);

  wifi_connect();

  // Initialize a streaming graph in your plotly account
  graph.init();
  // Initialize plotly's streaming service
  graph.openStream(); 
}

void loop() {
  // now let's stream data to plotly, giddyup!
  graph.plot(millis(), analogRead(A0), tokens[0]);
  graph.plot(millis(), analogRead(A1), tokens[1]);
}
```

## In the wild

#### A video of our real-time heart rate monitor (click to view)
<p align="center">
<a href="https://vine.co/v/Mq2LQexrbl7">
<img src="http://new.tinygrab.com/c751bc2ee2533bf46bba1b0b65720764edcfb06c6b.png" />
</a>
</p>

#### A video of an Arduino streaming-data from a mountain edge, in Peachland, BC
<p align="center">
<a href="http://vimeo.com/87362390">
<img src="http://new.tinygrab.com/c751bc2ee28fbde72ce2f6b8904f1efd034210827d.png" />
</a>
</p>


## 30 Second Quickstart
(Working on the Yún? Click on the `arduino_yún` folder for a separate instructions)

1. Sign up to plotly (it's free!): [https://plot.ly](https://plot.ly).
2. Download and uncompress the latest plotly release: [https://github.com/plotly/arduino-api/releases](https://github.com/plotly/arduino-api/releases).
3. Place the appropriate library your Arduino libraries folder. On a Mac, this is in `~/Documents/Arduino/libraries/`:
    ![](http://new.tinygrab.com/c751bc2ee29f2d309e4fd8985685df0a1d83cf115a.png)
4. Open up the Arduino IDE. If your using WiFi and haven't upgraded your firmware, use the [IDE version 1.0.3](http://arduino.cc/en/main/software).
5. Load up one of the examples from this repository. Fill in your plotly username, API key, stream tokens, and filename. You can find your API key and stream tokens here: [https://plot.ly/settings](https://plot.ly/settings). It'll look something like:
    
    ```cpp
    char *tokens[] = {"ab4kf5nfdn","kdf5bn4dbn"};
    plotly graph("anna.lyst","ab4kftunvd", tokens, "arduino graph");
    ```
    (those are fake keys and tokens, so don't try to use them, they won't work!)

6. Upload the program!
7. Open up your Serial Monitor. You'll see an output like:

    ```
    ... Attempting to connect to WPA network...
    ... Connected to network
    ... Attempting to connect to plotly's REST servers
    ... Connected to plotly's REST servers
    ... Sending HTTP Post to plotly
    ... Sent message, plotly's response:
    ... A-ok from plotly, All Streams Go!
    ... View your streaming plot here: https://plot.ly/~streaming-demos/6
    ... Connecting to plotly's streaming servers...
    ... Connected to plotly's streaming servers
    ... Initializing stream
    ... Done initializing, ready to stream!
    ```
8. Grab the URL that was printed out, view your graph in your browser, and celebrate! The graph and data is saved in your plotly account, so you can view it in your plotly file list here: [https://plot.ly/plot](https://plot.ly/plot). You can view, edit, and share your graphs while data is streaming to them in real-time! Everybody that views the graph will see the exact same data at the same time (try it out yourself: open your graph in two different browser windows).

## More on Usage

### Usage, Your Data Rights, and Private Graphs
When you make a graph on plotly, you retain the rights to your content (see our terms [here](https://plot.ly/tou). You also control whether your graphs are public or private. Public plotting is free; for a lot of private use, you can get a Premium or Organizational plan (see http://plot.ly/plans). It's just like GitHub.

By default, anyone can view the graphs at the unique URL. To make the graphs private, so that only you can see them when your logged in, set `world_readable` to `false`:

```Cpp
  plotly graph("your_plotly_username", "your_plotly_api_key", streaming_tokens, "your_plotly_filename", num_traces);
  graph.world_readable = false;
```

### Time stamping
By default, plotly assumes that `x` is `millis()` and automatically converts the `x` to a real-time timestamp with the timezone `"America/Montreal"` on the server. To disable this, set `convertTimestamp` to `false`, e.g.

```Cpp
  plotly graph("your_plotly_username", "your_plotly_api_key", streaming_tokens, "your_plotly_filename", num_traces);
  graph.autoTimestamp = false;
```

To change the timezone, set `timeZone` to one of the strings in here: [Accepted Timezone Strings.txt](https://github.com/plotly/arduino-api/blob/master/Accepted%20Timezone%20Strings.txt), e.g.

```Cpp
  plotly graph("your_plotly_username", "your_plotly_api_key", streaming_tokens, "your_plotly_filename", num_traces);
  graph.timezone = "Africa/Abidjan";
```

### Editing thelive  graph
Plotly graphs can be edited while data is streaming to them. Every aspect of the graph is configurable, ou can add a second y-axis, turn the graphs into subplots, change the colors, update the title, change the chart type, ... etc! To get started, just open up the graph in your list of files here: [https://plot.ly/plot](https://plot.ly/plot).

### Multiple Viewers
Everybody who looks at your streaming graph sees the exact same data, at the exact same time! Give it a try yourself: open up a graph in two different browser windows.

### Docs

```Cpp
class plotly(char *username, char *api_key, char* stream_tokens[], char *filename, int nTraces);
```

**Public Member Functions**

- `bool plotly.init()`
  
  Creates an empty graph in your plotly account that will get streamed to. This is done by making an API call to plotly's REST service. Returns `true` if initialization was successful, `false` otherwise.
- `void plotly.openStream()`
  
  Opens a TCP connection to plotly's streaming service. The stream is uniquely identified by the `stream_tokens`.
- `void plotly.closeStream()`
  
  Closes the TCP connection to plotly's streaming service.
- `void plotly.reconnectStream()`
  
  Reopens the connection to plotly's streaming service if not connected.
- `void plot(unsigned long x, int y, char *token)`
  
  Plots `(x, y)` to the streaming graph.
- `void plot(unsigned long x, float y, char *token)`
  
  Plots `(x, y)` to the streaming graph.
**Public Member Parameters**
- `int plotly.log_level` (Default `2`)
  
  Determines which messages are printed over serial. Levels are:
  - `0`: Debugging
  - `1`: Informational
  - `2`: Status
  - `3`: Errors
  - `4`: Quiet
- `bool plotly.dry_run`
  
  If `True`, then no calls are made to Plotly's servers.
- `int plotly.maxpoints` (Default `30`)
  
  Determines the number of points to plot at a time. Valid from `1` to `200000`. 
- `bool plotly.convertTimestamp` (Default `true`)
  
  If `true`, the Plotly assumes that `x` is milliseconds since program start (`millis()`) and automatically converts these values into a timestamp.
- `char *plotly.timeZone` (Default: `"America/Montreal"`)

  The timezone to convert the timestamps if `plotly.convertTimestamp=true`. A list of the accepted timezones are in this repo: [Accepted Timezone Strings.txt](https://github.com/plotly/arduino-api/blob/master/Accepted%20Timezone%20Strings.txt)

- `bool world_readable` (Default: true)

  If `true`, then your graph is publicly viewable and discoverable by unique url. If `false`, then only you can view the graph.
  
### Contributing Notes
The `wifi`, `ethernet`, `gsm`, and `cc3000` libraries and examples are 95% identical, and so are automatically generated from template files in the `plotly_dev` folder with one of Python's templating libraries, Mako. 

### Contact
- [@plotlygraphs](https://twitter.com/plotlygraphs)
- <chris@plot.ly>
