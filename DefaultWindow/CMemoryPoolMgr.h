#pragma once
class CMemoryPoolMgr
{
private:
	CMemoryPoolMgr(size_t blockSize, size_t poolSize)
		: blockSize(blockSize)
	{
		for (size_t i = 0; i <= poolSize; ++i) {
			pool.push_back(std::malloc(blockSize));
		}
	}
	~CMemoryPoolMgr() 
	{
		for (void* block : pool) 
		{
			std::free(block);
		}
	}


public:
	static CMemoryPoolMgr* Get_Instance(size_t blockSize = 0, size_t poolSize = 0) {
		if (!m_pInstance) {
			m_pInstance = new CMemoryPoolMgr(blockSize, poolSize);
		}
		return m_pInstance;
	}


	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	// 메모리 할당
	void* allocate() 
	{
		void* block = pool.back();
		pool.pop_back();
		return block;
	}

	// 메모리 반환
	void deallocate(void* ptr) {
		// 중복 반환 방지
		auto it = std::find(pool.begin(), pool.end(), ptr);
		pool.push_back(ptr);
	}


private:
	static CMemoryPoolMgr* m_pInstance;
	std::vector<void*> pool;  // 메모리 블록 저장
	size_t blockSize;         // 블록 크기
};

