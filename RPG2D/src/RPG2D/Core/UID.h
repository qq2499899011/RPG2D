#pragma once
#include <iostream>
#include <unordered_set>
#include <random>

namespace RPG2D {
	class UIDGenerator {
	public:
		UIDGenerator();
		uint64_t generateUID();
		void releaseID(uint64_t id);
		void insertID(uint64_t id);
		bool isIDAllocated(uint64_t id);
		static UIDGenerator& Get();
	private:
		std::mt19937_64 rng;
		std::uniform_int_distribution<uint64_t> distribution;
		std::unordered_set<uint64_t> allocatedIDs;
		static UIDGenerator* s_Instance;
	};
	class UID
	{
	public:
		UID();
		UID(uint64_t id);
		~UID();
		uint64_t getID();
		operator uint64_t() const { return id; }
	private:
		uint64_t id;
	};
}
namespace std {
	template <typename T> struct hash;
	template<>
	struct hash<RPG2D::UID>
	{
		std::size_t operator()(const RPG2D::UID & uid) const
		{
			return (uint64_t)uid;
		}
	};
}

