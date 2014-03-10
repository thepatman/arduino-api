### In progress!


### Setup

scp this folder over
```
$ scp -r * root@10.0.1.190:/root/
```

ssh in
```bash
$ ssh root@10.0.1.190
```
SSL
```bash
$ opkg update
$ opkg install python-openssl
```
with outputs:
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

### How does it work?

The Arduino microcontroller (ATmega32u4) sends data to the Linino over the `bridge` with the [YunMessenger](/plotly/YunMessenger) library. The `bridge` is just a TCP connection over port `6571` on `localhost` and the `YunMessenger` library abstracts communication over that socket to allow multiple subscribers to tune into the Arduino. 

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
Console.print('plotly'); // the name of the subscriber
Console.print(char(30)); // indicates the start of the message
Console.print("{\"x\": 1, \"y\": 10}"); // the message (in this case a plotly formatted JSON object)
Console.print(char(31)); // indicates the end of the message
```

On the Linino, it's abstracted to:

One of these subscribers is a socket to plotly's streaming servers, at `stream.plot.ly`:

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



### Troubleshooting
- Connecting the yun to WiFi has no error handling. It can fail silently while attempting to connect to a network, so double check your encryption and your network name. Also, network names are case-senstive and sometimes the Arduino will default to connecting to the lower-case version of your network.
- If you've connected multiple arduino yun's, you won't be able to connect with `arduino.local`. Instead, connect with the devices IP address
- Use the latest Arduino IDE (1.5.6 -r2)
- Reseting the network ("hold down the wifi button for > 5 seconds") often requires several tries!
- To verify that the python client on the Linino is ready to recieve commands, run `$ telnet localhost 6571`
