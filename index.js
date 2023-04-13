const main = require('./build/Release/main');
const http = require('http');
const fs = require('fs');

const server = http.createServer((req, res) => {
    console.log(req.url);
    if (req.url === '/') {
        // 读取 HTML 文件
        fs.readFile('./index.html', (err, data) => {
            if (err) {
                res.writeHead(500);
                return res.end('Error loading index.html');
            }
            // 返回 HTML 文件给客户端
            res.writeHead(200, { 'Content-Type': 'text/html' });
            res.end(data);
        });
    } else {
        res.writeHead(404);
        res.end('Page not found');
    }
});

const io = require('socket.io')(server, {
    allowEIO4: true,
    cors: {
        origin: "*", // refer
        methods: ["GET", "POST"]
    }
});
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
    setTimeout(getInfo, 10);
}

if (str === "true") {
    setTimeout(getInfo, 10);
}

