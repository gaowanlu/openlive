const express = require('express');
const http = require('http');
const { Server } = require('socket.io');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const AUTH_TOKEN = 'secure_token_123'; // Replace with the same secure token as in index.js

// Serve static files if needed
app.use(express.static('public'));

// Socket.IO server logic
io.on('connection', (socket) => {
    const isAuthorized = socket.handshake.auth.token === AUTH_TOKEN;

    if (isAuthorized) {
        console.log('Authorized index.js connected to broadcast_proxy');

        // Handle messages from index.js
        socket.on('proxy message', (msg) => {
            console.log('Broadcasting message to browsers:', msg);
            io.sockets.sockets.forEach((clientSocket) => {
                if (!clientSocket.handshake.auth.token) {
                    clientSocket.emit('chat message', msg); // Broadcast to browsers only
                }
            });
        });

        socket.on('disconnect', () => {
            console.log('Authorized index.js disconnected from broadcast_proxy');
        });
    } else {
        console.log('Browser client connected to broadcast_proxy');
        socket.on('disconnect', () => {
            console.log('Browser client disconnected from broadcast_proxy');
        });
    }
});

// Start the server
server.listen(20004, () => {
    console.log('broadcast_proxy listening on *:20004');
});
