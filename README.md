# V8 Inspector example

This repository contains a very simple example how to use a standalone V8 Inspector (without Node.js) with ChromeDevTools.

# Dependencies
* V8 7.4.288
* Boost.Beast (for WebSocket communication between ChromeDevTools and V8 Inspector)

# Building
```shell
$ mkdir build && cd build
$ cmake -DV8_INCLUDE_DIR=../deps/include/ -DV8_MONOLITH_LIB_PATH=../deps/libv8_monolith.a ../
$ make

# Execute the binary
$ ./v8_inspector_example
```
