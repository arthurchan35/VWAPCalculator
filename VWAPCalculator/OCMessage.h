#pragma once
#include "Message.h"
#include "Shares.h"

/* order cancel message */
class OCMessage : public Message, public Shares {
public:
	/* relevent contructor to this problem */
	OCMessage(char type, uint64_t shares)
		: Message(type), Shares(shares) {}

	~OCMessage() {}
};

