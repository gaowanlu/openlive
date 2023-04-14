# gyp-liver (network image transmission)

The OpenCV camera video stream used is pushed to the browser solution based on Base64 encoding in H264 format and transmitted using websocket. Use POSIX multithreading and C/C++plugins with Node API.

Can be used on Linux development boards such as Raspberry Pi and Linux system personal computers

## scheme

```txt

        C++ module        <---->  Node Server <---->  Browser(Player)
camera thread(Loop)      <----------                     /|\
    Take Frame          thread_start()                    |
                                                          |
    Convert to YUV color                      (H264Stream)|
    space                                                 |
                                                          |
    H264 encoder encodi-                                  |
    ng                                                    |
                                                          |
    Add H264 stream                                       |
                        get_mat(Blocking                  |
    data to buffer        waiting)                        |
    queue                ----------->   io.emit()---------|
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

## compile to run

```shell
node-gyp configure
npm run build
node index
```

Browser Access http://IP:8887

![show](./resources/2023-04-14213407.jpg)
