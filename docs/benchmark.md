## **GodBee-Benchmark**  

Bechmark with Locust and Boomer.
Run with 1000 users, Hatch rate 250.

Benchmark system:

- ThinkPad X280
- Intel(R) Core(TM) i5-8250U CPU @ 1.60GHz
- 2x8GiB SODIMM DDR4 2400 MHz
- Ubuntu 19.04

### **1. Set**  

#### **B-Tree**  

<div align="center">
    <img src="../images/set-btree.png">
</div>  

<div align="center">
    <img src="../images/b-set-1.png">
</div>  

#### **B+Tree**  

<div align="center">
    <img src="../images/set-b+tree.png">
</div>  

<div align="center">
    <img src="../images/b+set-1.png">
</div>  

### **2. Get**  

#### **B-Tree**  

<div align="center">
    <img src="../images/get-btree.png">
</div>  

<div align="center">
    <img src="../images/b-get-1.png">
</div>  

#### **B+Tree**  

<div align="center">
    <img src="../images/get-b+tree.png">
</div>  

<div align="center">
    <img src="../images/b+get-1.png">
</div>  

### **3. Exist**  

#### **B-Tree**  

<div align="center">
    <img src="../images/exist-btree.png">
</div>  

<div align="center">
    <img src="../images/b-exist-1.png">
</div>  

#### **B+Tree**  

<div align="center">
    <img src="../images/exist-b+tree.png">
</div>  

<div align="center">
    <img src="../images/b+exist-1.png">
</div>  

### **4. Remove**  

#### **B-Tree**  

<div align="center">
    <img src="../images/remove-btree.png">
</div>  

<div align="center">
    <img src="../images/b-rm-1.png">
</div>  

#### **B+Tree**  

<div align="center">
    <img src="../images/remove-b+tree.png">
</div>  

<div align="center">
    <img src="../images/b+remove-1.png">
</div>  

### **5. P99**  

P99 overview (miliseconds)

| Percentile | Storage | Exist | Get | Remove | Set  |
|------------|---------|-------|-----|--------|------|
| 50%        | B-Store | 20    | 19  | 88     | 110  |
|            | B+Store | 21    | 20  | 87     | 110  |
| 99%        | B-Store | 36    | 35  | 270    | 270  |
|            | B+Store | 39    | 37  | 250    | 270  |
| 100%       | B-Store | 71    | 59  | 880    | 1000 |
|            | B+Store | 77    | 75  | 810    | 1000 |
