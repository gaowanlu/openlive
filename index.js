const openlive = require('./build/Release/openlive'); // import openlive cpp moudle
const express = require('express'); // static web server with express

const app = express(); // create express instance
const server = require('http').Server(app); // http server
const io = require('socket.io')(server); // socket.io ws server for broadcast h264 stream

app.use(express.static('app')); // init static resource path

// ws init
io.on('connection', (socket) => {
    console.log('a user connected');
    socket.on('chat message', (msg) => {
        console.log('message: ' + msg);
    });
    socket.on('disconnect', () => {
        console.log('user disconnected');
    });
});

// static http server init
server.listen(20002, "0.0.0.0", () => {
    console.log('listening on *:20002');
});

// openlive config
openlive.setConf({
    "path": "0",
    "encodeBufferLen": 5,
    "captureBufferLen": 5
});

// loop stream reader callback
const getInfo = () => {
    openlive.getMat((res) => {
	if(res && res.length > 0)
	{
           io.emit('chat message', res);
	}
	else
	{
           console.error("res null or  res.length <= 0");
	}
	setTimeout(getInfo, 1);
    });
}

// start reader loop
if (openlive.start()) {
    getInfo();
}

