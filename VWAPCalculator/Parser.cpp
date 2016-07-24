#include "stdafx.h" // comment out this line if running on linux
#include "Parser.h"


Parser::Parser(char* file) {
	binaryData.open(file, ios::in | ios::binary);
	buffer = new char[64];
	currentProgress = 0;
	totalProgress = 0;
}

void Parser::printProgress(uint32_t size) {
	currentProgress += size;
	if (currentProgress >= (uint32_t)PROGRESSTHRESHHOLD) {
		totalProgress += 1;
		currentProgress = 0;
		cout << totalProgress * 128 << "MB of data read so far" << endl;
	}
}

uint32_t Parser::swapEndian(uint32_t * i) {
	uint32_t res = 0;
	char* ptr1 = (char*)i;
	char* ptr2 = (char*)&res;
	ptr2[0] = ptr1[3];
	ptr2[1] = ptr1[2];
	ptr2[2] = ptr1[1];
	ptr2[3] = ptr1[0];
	return res;
}

uint64_t Parser::swapEndian(uint64_t * l) {
	uint64_t res = 0;
	char* ptr1 = (char*)l;
	char* ptr2 = (char*)&res;
	ptr2[0] = ptr1[7];
	ptr2[1] = ptr1[6];
	ptr2[2] = ptr1[5];
	ptr2[3] = ptr1[4];
	ptr2[4] = ptr1[3];
	ptr2[5] = ptr1[2];
	ptr2[6] = ptr1[1];
	ptr2[7] = ptr1[0];
	return res;
}

string Parser::getStockString(char * start) {
	string result = "";
	for (int i = 0; i < STOCKNAMESIZE; ++i) {
		if (*start == ' ') break;
		result.push_back(*start++);
	}
	return result;
}

Message * Parser::buildSD(char * buffer) {
	char type = *buffer;
	string stock = getStockString(buffer + 11); // 11 bytes offset
	return new SDMessage(type, stock);
}

Message * Parser::buildAO(char * buffer, uint64_t& reference) {
	char type = *buffer;
	reference = swapEndian((uint64_t*)(buffer + 11));
	uint64_t shares = swapEndian((uint32_t*)(buffer + 20));
	string stock = getStockString(buffer + 24);
	uint32_t price = swapEndian((uint32_t*)(buffer + 32));
	return new AOMessage(type, shares, stock, price);
}

Message * Parser::buildOE(char * buffer, uint64_t& reference, uint64_t& match) {
	char type = *buffer;
	reference = swapEndian((uint64_t*)(buffer + 11));
	uint64_t shares = swapEndian((uint32_t*)(buffer + 19));
	match = swapEndian((uint64_t*)(buffer + 23));
	return new TMessage(type, shares);
}

Message * Parser::buildOEWP(char * buffer, uint64_t & reference, uint64_t& match) {
	if (*(buffer + 31) == 'N') return new Message('\0');
	char type = *buffer;
	reference = swapEndian((uint64_t*)(buffer + 11));
	uint64_t shares = swapEndian((uint32_t*)(buffer + 19));
	match = swapEndian((uint64_t*)(buffer + 23));
	uint32_t price = swapEndian((uint32_t*)(buffer + 32));
	return new TMessage(type, shares, price);
}

Message * Parser::buildOC(char * buffer, uint64_t & reference) {
	char type = *buffer;
	reference = swapEndian((uint64_t*)(buffer + 11));
	uint64_t shares = swapEndian((uint32_t*)(buffer + 19));
	return new OCMessage(type, shares);
}

Message * Parser::buildOD(char * buffer, uint64_t & reference) {
	char type = *buffer;
	reference = swapEndian((uint64_t*)(buffer + 11));
	return new Message(type);
}

Message * Parser::buildOR(char * buffer, uint64_t& oldRef) {
	char type = *buffer;
	oldRef = swapEndian((uint64_t*)(buffer + 11));
	uint64_t newRef = swapEndian((uint64_t*)(buffer + 19));
	uint64_t shares = swapEndian((uint32_t*)(buffer + 27));
	uint32_t price = swapEndian((uint32_t*)(buffer + 31));
	return new ORMessage(type, newRef, shares, price);
}

Message * Parser::buildT(char * buffer, uint64_t & match) {
	char type = *buffer;
	uint64_t shares = (type == 'P') ? (swapEndian((uint32_t*)(buffer + 20))) : (swapEndian((uint64_t*)(buffer + 11)));
	string stock = getStockString(buffer + ((type == 'P') ? 24 : 19));
	uint32_t price = swapEndian((uint32_t*)(buffer + ((type == 'P') ? 32 : 27)));
	match = swapEndian((uint64_t*)(buffer + ((type == 'P') ? 36 : 31)));
	return new TMessage(type, shares, stock, price);
}

Message * Parser::buildBT(char * buffer, uint64_t & match) {
	char type = *buffer;
	match =swapEndian((uint64_t*)(buffer + 11));
	return new Message(type);
}

Message* Parser::parseMessage(uint64_t& reference, uint64_t& match) {
	char test = '\0';
	if (!binaryData.get(test)) return NULL;
	binaryData.get(test);
	uint32_t size = test - '\0';
	binaryData.read(buffer, size);
	printProgress(size + 2);
	switch (*buffer) {
	case 'R':
		return buildSD(buffer);
		break;
	case 'A':
	case 'F':
		return buildAO(buffer, reference);
		break;
	case 'E':
		return buildOE(buffer, reference, match);
		break;
	case 'C':
		return buildOEWP(buffer, reference, match);
		break;
	case 'D':
		return buildOD(buffer, reference);
		break;
	case 'U':
		return buildOR(buffer, reference);
		break;
	case 'P':
	case 'Q':
		return buildT(buffer, match);
		break;
	case 'B':
		return buildBT(buffer, match);
		break;
	default: {
		//cout << "skipping non-relevant message, type: " << *buffer << endl; 
		return new Message('\0');
	}
	}
}

Parser::~Parser() {
}
