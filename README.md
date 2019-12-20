# **ZP Key-Value Store**  

## **Overview**  

Key-Value Store is the simplest type of database where each key is only mapped with one value. However, implementing a key-value store and store data on disk is a bit challenged because you have to cope with several problems about memory leak, race condition in multithreading environment, read/write file, I/O optimization.  

In this project, we use B-Tree and B+Tree data structures to organize and manipulate data. Service layer is written in Golang programming language. We use gRPC services to handle requests from client and use CGO to access data from C++ storage. 

This project is contributed by [Tran.](https://github.com/TranNDC) and [Quyen Pham](https://github.com/ptq204) 

## **Architecture**  

<div align="center">
    <img src="images/architect.png">
</div>  

## **Requirements**  

- C++17
- Golang 1.13.1
- Locust
- Docker Engine - Community 19.03.3  

## **Methods supported**  

```java
syntax = "proto3";

package service;

message Status {
    //code = 1 means success
    int32 code = 1;
    string error = 2;
}

message MessageResponse {
    Status status = 1;
}

enum StoreType {
    BTreeDisk = 0;
    BPlusTreeDisk = 1;
}

message ConnectionRequest {
    StoreType type = 1;
}

message CloseConnectionRequest {
}

message GetRequest {
    string key = 1;
}

message GetResponse {
    Status status = 1;
    string value = 2;
}

message SetRequest {
    string key = 1;
    string value = 2;
}

message RemoveRequest {
    string key = 1;
}

message RemoveResponse {
    Status status = 1;
    bool check = 2;
}

message ExistRequest {
    string key = 1;
}

message ExistResponse {
    Status status = 1;
    bool check = 2;
}

service KeyValueStoreService {
    rpc Connect(ConnectionRequest) returns (MessageResponse){}
    rpc CloseConnection (CloseConnectionRequest) returns (MessageResponse) {}
    rpc Get (GetRequest) returns (GetResponse) {}
    rpc Set (SetRequest) returns (MessageResponse) {}
    rpc Remove (RemoveRequest) returns (RemoveResponse) {}
    rpc Exist (ExistRequest) returns (ExistResponse) {}
}
```  

## **Build**  

```sh
# clone project
git clone https://github.com/zalopay-oss/key-value-store-service.git
```  

## **Run**  

- Make CGO understands C++17

```sh
# Modify Go enviroment variable
export CGO_CXXFLAGS="-g -rdynamic -std=c++17 -o -pthread
```  

- Run server:  

```sh
cd source
# Build server
make build-server
# Run server
./server
```  

- Or run docker server:  

```sh
cd source
# Build docker image named "zpkv-server"
sudo docker build -t zpkv-server .
# Run image
sudo docker run -it --net="host" zpkv-server
```

- Run client:  

```sh
cd source
# Build and run client
make client
```  

## **Test**  

### Test CLI

- **Connect** to B/B+ Storage:

> CONNECT {B | BPLUS}

- **Disconnect** to the storage:

> CLOSE

- **Insert** new Key-Value:

> SET key value

- **Get** value from key:

> GET key

- **Remove** key:

> REMOVE key

or

> DEL key

- Check whether key **exists** in storage or not.

> EXIST key

Example:  

```sh
KVZP > connect B
OK
KVZP > SET a a
OK
KVZP > GET a
"a"
KVZP > EXIST a
TRUE
KVZP > DEL a
OK
KVZP > GET a
(nil)
KVZP > EXIST a
FALSE
KVZP > SET b b
OK
KVZP > GET b
"b"
KVZP > exit
Bye bye!!
```

### Run unit tests  

```sh
cd source
make test
```  

## **Benchmark**  

You can view benchmark results at [here](docs/benchmark.md).  

P99 overview (miliseconds)

| Percentile | Storage | Exist | Get | Remove | Set  |
|------------|---------|-------|-----|--------|------|
| 50%        | B-Store | 20    | 19  | 88     | 110  |
|            | B+Store | 21    | 20  | 87     | 110  |
| 99%        | B-Store | 36    | 35  | 270    | 270  |
|            | B+Store | 39    | 37  | 250    | 270  |
| 100%       | B-Store | 71    | 59  | 880    | 1000 |
|            | B+Store | 77    | 75  | 810    | 1000 |

## **Contribution**  

If you find anything wrong or want to discuss more about this project, feel free to create issues or create pull requests.  

## **Acknowledgements**  

Thanks [AJ Pham](https://github.com/phamtai97) for guiding me during the project.  

