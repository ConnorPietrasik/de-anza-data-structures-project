#include "Hash.h"
#include "Product.h"
#include "LinkedList.h"
#include "TArray.h"
#include <iomanip>
#include <string>

/* Constructor
	Pre:	sizeInput - integer for size
	Post:	size == sizeInput
*/
Hash::Hash(int sizeInput) {
	size = sizeInput;
	arrSize = getPrime(sizeInput);
	for (int i = 0; i < arrSize; i++) arr.push_back(LinkedList<Product*, CompareName, EqualityName>(1));
}

/* Gets the next prime number
	Pre :	num - the size of the hash table input
	Post :
	Return:	Returns an int prime
*/
int Hash::getPrime(int num) {
	bool isPrime;
	do {
		isPrime = true;
		num++;
		for (int i = 2; i <= num / 2; i++) {
			if (num % i == 0) isPrime = false;
		}
	} while (isPrime == false);
	return num;
}

/* Gets the Longest Linked List
	Pre :	
	Post :
	Return:	Returns the number of nodes in the Longest Linked List
*/
int Hash::getLongestLL() {
	int longest = 0;
	for (int i = 0; i < arrSize; i++) {	if (arr[i].count() > longest) longest = arr[i].count();	}
	return longest;
}

/* Gets the average number of Nodes in the hash table
	Pre :
	Post :
	Return:	Returns the average number of nodes
*/
float Hash::getAverageLL() {
	float average = 0;
	int nonEmpty = 0;
	for (int i = 0; i < arrSize; i++) { 
		if (arr[i].count() > 0) nonEmpty++;
		average += arr[i].count(); 
	}
	return average / nonEmpty;
}

/* Create a TArray of a specific Linked List
	Pre :	i - the key for the Hash Table
	Post :
	Return:	Returns a TArray version of the Linked List
*/
TArray<Product*> Hash::getTArrayProduct(int i) {
	TArray<Product*> obj;
	for (auto x : arr[i]) obj.push_back(x);
	return obj;
}

//Puts all the products in the hash table into a TArray and returns it
TArray<Product*> Hash::getAllProducts() {
	TArray<Product*> obj;
	for (const auto& l : arr) {
		for (const auto& x : l) obj.push_back(x);
	}
	return obj;
}

/* The Hashing function that turns characters into numbers and mod it by the arrSize
	Pre:	str - the string that will be hashed into an int
	Post:
	Return:	Returns an int total, which is key
*/
int Hash::hash(std::string str) {
	int total = 0;
	for (int i = 0; i < str.size(); i++) {
		if (i % 2) total += str[i];
	}

	return total % arrSize;
}

/* Inputs data into the hash table
	Pre:	obj - the Product pointer
			str - the string used to get the key
	Post:	The hash table holds the Person class at the key index
			collisions increases depending on if any keys collide
			insertedKeys is increased by 1
*/
void Hash::input(Product* obj, std::string str) {
	int key = hash(str);
	arr[key].add(obj);
	if (arr[key].count() == 1) {
		insertedKeys++;
	}
	else collision++;
}

/* Compares a key to the strings in the Product
	Pre:	obj - a Product pointer
			str - the string being searched
	Post:
	Return:	bool of whether the key fits with the strings
*/
bool Hash::compare(Product*obj, std::string str) {
	if (obj->getName() == str) return true;
	if (obj->getUpc() == str) return true;
	if (obj->getSku() == str) return true;
	return false;
}

/* Finds location of the Product
	Pre:	str - the birthday string
	Post:
	Return:	Returns Product pointer
*/
Product* Hash::find(std::string str) {
	int key = hash(str);
	for (auto x : arr[key]) {
		if (compare(x, str)) return x;
	}
	return nullptr;
}

/* Erases a Node in the hash table
	Pre :	str - a string used to find the Node
	Post :	The node will no longer be in the hash table
*/
void Hash::erase(std::string str) {
	int key = hash(str);
	int count = 1;
	if (find(str)) {
		if (arr[key].count() == 1) insertedKeys--;
		else collision--;
	}
	arr[key].erase(0,find(str));
}

/* Displays the location of the Product
	Pre:	out - ostream
			str - the birthday string
	Post:	out will hold the Product in question
*/
void Hash::displayFind(std::ostream& out, std::string str) {
	if(find(str)) out << *find(str); 
	else out << "Not found\n";
}

/* Prints all contents of the Linked List
	Pre:	out - ostream
			key - Prints out keys from the LinkedList
	Post:	out will hold the Linked List output
*/
void Hash::displayList(std::ostream& out, int key) {
	int count = 0;
	for (auto x : arr[key]) {
		out << "  Node " << count << ":\t" << x->getName() << "\tUPC: " << x->getUpc() << "\n";
		count++;
	}
}

/* Prints the Hash Table
	Pre:	out - ostream
	Post:	out will hold the Hash table output
*/
void Hash::displayHash(std::ostream& out) {
	out << "Hash Table:\n";
	for (int i = 0; i < arrSize; i++)
		if (arr[i].count() > 0) {
			out << "Key [" << i << "]:\n";
			displayList(out, i);
			out << "\n";
		}
}

/* Prints the Hash Table Efficiency
	Pre:	out - ostream
	Post:	out will hold the Hash table efficiency statistics
*/
void Hash::displayEfficiency(std::ostream& out) {
	out << std::setprecision(4) << "Collisions =   " << getCollision()
		<< "\nLoad Factor = " << getInsertedKeys() << " / " << getASize() << " = " << std::fixed << getLoadFactor()
		<< "\n\nAmount of Nodes Longest Linked List: " << getLongestLL()
		<< "\nAverage number of Nodes in linked lists: " << std::setprecision(2) << std::fixed << getAverageLL() << "\n";
}

/* Overloading the display function
	Pre:	out - an ostream for displaying hash table
			obj - Hash class object
	Post:	out will now hold the information of the hash table
*/
std::ostream& operator << (std::ostream& out, Hash& obj) {
	obj.displayHash(out);
	return out;
}



