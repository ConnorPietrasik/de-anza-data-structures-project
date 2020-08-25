#include "Catalog.h"
#include "BST.h"
#include "Hash.h"
#include "Product.h"

//Destructor
Catalog::~Catalog()
{
	if (tree) delete tree;
	if (hash) delete hash;
	if (cart) delete cart;
}

//Builds the tree using the product list
void Catalog::buildTree()
{
	if (productList) {
		tree = new BST<Product*, CompareName, EqualityName, PrintName>;
		int listSize = productList->size();
		for (int i = 0; i < listSize; i++) {
			Product *p = (*productList)[i];
			tree->add(p);
		}
	}
}

//Builds the hash table using the product list
void Catalog::buildHash() {
	if (productList) {
		hash = new Hash(productList->size());

		for (auto p : *productList) {
			hash->input(p, p->getUpc());
		}
	}
}

//Removes the product from all data structures and deletes it
void Catalog::remove(Product* p) {
	tree->remove(p, [](Product* a, Product* b) { return (a == b); });
	hash->erase(p->getUpc());
	productList->erase(productList->find(p, 0, [](Product* a, Product* b) { return (a == b); }));
	delete p;
}

//Adds the product to all data structures
void Catalog::add(Product* p) {
	tree->add(p);
	hash->input(p, p->getUpc());
	productList->push_back(p);
}