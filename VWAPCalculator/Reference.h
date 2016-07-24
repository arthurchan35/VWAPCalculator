#pragma once
#include <cstdint>

using namespace std;
/* reference interface, only for easy purpose for this problem */
class Reference {
	uint64_t reference;
public:
	/* relevent contructor to this problem */
	Reference(uint64_t reference) {
		this->reference = reference;
	}

	uint64_t getReference() { return reference; }
	~Reference() {}
};

