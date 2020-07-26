#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include "varint_num.h"
#include "ref.h"

namespace elti {
template<typename T>
void seri(const T& obj, std::vector<uint8_t>& container);

template<size_t n>
void seri(const char(&arr)[n], std::vector<uint8_t>& container) {
  container.resize(n);
  memcpy(&container.front(), &(arr[0]), n);
}

template<typename T>
T parse(const std::vector<uint8_t>& container);

void seri(const std::vector<uint8_t>& obj, std::vector<uint8_t>& container);

void seri(std::vector<uint8_t>&& obj, std::vector<uint8_t>& container);

void seri(const std::string& obj, std::vector<uint8_t>& container);

void seri(const char* obj, std::vector<uint8_t>& container);

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

template<>
std::string parse(const std::vector<uint8_t>& container);

template<>
std::vector<uint8_t> parse(const std::vector<uint8_t>& container);

template<>
int8_t parse(const std::vector<uint8_t>& container);

template<>
uint8_t parse(const std::vector<uint8_t>& container);

template<>
int16_t parse(const std::vector<uint8_t>& container);

template<>
uint16_t parse(const std::vector<uint8_t>& container);

template<>
int32_t parse(const std::vector<uint8_t>& container);

template<>
uint32_t parse(const std::vector<uint8_t>& container);

template<>
int64_t parse(const std::vector<uint8_t>& container);

template<>
uint64_t parse(const std::vector<uint8_t>& container);

template<>
varintNum parse(const std::vector<uint8_t>& container);
}
