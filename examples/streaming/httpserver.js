var http = require('http')
 , server = http.createServer(sourcehandler)

server.listen(8080)

function sourcehandler (req, res) {

   req.on('error', function (e) {
       console.log(JSON.stringify(({err:e, msg: "Request Stream Error", code: 500})))
   })

   req.on('close', function () {
      console.log("REQUEST CLOSING")
   })

   req.on('data', function (data) {
       console.log(data)
   })

   console.log(req.headers)

}