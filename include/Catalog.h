#pragma once

#include "Product.h"
#include "TArray.h"
#include "BST.h"
#include "Hash.h"
#include "ShoppingCart.h"
class Catalog
{
private:
	bool isEmployee = false;
	TArray<Product*>* productList = nullptr;
	BST<Product*, CompareName, EqualityName, PrintName>* tree;
	Product* product = nullptr;
	Hash* hash;
	ShoppingCart* cart;

public:
	Catalog() : tree(nullptr), hash(nullptr), cart(nullptr) {}
	~Catalog();
	void buildTree();
	void buildHash();
	void setEmployee(bool value) { isEmployee = value; };
	void setProductList(TArray<Product*>* list) { productList = list; };
	void setCart(ShoppingCart* x) { cart = x; }
	BST<Product*, CompareName, EqualityName, PrintName>* getBST() { return tree; }
	Hash* getHash() { return hash; }
	ShoppingCart* getCart() { return cart; }

	void remove(Product*);
	void add(Product*);

};



