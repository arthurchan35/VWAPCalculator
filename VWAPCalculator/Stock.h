#pragma once
#include <string>

using namespace std;
/* stock interface, only for easy purpose for this problem */
class Stock {
	string stock;
public:
	/* relevent contructor to this problem */
	Stock(string stock){
		this->stock = stock;
	}

	string getStock() { return stock; }
	void setStock(string stock) { this->stock = stock; };
	~Stock() {}
};

