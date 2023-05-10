const openlive = require('./build/Release/openlive');
const express = require('express');
const app = express();
const server = require('http').Server(app);
const io = require('socket.io')(server);

app.use(express.static('app'));

io.on('connection', (socket) => {
    console.log('a user connected');
    socket.on('chat message', (msg) => {
        console.log('message: ' + msg);
    });
    socket.on('disconnect', () => {
        console.log('user disconnected');
    });
});

server.listen(20002, "0.0.0.0", () => {
    console.log('listening on *:20002');
});

//C++ execute
openlive.setConf({
    "path": "0",
    "encodeBufferLen": 5,
    "captureBufferLen": 5
});

const getInfo = () => {
    openlive.getMat((res) => {
        io.emit('chat message', res);
        setTimeout(getInfo, 1);
    });
}

if (openlive.start()) {
    getInfo();
}

//netsh interface portproxy add v4tov4 listenport=20002 listenaddress=10.34.119.245 connectport=20002 connectaddress=127.0.0.1
//netsh interface portproxy delete v4tov4 listenport=20002 listenaddress=0.0.0.0
