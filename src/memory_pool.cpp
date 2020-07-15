#include "elti/memory_pool.h"
#include <algorithm>
#include <iostream>

void* MemoryBlock::constructMemoryBlock(uint32_t size) {
	char* ptr = new char[size + sizeof(uint32_t)];
	*(static_cast<uint32_t*>(static_cast<void*>(ptr))) = size;
	return static_cast<void*>(ptr + sizeof(uint32_t));
}

uint32_t MemoryBlock::getBlockSize(void* ptr) {
	char* tmp = static_cast<char*>(ptr) - sizeof(uint32_t);
	return *(static_cast<uint32_t*>(static_cast<void*>(tmp)));
}

void MemoryBlock::freeMemoryBlock(void* ptr) {
	char* tmp = static_cast<char*>(ptr) - sizeof(uint32_t);
	delete[] tmp;
}

MemoryBlockSet::MemoryBlockSet(uint32_t size, size_t blocks) : block_size_(size) {
	for (size_t i = 0; i < blocks; ++i) {
		void* tmp = MemoryBlock::constructMemoryBlock(size);
		unused_blocks_.insert(tmp);
	}
}

MemoryBlockSet::MemoryBlockSet(MemoryBlockSet&& other) noexcept : block_size_(other.block_size_) {
	unused_blocks_ = std::move(other.unused_blocks_);
	used_blocks_ = std::move(other.used_blocks_);
}

MemoryBlockSet& MemoryBlockSet::operator=(MemoryBlockSet&& other) noexcept {
	freeAllBlocks();
	block_size_ = other.block_size_;
	used_blocks_ = std::move(other.used_blocks_);
	unused_blocks_ = std::move(other.unused_blocks_);
	return *this;
}

uint32_t MemoryBlockSet::getBlockSize() const {
	return block_size_;
}

void* MemoryBlockSet::allocate() {
	if (unused_blocks_.empty() == true) {
		return nullptr;
	}
	auto it = unused_blocks_.begin();
	void* ptr = *it;
	used_blocks_.insert(ptr);
	unused_blocks_.erase(ptr);
	return ptr;
}

void* MemoryBlockSet::allocate_mt() {
	std::unique_lock<std::mutex> lock(mut_);
	if (unused_blocks_.empty() == true) {
		return nullptr;
	}
	auto it = unused_blocks_.begin();
	void* ptr = *it;
	used_blocks_.insert(ptr);
	unused_blocks_.erase(ptr);
	return ptr;
}

void MemoryBlockSet::deallocate(void* ptr) {
	if (getBlockSize() != MemoryBlock::getBlockSize(ptr)) {
		std::cerr << "critical. deallocate error. " << std::endl;
		exit(-1);
	}
	used_blocks_.erase(ptr);
	unused_blocks_.insert(ptr);
}

void MemoryBlockSet::deallocate_mt(void* ptr) {
	if (getBlockSize() != MemoryBlock::getBlockSize(ptr)) {
		std::cerr << "critical. deallocate error. " << std::endl;
		exit(-1);
	}
	std::unique_lock<std::mutex> lock(mut_);
	used_blocks_.erase(ptr);
	unused_blocks_.insert(ptr);
}

void MemoryBlockSet::freeAllBlocks() {
	if (used_blocks_.empty() == false) {
		std::cerr << "should not free used blocks. " << std::endl;
		exit(-1);
	}
	for (auto each : unused_blocks_) {
		MemoryBlock::freeMemoryBlock(each);
	}
	unused_blocks_.clear();
}

MemoryBlockSet::~MemoryBlockSet() {
	freeAllBlocks();
}

MemoryBlockSet::snap_info MemoryBlockSet::snap() const {
	snap_info result;
	result.block_size = block_size_;
	result.unused_block_count = unused_blocks_.size();
	result.used_block_count = used_blocks_.size();
	result.idleness = static_cast<double>(result.unused_block_count) / (result.unused_block_count + result.used_block_count);
	return result;
}

MemoryPool& MemoryPool::instance() {
	static MemoryPool obj;
	return obj;
}

std::vector<MemoryPool::snap_info> MemoryPool::snap() const {
	std::vector<snap_info> result;
	for (const auto& each : pool_) {
		result.push_back(each.snap());
	}
	return result;
}

void MemoryPool::init(std::initializer_list<std::pair<uint32_t, size_t>> list) {
	pool_.clear();
	for (auto each : list) {
		pool_.emplace_back(each.first, each.second);
	}
	std::sort(pool_.begin(), pool_.end(), [](const MemoryBlockSet& m1, const MemoryBlockSet& m2)-> bool {
		return m1.getBlockSize() < m2.getBlockSize();
	});
}

void* MemoryPool::allocate(size_t n) {
	for (auto& each : pool_) {
		if (each.getBlockSize() >= n) {
			return each.allocate();
		}
	}
	return nullptr;
}

void MemoryPool::deallocate(void* ptr) {
	if (ptr == nullptr) {
		return;
	}
	uint32_t block_size = MemoryBlock::getBlockSize(ptr);
	for (auto& each : pool_) {
		if (block_size == each.getBlockSize()) {
			each.deallocate(ptr);
			return;
		}
	}
	std::cerr << "dealloc errror , block size = " << block_size << std::endl;
	exit(-1);
}

void* MemoryPool::allocate_mt(size_t n) {
	for (auto& each : pool_) {
		if (each.getBlockSize() >= n) {
			return each.allocate_mt();
		}
	}
	return nullptr;
}

void MemoryPool::deallocate_mt(void* ptr) {
	if (ptr == nullptr) {
		return;
	}
	uint32_t block_size = MemoryBlock::getBlockSize(ptr);
	for (auto& each : pool_) {
		if (block_size == each.getBlockSize()) {
			each.deallocate_mt(ptr);
			return;
		}
	}
	std::cerr << "dealloc errror , block size = " << block_size << std::endl;
	exit(-1);
}

MemoryPool::MemoryPool() {
	std::initializer_list<std::pair<uint32_t, size_t>> list = {
		{64, 1024},
		{128, 512},
		{512, 128},
		{1024, 64}
	};
	for (auto each : list) {
		pool_.emplace_back(each.first, each.second);
	}
	std::sort(pool_.begin(), pool_.end(), [](const MemoryBlockSet& m1, const MemoryBlockSet& m2)-> bool {
		return m1.getBlockSize() < m2.getBlockSize();
	});
}
