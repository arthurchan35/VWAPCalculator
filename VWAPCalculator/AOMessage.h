#pragma once
#include "Message.h"
#include "Stock.h"
#include "Price4.h"
#include "Shares.h"

/* add order message , both types of add order messages belong to this class */
class AOMessage : public Message, public Shares, public Stock, public Price4 {
public:
	/* relevent contructor to this problem */
	AOMessage(char type, uint64_t shares, string stock, uint32_t price4) 
		: Message(type), Shares(shares), Stock(stock), Price4(price4) {}
	~AOMessage() {}
};

