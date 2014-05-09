
### Plotly over Serial
Graph data from your arduino with just a serial connection. Plotly-Serial connects serially connected Arduinos to Plotly's real-time graphing API with the help of the [Standard Firmata protocal](/firmata/arduino), [Node.js](https://http://nodejs.org/), and [Johnny-Five](/rwaldron/johnny-five).

### Quickstart
1 - Install node.js: [http://nodejs.org/download/](http://nodejs.org/download/)
2 - Launch the Arduino IDE and upload the `Standard Firmata` sketch.
    ![Standard Firmata Sketch Screenshot](http://new.tinygrab.com/c751bc2ee2f590b37ee2c70217786a4f9cf73a6966.png)
3 - Once uploaded, close the Arduino IDE. We're done with it. Your computer will now communicate with your Arduino over serial with node.js via the Standard Firmata protocal. Nice!
3 - Open your terminal, create a project folder, move into it:
    ```bash
    $ mkdir plotly_project
    $ cd plotly_project
    ```
4 - Use the Node Package Manager (npm) to install two libraries
    node.js is awesome and will only install these modules in your project directory. It's easy to keep things modular and organized!
    
    ```bash
    $ npm install plotly
    $ npm install johnny-five
    ```
5 - Download one of our example scripts into your project folder and run it with node:
    
    ```bash
    $ node simple_plotly.js
    https://plot.ly/~streaming-demos/6
    ```

6 - Grab the url that is printed out and view your live-updating graph in plotly! Here is an example url: [https://plot.ly/~streaming-demos/6](https://plot.ly/~streaming-demos/6)

### Example Programs

See our [workshop page](https://plot.ly/workshop) for detailed instruction.

- DHT22 Temperature and Humidity sensor: [http://plot.ly/workshop/arduino-dht22/](http://plot.ly/workshop/arduino-dht22/)

- Analog Light Sensor: [http://plot.ly/workshop/arduino-analoglight/](http://plot.ly/workshop/arduino-analoglight/)

- ML8511 UV Sensor: [http://plot.ly/workshop/arduino-uvsensor/](http://plot.ly/workshop/arduino-uvsensor/)

- Air Quality Sensor: [http://plot.ly/workshop/arduino-airquality/](http://plot.ly/workshop/arduino-airquality/)

- Water Flow Sensor: [http://plot.ly/workshop/arduino-waterflow/](http://plot.ly/workshop/arduino-waterflow/)

- TMP36 Temperature Sensor: [http://plot.ly/workshop/arduino-tmp36/](http://plot.ly/workshop/arduino-tmp36/)