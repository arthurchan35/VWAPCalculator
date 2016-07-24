#pragma once
#include <cstdint>
#include <iostream>
#include <string>
using namespace std;
/* parent class for all message types, only storing relavent infomation */
class Message {
	char type;

public:
	/* relevent contructor to this problem */
	Message(char type) { this->type = type; }
	char getType() { return type; }
	virtual ~Message() {
	}
};

