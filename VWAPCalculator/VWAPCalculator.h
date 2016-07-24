#pragma once

#include <unordered_map>
#include "Parser.h"
#include <iomanip>
using namespace std;

/* struct of total shares and values for a stock */
struct QnT {
	uint64_t quantity; // total shares of executions or trades of a stock
	uint64_t total; // total values of executions or trades of a stock, using uint64_t to preserve accuracy
	QnT() { quantity = 0; total = 0; }
	QnT(uint64_t q, uint64_t t) { quantity = q; total = t; }
};

class VWAPCalculator {
	unordered_map<string, QnT> stockQnT; // stock, and its executed quantity and total. 
	unordered_map<uint64_t, Message*> referToOrder; // mapping from reference number to its order message.
	unordered_map<uint64_t, Message*> matchToExecution; // mapping from match number to previous exection or trade message.
	Parser* parser;

	/* utility method to add share and value to stock */
	inline void addQnT(TMessage* tm);
	/* utility method to sub share and value to stock */
	inline void subQnT(TMessage* tm);
	/* action for stock directory message, initialize stock map */
	void SDAction(Message* message);
	/* action for add order message, add order to order map */
	void AOAction(Message* message, uint64_t& reference);
	/* action for order execetued message, add shares and values to stock map */
	void OEAction(Message* message, uint64_t& reference, uint64_t& match);
	/* action for order execetued with price message, add shares and values to stock map */
	void OEWPAction(Message* message, uint64_t& reference, uint64_t& match);
	/*  action for order cancelation with order reference */
	void OCAction(Message* message, uint64_t& reference);
	/* action for order delete message, remove add order message from map */
	void ODAction(Message* message, uint64_t& reference);
	/* action for order replace message, replace old add order message with updated info */
	void ORAction(Message* message, uint64_t& oldRef);
	/* action for trade and cross trade message */
	void TAction(Message* message, uint64_t& match);
	/* action for broken trade message, void previous executed order */
	void BTAction(Message* message, uint64_t& match);
	/* assign parsed message to appropriate map for calculation */
	void loadMessages();
public:

	/* passing in your data file name to this contructor */
	VWAPCalculator(char* file);
	/* public method to print out VWAP for each stock, calculations are done in this method */
	void printVWAPs();
};