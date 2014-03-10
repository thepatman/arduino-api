var net = require('net');

var HOST = '127.0.0.1';
var PORT = 8080;

net.createServer(function(socket) {        
     socket.on('error', function (e) {
         console.log(JSON.stringify(({err:e, msg: "Request Stream Error", code: 500})))
     })

    socket.on('close', function(data) {
        console.log('REQUEST CLOSING');
    });

    socket.on('data', function(data) {
        console.log('DATA ' + socket.remoteAddress + ': ' + data);
    });

}).listen(PORT, HOST);