# arbys-bignum
A _modern_ C++ library for arbitrary-precision integer arithmetic, _built from scratch_ with a focus on **_clean API design_**.

## Features
- Arbitrary-precision signed integers (big_int)
- Basic arithmetic:
    - addition, subtraction
    - multiplication
    - division
    - comparison

## Requirements
- C++26 compatible compiler
- CMake >= 3.28

## Building & Installing
### 1. Clone this repository
```bash
git clone https://github.com/arbys-gnome/arbys-bignum.git
cd arbys-bignum
```
### 2. Configure & Build
```bash
cmake -S . -B build
cmake --build build
```
### 3. Run tests
```bash
ctest --test-dir build
```
### 4. Install system-wide
```bash
sudo cmake --install build
```

## Using _arbys-bignum_ in your project
### CMake integration
```cmake
find_package(arbys-bignum REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE arbys::bignum)
```