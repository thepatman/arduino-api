from YunMessenger import Console
import plotly
import json

# Load up plotly credentials
with open('./config.json') as config_file:
    plotly_user_config = json.load(config_file)

username = plotly_user_config['plotly_username'] 
api_key = plotly_user_config['plotly_api_key']
stream_token = plotly_user_config['plotly_streaming_tokens'][0]

# Make the initializing plotly request
p = plotly.plot(username, api_key)
p.off()
print p.plot([{'x': [], 'y': [], 'stream': {'token': stream_token}}])


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
