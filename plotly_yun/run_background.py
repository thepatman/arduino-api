import plotly
import time

while True:
    try:
        s = plotly.stream('ds5xbsv9il')
        while True:
            s.write({'x': time.time(), 'y': time.time()})
            time.sleep(1)
    except KeyboardInterrupt:
        break
    except:
        time.sleep(1)
        try:
            s.close()
        except:
            pass