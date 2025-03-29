const openlive = require('./build/Release/openlive'); // import openlive cpp moudle
const express = require('express'); // static web server with express
const { io: Client } = require('socket.io-client'); // socket.io client

const app = express(); // create express instance
const server = require('http').Server(app); // http server
const io = require('socket.io')(server); // socket.io ws server for broadcast h264 stream

// Connect to broadcast_proxy server with authentication token
const AUTH_TOKEN = 'secure_token_123'; // Replace with a secure token
let proxySocket = null;

const connectToProxy = () => {
    proxySocket = Client('http://localhost:20004', {
        auth: { token: AUTH_TOKEN },
        reconnection: true, // Enable automatic reconnection
        reconnectionAttempts: Infinity, // Retry indefinitely
        reconnectionDelay: 1000 // Retry every 1 second
    });

    proxySocket.on('connect', () => {
        console.log('Connected to broadcast_proxy');
    });

    proxySocket.on('disconnect', () => {
        console.warn('Disconnected from broadcast_proxy, attempting to reconnect...');
        connectToProxy(); // Attempt to reconnect
    });

    proxySocket.on('connect_error', (err) => {
        console.error('Error connecting to broadcast_proxy:', err.message);
    });
};

// Initialize connection to broadcast_proxy
connectToProxy();

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
server.listen(20003, "0.0.0.0", () => {
    console.log('listening on *:20003');
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
        if (res && res.length > 0) {
            io.emit('chat message', res); // Emit to local clients
            if (proxySocket && proxySocket.connected) {
                proxySocket.emit('proxy message', res); // Forward to broadcast_proxy
            } else {
                console.warn('proxySocket is not connected, skipping message forwarding');
            }
        } else {
            console.error("res null or res.length <= 0");
        }
        setTimeout(getInfo, 1);
    });
}

// start reader loop
if (openlive.start()) {
    getInfo();
}
