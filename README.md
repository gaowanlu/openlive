# gyp-liver

The OpenCV camera photo stream push to browser scheme used in local area networks is based on Base64 encoding in JPEG format and transmitted using websocket. Use POSIX multithreading and C/C++addons with Node API.

Environmental preparation

```shell
sudo apt install -g node-gyp
sudo apt install libopencv-dev pkg-config
npm install
```

Compile to Run

```shell
node-gyp configure
npm run build
node index
```