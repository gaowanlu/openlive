# openlive (network image transmission)

The OpenCV camera video stream used is pushed to the browser solution based on Base64 encoding in H264 format and transmitted using websocket. Use POSIX multithreading and C/C++plugins with Node API.

Can be used on Linux development boards such as Raspberry Pi and Linux system personal computers

## scheme

```txt

/**************************************************************************/
*     C++ module             <---->  Node SocketIO <---->  Browser(Player) *
* capture thread         <----------                       /|\              *
*    Take Frame(Loop)      start()                          |               *
*   |                                                       |               *
*   | Image(get_mat())                                      |               *
*   |                                                       |               *
*  \|/                                                      |               *
* encode_thread(Loop)                                       |               *
*   |-Convert to YUV color                      (H264Stream)|               *
*   | space                                                 |               *
*   |                                                       |               *
*   |-H264 encoder encodi-                                  |               *
*   | ng                                                    |               *
*   |                                                       |               *
*   |-Add H264 stream                                       |               *
*   | data to buffer                                        |               *
*   | queue                                                 |               *
*      |                              (Loop)                |               *
*      |------------------------->   io.emit()  ------------|               *
*       getFrame  callback()                                                *
*                                                                           *
/**************************************************************************/
```

## environmental preparation

libx264(https://www.videolan.org/developers/x264.html)  
opencv(https://github.com/opencv/opencv)

```shell
sudo apt install libx264-dev
sudo apt install -g node-gyp
sudo apt install libopencv-dev pkg-config
npm install
```

## conf

```js
let devVideo="http://10.34.119.245:8888";//stream,use MJPEG Streamer on Windows
devVideo="0";// 0 is /dev/video0 in opencv api,default
openlive.setConf({
    "path": devVideo,
    "encodeBufferLen": 5,
    "captureBufferLen": 5
});
//path：Path can be left unchecked and defaults to "0" to try opening the camera
//encodeBufferLen：H264 encoder frame buffer size，default to 5
//captureBufferLen：Camera frame buffer size，default to 5
let startRes = openlive.start();
const getInfo = () => {
    openlive.getMat((res) => {
        io.emit('chat message', res);
        setTimeout(getInfo, 1);
    });
}
if (startRes === true) {
    getInfo();
}
```

## compile to run

```shell
node-gyp configure
npm run build
node index
```

Browser Access http://IP:8887

![show](./resources/2023-04-14213407.jpg)
