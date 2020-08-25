#pragma once
#include "TArray.h"
#include "Product.h"
#include<string>

class ShoppingCart {
	float cost;
	TArray<Product*> cart;
public:
	ShoppingCart() { cost = 0; }
	
	TArray<Product*> getCart() { return cart; };

	void add(Product*, int = 1);
	void remove(Product*);
	void clear() { cart = TArray<Product*>(); cost = 0; }

	std::string ftos(float);
	std::string itos(int);
	std::string prod(Product*);
	std::string reciept();

	float getCost() { return cost; }

	void display(std::ostream& out) { out << reciept(); }
};