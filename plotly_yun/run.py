from YunMessenger import Console
import plotly
import json

def streamz(msg):
    print msg
    s = plotly.stream('954epeiyvz')
    s.write(json.parse(msg))

c = Console.Console()
c.onMessage['plotly'] = streamz

c.run()