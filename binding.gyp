{
	"targets":[
		{
			"target_name":"main",
			"sources": [
				"<!(ls src/*.cpp)",
				"<!(ls src/*/*.cpp)"
			],
			"include_dirs":[
				"<!@(node -p \"require('node-addon-api').include\")",
				"<!(pkg-config opencv4 --cflags)",
				"src/"
			],
			"libraries":[
				"-lstdc++",
				"-lpthread",
				"<!(pkg-config opencv4 --libs)"
			],
			"cflags": [ "-std=c++11 -fexceptions -frtti" ],
    		"cflags_cc": [ "-std=c++11 -fexceptions -frtti" ]
		}
	],	
}
