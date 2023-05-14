# OpenLive

The OpenCV camera video stream used is pushed to the browser solution based on Base64 encoding in H264 format and transmitted using websocket. Use POSIX multithreading and C/C++plugins with Node API.

Can be used on Linux development boards such as Raspberry Pi and Linux system personal computers

## Scheme

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
*       getFrame getMat  callback()                                         *
*                                                                           *
/**************************************************************************/
```

## Environmental Preparation

* [Libx264](https://www.videolan.org/developers/x264.html)
* [OpenCV](https://github.com/opencv/opencv)

```shell
sudo apt install libx264-dev
sudo npm install -g node-gyp
sudo apt install libopencv-dev pkg-config
npm install
```

## API Documentation

* `import openlive`

```js
const openlive = require('./build/Release/openlive');
```

* `setConf`

configuration parameter

```js
openlive.setConf({
    "path": "0",
    "encodeBufferLen": 5,
    "captureBufferLen": 5
});
```

path(string): The default is "0", opencv opens the camera's parameters, camera device number,/dev/video0. Simultaneously supports HTTP streaming, such as using MJPEG Streamer to push streams, which can be filled in ` http://ip:port `.  
encodeBufferLen(number): Default value is 5 , encoder frame buffer size.
captureBufferLen(number): The default value is 5, and the camera takes the image frame buffer size.  

* `getMat`

Retrieve the encoded H264 bare stream frame data from the encoder, and the returned data is encoded in Base64 format. It needs to be called after the start method is called before it can be called.

```js
openlive.getMat((res) => {

});
```

* `start`

Please call after calling the setConf method. The return value is a Boolean value. The range of openlive startup success is true, otherwise it returns false. After starting, the getMat method can be used.

```js
openlive.start()
```

## Examples

* [Browser viewing of real-time camera images](./index.js)

## Compile to run

```shell
node-gyp configure
npm run build
node index
```

![show](./resources/2023-04-14213407.jpg)
