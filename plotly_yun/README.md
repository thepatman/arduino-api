[![Mock Signal Data](https://raw.github.com/plotly/Streaming-Demos/master/readme_gifs/real-timesensor.gif)](http://plot.ly/~streaming-demos/6)

# Status
Basic functionality, through the setup below, works! I'm just cleaning up the interface, setting up some better error handling, setting it up as a reliable background process.


# Setup

#### Connect your Yún to wifi
You know the drill:

1. Reset the WiFi chip: hold down the WLAN button for like 10 seconds (probably multiple times) until Arduin Yun-XXXX comes up as a network
2. Connect to that network, enter `arduino.local` into your browser
3. Configure the network, making sure that:
 - Your encryption (WPA, WPA2, WEP) is correct
 - Your *case-sensitive* network name is correct
4. Connect to your old network, go to `arduino.local` in your browser. If your browser can't find it, go back to step 1!

#### Update the `config.json` file with your plotly credentials
1. Download this folder
2. Sign up to plotly here (it's free!): [https://plot.ly/ssu](https://plot.ly/ssu)
3. Find your API key and stream tokens here: [https://plot.ly/settings](https://plot.ly/settings)
4. Open up `config.json` from this folder and add your username, API key, and stream token to the file. It'll look something like:
 ```
 {
    "plotly_username": "anna.lyst",
    "plotly_api_key": "ab4kftunvd",
    "plotly_streaming_tokens": ["ab4kf5nfdn"]
 }
 ```

 (those are fake keys and tokens, so don't try to use them, they won't work!)

#### Copy a ton of files over to the Linino
1. Open up your computer's terminal and move to where you downloaded this folder:
 ```bash
 $ cd ~/Downloads/plotly_yun
 $ ls
 Arduino			plotly			run.py
 README.md		requests		run2.ino
 YunMessenger		run.ino			run_background.py
 ```

2. Copy all of these files onto your arduino
 ```bash
 $ scp -r * root@arduino.local:/root/
 ```
 a TON of stuff will print out, don't worry!

#### Cruise into the Linino, install some stuff, start listening for the Arduino
1. SSH into your Linino:
 ```bash
 $ ssh root@arduino.local
 ```

2. Install `python-openssl` so that you can chat to plotly securely:
 ```bash
 root@Arduino:~# opkg update
 root@Arduino:~# opkg install python-openssl
 ```
 with outputs, it'll look like:
 ```bash
 root@Arduino:~# opkg update
 Downloading http://download.linino.org/dogstick/all-in-one/latest/packages//Packages.gz.
 Updated list of available packages in /var/opkg-lists/barrier_breaker.
 Downloading http://download.linino.org/dogstick/all-in-one/latest/packages//Packages.sig.
 Signature check passed.
 root@Arduino:~# opkg install python-openssl
 Installing python-openssl (2.7.3-2) to root...
 Downloading http://download.linino.org/dogstick/all-in-one/latest/packages//python-openssl_2.7.3-2_ar71xx.ipk.
 Configuring python-openssl.
 ```

3. Run the python program `run.py` and wait
 ```bash
 root@Arduino:~# python run.py
 ```

#### Open up the [latest version of the Arduino IDE](http://arduino.cc/en/main/software)
1. Load up run.ino from this folder
2. Make sure that your IDE has selected `Arduino Yun` and the remote port:
![Arduino IDE Port Selection](http://new.tinygrab.com/c751bc2ee2ee7fe227a406dbdb8a3d6dfb53b00b29.png)
3. Upload the program!
4. Check out the Linino terminal that you ssh'd into for status updates and error messages
5. Watch your plotly graph update in real-time!

# How does it work?

The Arduino microcontroller (ATmega32u4) sends data to the Linino over the `bridge` with the [YunMessenger](https://github.com/plotly/YunMessenger) library. The `bridge` is just a TCP connection over port `6571` on `localhost` and the `YunMessenger` library formalizes communication over that socket and allows multiple subscribers to listen to messages coming from the Arduino (the 32u4).

On the Arduino side, the communication over this socket looks like:
```C
#include <Bridge.h>
#include <Console.h>

Bridge.begin();
Console.begin();
while (!Console) {
  ; // wait for Console port to connect.
}

Console.print(char(29)); // indicate the start of the subscriber name
Console.print("plotly"); // the name of the subscriber
Console.print(char(30)); // indicates the start of the message
Console.print("{\"x\": 1, \"y\": 10}"); // the message (in this case a plotly formatted JSON object)
Console.print(char(31)); // indicates the end of the message
```

One of these subscribers is a socket to plotly's streaming servers, at `stream.plot.ly`. On the Linino side, the YunMessenger abstracts handling this communication to something like:

```python
from YunMessenger import Console
import plotly
import json

# Send messages to plotly's streaming servers
def handlePlotlyMessages(msg):
    print msg
    s = plotly.stream(stream_token)
    s.write(json.loads(msg))

# Initialize a YunMessenger console object
c = Console.Console()

# Assign a "subscriber" to `plotly` messages
c.onMessage['plotly'] = handlePlotlyMessages

# Connect to the socket on `localhost:6571` 
# Parse the data the comes over TCP for characters 29, 30, and 31
# and distribute "messages" (the data between characters 30 and 31)
# to the associated "subscriber" (the data between characters 29 and 30).

c.run()

```

# Contact!
- <chris@plot.ly>
- [@plotlygraphs](https://twitter.com/plotlygraphs)


# Troubleshooting
- Connecting the yun to WiFi has no error handling. It can fail silently while attempting to connect to a network, so double check your encryption and your network name. Also, network names are case-senstive and sometimes the Arduino will default to connecting to the lower-case version of your network.
- If you've connected multiple arduino yun's, you won't be able to connect with `arduino.local`. Instead, connect with each device's IP address
- Use the latest Arduino IDE (1.5.6 -r2)
- Reseting the network ("hold down the wifi button for > 5 seconds") often requires several tries!
- To verify that the python client on the Linino is ready to recieve commands, run `$ telnet localhost 6571`
- Don't forget:
```
while (!Console) {
  ; // wait for Console port to connect.
}
```
