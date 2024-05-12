
#include "RPG2Dpch.h"
#include "UID.h"
#include <iostream>
#include <unordered_set>
#include <random>
namespace RPG2D {
	UIDGenerator* UIDGenerator::s_Instance = nullptr;
	UIDGenerator::UIDGenerator():rng(std::random_device{}()), distribution(1, std::numeric_limits<uint64_t>::max()){}
	uint64_t UIDGenerator::generateUID() {
		//生成UID
		uint64_t id;
		do {
			id = distribution(rng);
		} while (allocatedIDs.count(id) > 0);

		allocatedIDs.insert(id);
		return id;
	}
	void UIDGenerator::releaseID(uint64_t id) {
		allocatedIDs.erase(id);
	}
	void UIDGenerator::insertID(uint64_t id) {
		allocatedIDs.insert(id);
	}
	bool UIDGenerator::isIDAllocated(uint64_t id) {
		return allocatedIDs.count(id) > 0;
	}
	UIDGenerator& UIDGenerator::Get() {
		if(!s_Instance)s_Instance = new UIDGenerator();
		return *s_Instance;
	}
	
	UID::UID() {
		id = UIDGenerator::Get().generateUID();
	}
	UID::UID(uint64_t id)
	{
		//需要判断当前id是否已经被占用
		if (UIDGenerator::Get().isIDAllocated(id)) {
			RPG2D_CORE_ERROR("UID {0} has been allocated!",id);
		}
		//没有占用则新增
		UIDGenerator::Get().insertID(id);
		this->id = id;
	}
	UID::UID(uint64_t id, bool isNewID)
	{
		if(!isNewID) {
			this->id = id;
		}
		else {
			//需要判断当前id是否已经被占用
			if (UIDGenerator::Get().isIDAllocated(id)) {
				RPG2D_CORE_ERROR("UID {0} has been allocated!",id);
			}
			//没有占用则新增
			UIDGenerator::Get().insertID(id);
			this->id = id;
		}
	}
	UID::~UID() {
		//析构时需要去除相应id
		UIDGenerator::Get().releaseID(id);
	}
	uint64_t UID::getID() {
		return id;
	}
	
}

