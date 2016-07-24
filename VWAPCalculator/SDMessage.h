#pragma once
#include "Message.h"
#include "Stock.h"

/* stock directory message */
class SDMessage : public Message, public Stock {
public:
	/* relevent contructor to this problem */
	SDMessage(char type, string stock) 
		: Message(type), Stock(stock) {}
	~SDMessage() {}
};

