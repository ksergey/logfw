# LogFW
> A logger framework

LogFW is basis for creating own logging library

Provide ability to prepare format string and format arguments in-place and ability to serialize prepared data in separate thread or process

## Features

* Format string pre-processing
* Type safe arguments formatting
* No overhead in runtime
* Low latency
* Header only

## Requirements
* c++17 compiler

## Build

The project is built with *cmake*

```
git clone https://github.com/ksergey/logfw.git
cd logfw
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
