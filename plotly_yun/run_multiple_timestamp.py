from YunMessenger import Console
import plotly
import json
import datetime

# Load up plotly credentials
with open('./config.json') as config_file:
    plotly_user_config = json.load(config_file)

username = plotly_user_config['plotly_username'] 
api_key = plotly_user_config['plotly_api_key']
stream_token0 = plotly_user_config['plotly_streaming_tokens'][0]
stream_token1 = plotly_user_config['plotly_streaming_tokens'][1]


# Make the initializing plotly request
p = plotly.plotly(username, api_key)
p.ioff()
print p.plot([{'x': [], 'y': [], 'stream': {'token': stream_token0}},
              {'x': [], 'y': [], 'stream': {'token': stream_token1}}], 
              filename="stream multiple", fileopt="overwrite")


# Send messages to plotly's streaming servers
def plotly0(msg):
    print msg
    s = plotly.stream(stream_token0)
    data = json.loads(msg)
    data['x'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')
    s.write(data)

def plotly1(msg):
    print msg
    s = plotly.stream(stream_token1)
    data = json.loads(msg)
    data['x'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')
    s.write(data) 

# Initialize a YunMessenger console object
c = Console.Console()

# Assign a "subscriber" to `plotly` messages
c.onMessage['plotly_0'] = plotly0
c.onMessage['plotly_1'] = plotly1

# Connect to the socket on `localhost:6571` 
# Parse the data the comes over TCP for characters 29, 30, and 31
# and distribute "messages" (the data between characters 30 and 31)
# to the associated "subscriber" (the data between characters 29 and 30).

c.run()
