{
        "targets": [
                {
                        "target_name": "main",
                        "sources": [
                                "src/base64/base64.cpp",
                                "src/x264Encoder/x264Encoder.cpp",
                                "src/capture_thread/capture_thread.cpp",
                                "src/encode_thread/encode_thread.cpp",
                                "src/main.cpp"
                                # "<!(ls src/*/*.cpp)",
                                # "<!(ls src/*.cpp)"
                        ],
                        "include_dirs": [
                                "<!@(node -p \"require('node-addon-api').include\")",
                                "src/"
                        ],
                        "libraries": [
                                "-lstdc++",
                                "-lpthread",
                                "<!(pkg-config opencv4 --libs)",
                                "<!(pkg-config x264 --libs)"
                        ],
                        "cflags": [
                                "-std=c++17 -fexceptions -frtti",
                                "<!(pkg-config opencv4 --cflags)",
                                "<!(pkg-config x264 --cflags)"
                        ],
                        "cflags_cc": [
                                "-std=c++17 -fexceptions -frtti",
                                "<!(pkg-config opencv4 --cflags)",
                                "<!(pkg-config x264 --cflags)"
                        ]
                }
        ],
}