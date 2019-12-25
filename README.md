# **GodBee**  

![Build Status](https://img.shields.io/badge/build-passing-brightgreen) ![Project Level](https://img.shields.io/badge/level-experiment-yellowgreen) ![Coverage Status](https://img.shields.io/badge/coverage-90%25-green) 
[![version](https://img.shields.io/badge/repo%20status-active-brightgreen)](https://github.com/zalopay-oss/key-value-store-service) [![version](https://img.shields.io/badge/contributors-2-blueviolet)](https://github.com/zalopay-oss/key-value-store-service/graphs/contributors) [![version](https://img.shields.io/badge/open%20issues-0-red)](https://github.com/zalopay-oss/key-value-store-service/issues)

<div align="center">
    <img width=50% src="images/logo.png">
</div>  

<!-- TOC -->
- [GodBee](#godbee)
  - [Overview](#overview)
  - [Architecture](#architecture)
  - [Requirements](#requirements)
  - [Methods supported](#methods-supported)
  - [Build](#build)
  - [Run](#run)
  - [Test](#test)
    - [Test CLI](#test-cli)
    - [Run unit tests](#run-unit-tests)
  - [Benchmark](#benchmark)
  - [Documentation](#documentation)
  - [Acknowledgements](#acknowledgements)

## **Overview**  

GodBee is a Key-Value Store Service project. In this project, we choose B-Tree and B+Tree data structures to organize and manipulate data. Key-Value Storage is written in C++ and Service layer is written in Golang programming language. We use gRPC services to handle requests from client and use CGO to access data from C++ storage.  

## **Architecture**  

<div align="center">
    <img src="images/GodbeeArchitecture.png">
</div>  

## **Requirements**  

- C++17
- Golang 1.13.1
- Locust
- Python 3.7.3
- Docker Engine - Community 19.03.3  

## **Methods supported**  

```java
syntax = "proto3";

package service;

service KeyValueStoreService {
    rpc Connect (ConnectionRequest) returns (MessageResponse){}
    rpc Disconnect (DisconnectRequest) returns (MessageResponse) {}
    rpc Get (GetRequest) returns (GetResponse) {}
    rpc Set (SetRequest) returns (MessageResponse) {}
    rpc Remove (RemoveRequest) returns (RemoveResponse) {}
    rpc Exist (ExistRequest) returns (ExistResponse) {}
}
```  

## **Build**  

```sh
# Clone project
git clone https://github.com/zalopay-oss/godbee.git
```  

## **Run**  

- Make CGO understands C++17

```sh
# Modify Go enviroment variable
export CGO_CXXFLAGS="-g -rdynamic -std=c++17 -o -pthread
```  

- Run server:  

```sh
# Build server
make build-server

# Run server
./server
```  

- Or run Docker server:  

```sh
# Build docker image named "godbee-server"
docker build -t godbee-server .

# Run image
docker run -it --net="host" godbee-server
```

## **Test**  

### Test CLI

After [run server](#run), you can use GodBee-CLI to send commands to GodBee, and read the replies sent by the server, directly from the terminal.

**Run CLI**:  

```sh
# Build and run client
make cli
```  

**Command line usage**:

- **Connect** to B/B+ Storage:

```sh
# CONNECT {B | BPLUS}
GodBee > CONNECT B
OK
```  

- **Disconnect** to the storage:

```sh
# CLOSE
GodBee > CLOSE
OK
```

- **Insert** new Key-Value:

```sh
# SET key value
GodBee > SET a a
OK
```

- **Get** value from key:

```sh
# GET key
GodBee > GET a
"a"
```

- **Remove** key:

```sh
# {REMOVE|DEL} key
GodBee > DEL a
OK
```

- Check whether key **exists** in storage or not.

```sh
# EXIST key
GodBee > EXIST a
FALSE
```

- Exit program

```sh
GodBee > exit
Bye bye!!! Beeeeee~
```  

### Run unit tests  

```sh
# Run test
make test
```  

## **Benchmark**  

You can view benchmark results at [here](docs/benchmark.md).

Benchmark system:

- ThinkPad X280
- CPU: Intel(R) Core(TM) i5-8250U CPU @ 1.60GHz
- 16GiB SODIMM DDR4 2400 MHz

P99 overview (miliseconds)

| Percentile | Storage | Exist | Get | Remove | Set  |
|------------|---------|-------|-----|--------|------|
| 50%        | B-Store | 20    | 19  | 88     | 110  |
|            | B+Store | 21    | 20  | 87     | 110  |
| 99%        | B-Store | 36    | 35  | 270    | 270  |
|            | B+Store | 39    | 37  | 250    | 270  |
| 100%       | B-Store | 71    | 59  | 880    | 1000 |
|            | B+Store | 77    | 75  | 810    | 1000 |

## **Documentation**

- Slide: [B+ Tree](docs/B+tree.pdf), [GodBee](docs/GodBee.pdf)

## **Acknowledgements**  

Thanks [AJ Pham](https://github.com/phamtai97) for guiding us during the project.  
