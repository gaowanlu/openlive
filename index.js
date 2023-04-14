const main = require('./build/Release/main');
const express = require('express');
const app = express();
const server = require('http').Server(app);
const io = require('socket.io')(server);
const fs = require('fs');

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

server.listen(8887, "0.0.0.0", () => {
    console.log('listening on *:8887');
});

//C++ 模块运行
let str = main.thread_start();

const getInfo = () => {
    let str = main.get_mat();
    io.emit('chat message', str);
    setTimeout(getInfo, 1);
}

if (str === "true") {
    getInfo();
}

