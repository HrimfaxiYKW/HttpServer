# HttpServer
Develop configure
  Cmake : 3.9.2
  gcc : 7.3.1
  g++ : 7.3.1

How to run
  build client and server : ./build.sh -a
  build client : ./build.sh -c
  build server : ./build.sh -s

Then you can find the binary under /build/src/....

Layout:
├── build
│   ├── CMakeCache.txt
│   ├── ......
│   ├── Makefile
│   └── src
│       ├── client
│       │   ├── client
│       │   ├── ......
│       │   └── Makefile
│       ├── server
│       └── ├── server
│           ├── ......
│           └── Makefile
├── build.sh
├── CMakeLists.txt
├── README.md
└── src
    ├── client
    │   ├── client.cpp
    │   └── CMakeLists.txt
    ├── CMakeLists.txt
    └── server
        ├── CMakeLists.txt
        └── server.cpp
