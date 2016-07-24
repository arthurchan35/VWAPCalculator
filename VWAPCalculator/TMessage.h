#pragma once
#include "Message.h"
#include "Stock.h"
#include "Price4.h"
#include "Shares.h"
/* trade message, all execution and trade(cross) messages included in this class */
class TMessage : public Message, public Shares, public Stock, public Price4 {
public:
	/* relevent contructor to this problem */
	TMessage(char type, uint64_t shares)
		: Message(type), Shares(shares), Stock(""), Price4(0) {}

	TMessage(char type, uint64_t shares, uint32_t price4)
		: Message(type), Shares(shares), Stock(""), Price4(price4) {}

	TMessage(char type, uint64_t shares, string stock, uint32_t price4)
		: Message(type), Shares(shares), Stock(stock), Price4(price4) {}

	~TMessage() {}
};

