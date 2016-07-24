#pragma once
#include <cstdint>

using namespace std;
/* price4 interface, only for easy purpose for this problem */
class Price4 {
	uint32_t price4; // divide by 10000 to get actual float number of price
public:
	/* relevent contructor to this problem */
	Price4(uint32_t price4) {
		this->price4 = price4;
	}

	uint32_t getPrice4() { return price4; }
	void setPrice4(uint32_t price4) { this->price4 = price4; }
	~Price4() {}
};

