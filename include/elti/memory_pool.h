#include <cstdint>
#include <set>
#include <mutex>
#include <cstdint>
#include <vector>
#include <initializer_list>
#include <utility>

class MemoryBlock {
public:
	static void* constructMemoryBlock(uint32_t size);
	
	static uint32_t getBlockSize(void* ptr);

	static void freeMemoryBlock(void* ptr);
};

class MemoryBlockSet {
public:
	MemoryBlockSet(uint32_t size, size_t blocks);

	MemoryBlockSet(MemoryBlockSet&& other) noexcept ;

	MemoryBlockSet& operator=(MemoryBlockSet&& other) noexcept ;

	uint32_t getBlockSize() const;

	void* allocate();

	void* allocate_mt();

	void deallocate(void* ptr);

	void deallocate_mt(void* ptr);

	void freeAllBlocks();

	~MemoryBlockSet();

	struct snap_info {
		uint32_t block_size;
		size_t used_block_count;
		size_t unused_block_count;
		double idleness;
	};

	snap_info snap() const;

private:
	std::set<void*> unused_blocks_;
	std::set<void*> used_blocks_;
	uint32_t block_size_;

	std::mutex mut_;
};

class MemoryPool {
public:
	static MemoryPool& instance();

	using snap_info = MemoryBlockSet::snap_info;

	std::vector<snap_info> snap() const;

	void init(std::initializer_list<std::pair<uint32_t, size_t>> list);

	void* allocate(size_t n);

	void deallocate(void* ptr);

	void* allocate_mt(size_t n);

	void deallocate_mt(void* ptr);

private:
	MemoryPool();

	std::vector<MemoryBlockSet> pool_;
};
