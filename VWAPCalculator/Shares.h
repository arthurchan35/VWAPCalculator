#pragma once
#include <cstdint>

using namespace std;
/* shares interface, only for easy purpose for this problem */
class Shares {
	uint64_t shares;
public:
	/* relevent contructor to this problem */
	Shares(uint64_t shares) {
		this->shares = shares;
	}

	uint64_t getShares() { return shares; }
	void setShares(uint64_t shares) { this->shares = shares; }
	~Shares() {}
};

