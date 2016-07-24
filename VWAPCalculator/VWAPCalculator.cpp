// VWAPCalculator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h" // comment out this line if running on linux
#include "VWAPCalculator.h"
#include <algorithm>


VWAPCalculator::VWAPCalculator(char* file) {
	parser = new Parser(file);
}

void VWAPCalculator::printVWAPs() {
	cout << "parsing and loading messages" << endl;
	loadMessages();
	cout << setw(20) << left << "stock name" << setw(20) << left << "total shares" << setw(20) << left << "total values" << setw(20) << left << "VWAP" << endl;
	for (auto pair : stockQnT) {
		double totalTrans = (double)(pair.second.total) / 10000.0;
		cout << setw(20) << left << pair.first << setw(20) << left << pair.second.quantity << setw(20) << left << totalTrans << setw(20) << left;
		if (pair.second.quantity != 0)
			cout << totalTrans / (double)(pair.second.quantity);
		cout << endl;

	}
}

void VWAPCalculator::loadMessages() {
	uint64_t reference = 0; // message feild, but in order to save memory, make it a reference variable 
	uint64_t match = 0; // message feild, but in order to save memory, make it a reference variable 
	while (true) {
		Message* message = parser->parseMessage(reference, match);
		if (message == NULL) break;
		switch (message->getType()) {
		case 'R':
			SDAction(message);
			break;
		case 'A':
		case 'F':
			AOAction(message, reference);
			break;
		case 'E':
			OEAction(message, reference, match);
			break;
		case 'C':
			OEWPAction(message, reference, match);
			break;
		case 'D':
			ODAction(message, reference);
			break;
		case 'U':
			ORAction(message, reference);
			break;
		case 'P':
		case 'Q':
			TAction(message, match);
			break;
		case 'B':
			BTAction(message, match);
			break;
		default:
			delete message;
		}
	}
}

void VWAPCalculator::addQnT(TMessage* tm) {
	stockQnT[tm->getStock()].quantity += tm->getShares();
	stockQnT[tm->getStock()].total += tm->getShares() * (uint64_t)(tm->getPrice4());
}

void VWAPCalculator::subQnT(TMessage* tm) {
	stockQnT[tm->getStock()].quantity -= tm->getShares();
	stockQnT[tm->getStock()].total -= tm->getShares() * (uint64_t)(tm->getPrice4());
}

void VWAPCalculator::SDAction(Message * message) {
	SDMessage* sdm = static_cast<SDMessage*>(message);
	stockQnT[sdm->getStock()] = QnT(0, 0);
	delete sdm;
}

void VWAPCalculator::AOAction(Message * message, uint64_t& reference) {
	referToOrder[reference] = message;
}

void VWAPCalculator::OEAction(Message* message, uint64_t& reference, uint64_t& match) {
	TMessage* tm = static_cast<TMessage*>(message);
	AOMessage* aom = static_cast<AOMessage*>(referToOrder[reference]);
	tm->setStock(aom->getStock());
	tm->setPrice4(aom->getPrice4());
	uint64_t remain = aom->getShares() - tm->getShares();
	if (aom->getShares() == tm->getShares()) {
		delete aom;
		referToOrder.erase(reference);
	}
	else if (aom->getShares() > tm->getShares()) {
		aom->setShares(remain);
	}
	else {
		cerr << "error: In order execution: trying to remove " << tm->getShares() << 
			" shares from stock " << aom->getStock() << " with remaining of " << aom->getShares() << endl;
		cerr << "removing the rest shares anyway" << endl;
		delete aom;
		referToOrder.erase(reference);
	}
	addQnT(tm);
	matchToExecution[match] = message;
}

void VWAPCalculator::OEWPAction(Message * message, uint64_t& reference, uint64_t& match) {
	TMessage* tm = static_cast<TMessage*>(message);
	AOMessage* aom = static_cast<AOMessage*>(referToOrder[reference]);
	tm->setStock(aom->getStock());
	uint64_t remain = aom->getShares() - tm->getShares();
	if (aom->getShares() == tm->getShares()) {
		delete aom;
		referToOrder.erase(reference);
	}
	else if (aom->getShares() > tm->getShares()) {
		aom->setShares(remain);
	}
	else {
		cerr << "error: In order execution with price: trying to remove " << tm->getShares() << 
			" shares from stock " << aom->getStock() << " with remaining of " << aom->getShares() << endl;
		cerr << "removing the rest shares anyway" << endl;
		delete aom;
		referToOrder.erase(reference);
	}
	addQnT(tm);
	matchToExecution[match] = message;
}

void VWAPCalculator::OCAction(Message * message, uint64_t & reference) {
	OCMessage* ocm = static_cast<OCMessage*>(message);
	AOMessage* aom = static_cast<AOMessage*>(referToOrder[reference]);
	uint64_t remain = aom->getShares() - ocm->getShares();
	if (remain == 0) {
		delete aom;
		referToOrder.erase(reference);
	}
	else if (remain > 0) {
		aom->setShares(remain);
	}
	else {
		cerr << "error: trying to remove " << ocm->getShares() << " shares from stock " << 
			aom->getStock() << " with remaining of " << aom->getShares() << endl;
		cerr << "removing the rest shares anyway" << endl;
		delete aom;
		referToOrder.erase(reference);
	}
	delete ocm;
}

void VWAPCalculator::ODAction(Message* message, uint64_t & reference) {
	delete message;
	delete referToOrder[reference];
	referToOrder.erase(reference);
}

void VWAPCalculator::ORAction(Message * message, uint64_t& oldRef) {
	ORMessage* orm = static_cast<ORMessage*>(message);
	AOMessage* aom = static_cast<AOMessage*>(referToOrder[oldRef]);
	Message* newaom = orm->newAOM(aom->getType(), aom->getStock());
	referToOrder[orm->getReference()] = newaom;
	delete aom;
	referToOrder.erase(oldRef);
	delete orm;
}

void VWAPCalculator::TAction(Message * message, uint64_t & match) {
	TMessage* tm = static_cast<TMessage*>(message);
	addQnT(tm);
	matchToExecution[match] = message;
}

void VWAPCalculator::BTAction(Message* btMessage, uint64_t & match) {
	auto iter = matchToExecution.find(match);
	if (iter == matchToExecution.end()) return;
	TMessage* tm = static_cast<TMessage*>(iter->second);
	subQnT(tm);
	delete tm;
	matchToExecution.erase(match);
	delete btMessage;
}



int main(int argc, char* argv[]) {
	//char* file = argv[1];
	char* file = "06022014.NASDAQ_ITCH50";
	VWAPCalculator calculator = VWAPCalculator(file);

	calculator.printVWAPs();

	getchar();
	return 0;
}
