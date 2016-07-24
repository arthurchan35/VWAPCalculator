#pragma once
#include <fstream>
#include "AOMessage.h"
#include "SDMessage.h"
#include "OCMessage.h"
#include "ORMessage.h"
#include "TMessage.h"
#define STOCKNAMESIZE 8
#define PROGRESSTHRESHHOLD 134217718 //128 * 1024 * 1024 - 10
using namespace std;
class Parser {
	ifstream binaryData;
	char* buffer;
	uint32_t currentProgress; // used for calculation loading progress
	uint32_t totalProgress; // used for calculation loading progress

	Parser(char* file);
	/* print out how many MB of data been read so far */
	inline void printProgress(uint32_t size);
	/* change endian for a 4 bytes unsigned integer, return new endian integer */
	inline uint32_t swapEndian(uint32_t* i);
	/* change endian for a 8 bytes unsigned integer, return new endian integer */
	inline uint64_t swapEndian(uint64_t* l);
	/* get stock symbol at given start of a character buffer */
	inline string getStockString(char* start);
	/* build a stock directory message with given character buffer */
	Message* buildSD(char* buffer);
	/* build an add order message with given character buffer */
	Message* buildAO(char* buffer, uint64_t& reference);
	/* build an order executed message(excluding order executed with price message) with given character buffer */
	Message* buildOE(char* buffer, uint64_t& reference, uint64_t& match);
	/* build an order executed with price message with given character buffer */
	Message* buildOEWP(char* buffer, uint64_t& reference, uint64_t& match);
	/* build an order cancel message with given character buffer */
	Message* buildOC(char* buffer, uint64_t& reference);
	/* build an order delete message with given character buffer */
	Message* buildOD(char* buffer, uint64_t& reference);
	/* build an order replace message with given character buffer */
	Message* buildOR(char* buffer, uint64_t& oldRef);
	/* build an trade message(including cross trade) with given character buffer */
	Message* buildT(char* buffer, uint64_t& match);
	/* build a broken trade message with given character buffer */
	Message* buildBT(char* buffer, uint64_t& match);
	/* build a message with given character buffer, and return the message to calculator object */
	Message* parseMessage(uint64_t& reference, uint64_t& match);
	~Parser();

	/* this class is only used for calculator object */
	friend class VWAPCalculator;
};

