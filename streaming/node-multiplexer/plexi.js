/*
* npm install split
* npm install through2
* npm install timezone //http://bigeasy.github.io/timezone
* npm install hyperquest
*/


var http = require('http')
, split = require("split")
, through2 = require("through2")
, tz = require("timezone") 
, server = http.createServer(sourcehandler)
, hyperquest = require("hyperquest");

server.listen(8080);

function sourcehandler (req, res) {
   var plotlyStreams = {};
   
   if (req.method !== "POST") {
      console.log(JSON.stringify({msg: "Bad Request Method: " + req.method, code: 406}));
      req.destroy();
   }

   else {
     if (req.headers["plotly-streamtoken"]) {
      var headertoken = req.headers["plotly-streamtoken"];
      plotlyStreams[headertoken] = plotlyStream(headertoken);
     }
   }

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

  req.on('error', function (e) {
   console.log(JSON.stringify(({err:e, msg: "Request Stream Error", code: 500})));
 });

  req.on('close', function (haderror) {
    closePlotlyStreams('');
    console.log("REQUEST CLOSING");
 });

  req.setEncoding('utf8');

  var splitter = split();
  splitter.on('error', function(){console.log('splitter error');});

  var dateTransformer = through2(function dateTransformerFunc(jsonStr, enc, callback){
    var data;
    try{    
      data = JSON.parse(jsonStr);
    } catch(e){
      // Pass the junk to streamy to error handle
      this.push(jsonStr+'\n');
      callback();
      return;
    }
    if(convertTimestamp){
      data.x = tz(data.x+startTime, timezone, timezoneObj, "%Y-%m-%d %H:%M:%S.%N"); //http://bigeasy.github.io/timezone/#section-70
    }
    this.push(JSON.stringify(data)+"\n");
    callback();
  });
  dateTransformer.on('error', function(){console.log('dateTransformer error');});

  var plexer = through2(function plexerFunc(jsonStr, enc, callback) {
    var data;
    try{    
      data = JSON.parse(jsonStr);
    } catch(e){
      // Pass the junk to streamy to error handle
      this.push(jsonStr+'\n');
      callback();
      return;
    }
    if('streamtoken' in data){
      var streamtoken = data.streamtoken;
      delete data.streamtoken;      
      if(!(streamtoken in plotlyStreams)){
        // initialize new stream
        plotlyStreams[streamtoken] = plotlyStream(streamtoken);
      }
      plotlyStreams[streamtoken].write(JSON.stringify(data)+"\n");
    } else if(typeof headertoken != 'undefined'){
      // if token not supplied in data but was supplied in header, 
      // then write to the header token stream
      plotlyStreams[headertoken].write(JSON.stringify(data)+"\n");
    }

    this.push(JSON.stringify(data)+"\n");
    callback();
  });

  plexer.on('error', function(){console.log('plexer error');});

  function plotlyStream(token) {
    var streamReq = hyperquest({
      method: 'POST'
    , uri: "http://stream.plot.ly" // "http://localhost:8090" // 
    , port: 80 //8090 // 
    , headers: {
        "connection": "keepalive"
      , "plotly-streamtoken": token
      }
    });

    streamReq.on('response', function(response){
      console.log("RESPONSE: ", response.statusCode, ": ", token);
      res.writeHead(response.statusCode);
      res.end('response.statusCode'); // TODO: Lookup message
      res.destroy();
      req.destroy();
      plexer.end();
      closePlotlyStreams(token);
    });

    streamReq.on('error', function(err){
      console.log('streamReq error: ', token, ' --> ', err);
    });

    streamReq.on('close', function(err){
      console.log('streamReq closed: ', token);
    });
    return streamReq;
  }

  function closePlotlyStreams(dontCloseThisToken){
    // Destroy all connected streams to stream.plot.ly,
    for(var tokeni in plotlyStreams){
      if(tokeni != dontCloseThisToken){
        console.log("CLOSING: ", tokeni);
        plotlyStreams[tokeni].destroy();
      }
    }
  }

  req.pipe(splitter).pipe(dateTransformer).pipe(plexer);//.pipe(process.stdout);

}















