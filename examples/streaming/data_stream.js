var hyperquest = require('hyperquest') // this is a stream friendly http request module from Substack
 , Signal = require("random-signal")  // This module produces random signal data



// hyperquest options
var httpOpts =  {
   method: 'POST'
 , uri: "http://192.168.2.17:8080"
 , headers: {
     "plotly-streamtoken": "1234567890"
 }
}

/*
* random signal stream options
* Plotly only accepts stringified newline seperated JSON
* so the separator is very important
*/
var sigOpts = {sep: "\n", tdelta: 10000}

var signalstream = Signal(sigOpts)
var plotlystream = hyperquest(httpOpts)

// Okay - stream to our plot!
signalstream.pipe(plotlystream)

plotlystream.on("error", function (err) {
   signalstream.destroy()
   console.log(err)
})