#pragma once

#include "elti/varint_num.h"
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>

namespace elti {
template<typename T>
void seri(const T& obj, std::vector<uint8_t>& container);

template<size_t n>
void seri(const char(&arr)[n], std::vector<uint8_t>& container) {
  container.resize(n);
  memcpy(&container.front(), &(arr[0]), n);
}

template<size_t n>
void seri(char(&arr)[n], std::vector<uint8_t>& container) {
  container.resize(n);
  memcpy(&container.front(), &(arr[0]), n);
}

void seri(const std::vector<uint8_t>& obj, std::vector<uint8_t>& container);

void seri(std::vector<uint8_t>&& obj, std::vector<uint8_t>& container);

void seri(const std::string& obj, std::vector<uint8_t>& container);

void seri(const char* obj, std::vector<uint8_t>& container);

void seri(char* obj, std::vector<uint8_t>& container);

void seri(const int8_t& obj, std::vector<uint8_t>& container);

void seri(const uint8_t& obj, std::vector<uint8_t>& container);

void seri(const int16_t& obj, std::vector<uint8_t>& container);

void seri(const uint16_t& obj, std::vector<uint8_t>& container);

void seri(const int32_t& obj, std::vector<uint8_t>& container);

void seri(const uint32_t& obj, std::vector<uint8_t>& container);

void seri(const int64_t& obj, std::vector<uint8_t>& container);

void seri(const uint64_t& obj, std::vector<uint8_t>& container);

void seri(const varintNum& obj, std::vector<uint8_t>& container);

void seri(const bool& obj, std::vector<uint8_t>& container);

std::string parse(const std::vector<uint8_t>& container);

//只有返回值不同的函数不能重载，因此需要使用模板特化的方式实现。
template<typename T>
T parse(const char* ptr, size_t n);

template<>
std::string parse(const char* ptr, size_t n);

template<>
std::vector<uint8_t> parse(const char* ptr, size_t n);

template<>
int8_t parse(const char* ptr, size_t n);

template<>
uint8_t parse(const char* ptr, size_t n);

template<>
int16_t parse(const char* ptr, size_t n);

template<>
uint16_t parse(const char* ptr, size_t n);

template<>
int32_t parse(const char* ptr, size_t n);

template<>
uint32_t parse(const char* ptr, size_t n);

template<>
int64_t parse(const char* ptr, size_t n);

template<>
uint64_t parse(const char* ptr, size_t n);

template<>
varintNum parse(const char* ptr, size_t n);

template<>
bool parse(const char* ptr, size_t n);
}
