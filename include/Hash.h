#pragma once
#include <string>
#include "Product.h"
#include "LinkedList.h"
#include "TArray.h"

class Hash {
	int size;
	int arrSize;
	TArray<LinkedList<Product*, CompareName, EqualityName>> arr;
	int collision = 0;
	int insertedKeys = 0;

public:
	Hash(int s = 0);
	~Hash() {}

	int getSize() const { return size; }
	int getASize() const { return arrSize; }
	int getCollision() const { return collision; }
	int getInsertedKeys() const { return insertedKeys; }
	float getLoadFactor() const { return float(insertedKeys) / arrSize; }
	int getLongestLL();
	float getAverageLL();
	TArray<LinkedList<Product*, CompareName, EqualityName>> getTArrayList() { return arr; }
	LinkedList<Product*, CompareName, EqualityName> getLinkedList(int i) { return arr[i]; }
	TArray<Product*> getTArrayProduct(int);
	TArray<Product*> getAllProducts();
	
	// Creating the Table
	int getPrime(int);
	int hash(std::string);
	void input(Product*, std::string);

	// Table functions
	bool compare(Product*, std::string);
	Product* find(std::string);
	void erase(std::string);

	// Operators
	LinkedList<Product*, CompareName, EqualityName> operator [](int i) { return arr[i]; }

	// Ostreams
	void displayFind(std::ostream&, std::string);
	void displayList(std::ostream&, int);
	void displayHash(std::ostream&);
	void displayEfficiency(std::ostream&);
	friend std::ostream& operator << (std::ostream&, Hash&);
};
