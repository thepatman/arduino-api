/*
* npm install split
* npm install through2
* npm install timezone
*/


var http = require('http')
 , split = require("split")
 , through2 = require("through2")
 , tz = require("timezone")
 , server = http.createServer(sourcehandler);

server.listen(8080);


function sourcehandler (req, res) {

   if (req.method !== "POST") {
       console.log(JSON.stringify({msg: "Bad Request Method: " + req.method, code: 406}));
       req.destroy();
   }

   else {
       if (!req.headers["plotly-streamtoken"]) {
           console.log("BAD HEADERS");
           req.destroy();
       }
   }

   var token = req.headers["plotly-streamtoken"];
   console.log(req.headers);

    if(req.headers['plotly-converttimestamp']){
      var startTime = new Date().getTime()
        , convertTimestamp = true
        , timezone = req.headers['plotly-converttimestamp']
        , timezoneObj;
      console.log("got timezone: ", timezone);
      try{
        timezoneObj = require("timezone/"+timezone);
      } catch(e){
        timezoneObj = require("timezone/America/Montreal");
      }
    }

   console.log("GOT STREAM WITH TOKEN:", token);

   req.once('error', function (e) {
       console.log(JSON.stringify(({err:e, msg: "Request Stream Error", code: 500})));
   });

   req.once('close', function (haderror) {
       console.log("REQUEST CLOSING");
   });

   req.setEncoding('utf8');

   var datastream = through2(dateTransformer);
   var splitter = split();

   req.pipe(splitter).pipe(datastream).pipe(process.stdout);


  function dateTransformer (jsonStr, enc, callback) {

     var data = JSON.parse(jsonStr);

    if(convertTimestamp){
      data.x = tz(data.x+startTime, timezone, timezoneObj, "%m-%d-%Y %H:%M:%S.%N"); //http://bigeasy.github.io/timezone/#section-70
    }

     this.push(JSON.stringify(data)+"\n");

     callback();

  }


}


