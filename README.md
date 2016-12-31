# logfw
> A logger framework

logfw is basis for creating own logging library

it's provide ability to prepare format string and format arguments in-place and ability to serialize prepared data in separate thread or process

## features

* format string pre-processing
* type safe arguments formatting
* no overhead in runtime
* low latency
* header only

## requirements
* c++14 compiler

## build

the project is built with *cmake*

```
git clone https://github.com/ksergey/logfw.git
cd logfw
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
