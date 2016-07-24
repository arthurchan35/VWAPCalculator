#pragma once
#include "Message.h"
#include "Reference.h"
#include "Price4.h"
#include "AOMessage.h"
#include "Shares.h"

/* add order message , both types of add order messages belong to this one */
class ORMessage : public Message, public Reference, public Shares, public Price4 {
public:
	/* relevent contructor to this problem */
	ORMessage(char type, uint64_t newRef, uint64_t shares, uint32_t price4) 
		: Message(type), Reference(newRef), Shares(shares), Price4(price4) {}

	AOMessage* newAOM(char type, string stock) {
		return new AOMessage(type, this->getShares(), stock, this->getPrice4());
	}
	~ORMessage() {}
};

