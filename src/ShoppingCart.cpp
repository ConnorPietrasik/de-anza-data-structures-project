#include "ShoppingCart.h"
#include "TArray.h"
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>

/* Adds a product into the Shopping cart
	Pre:	obj - the Product Pointer
			x - the amount of objects being added
	Post:	Product Pointer will be added into the cart TArray
*/
void ShoppingCart::add(Product* obj, int x) {
	for (int i = 0; i < x; i++) {
		cart.push_back(obj);
		cost += obj->getPrice();
	}
}

/* Removes a product into the Shopping cart
	Pre:	obj - the Product Pointer
	Post:	Product Pointer will be removed from the cart TArray
*/
void ShoppingCart::remove(Product* obj) {
	int count = 0;
	int found = cart.find(obj, 0, [](Product* a, Product* b) {return (a == b); });
	if (found != -1) {
		cart.erase(found);
		cost -= obj->getPrice();
	}
}

/* Turn floats into a 2 decimal string
	Pre:	num - the float
	Post:	
	Return:	A string that holds the float in two decimals
*/
std::string ShoppingCart::ftos(float num) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << num;
	return stream.str();
}

/* Turn ints into a 2 decimal string
	Pre:	num - the int
	Post:
	Return:	A string that holds the int in two decimals
*/
std::string ShoppingCart::itos(int num) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << num;
	return stream.str();
}

/* Formats the Product in the reciept
	Pre:	obj - the Product Pointer
	Post:
	Return:	A formatted string of the Product
*/
std::string ShoppingCart::prod(Product* obj) {
	std::stringstream stream;
	std::string name;
	if (obj->getName().size() > 20) name = obj->getName().substr(0, 16) + "...";
	else name = obj->getName();
	stream << "\t" << std::fixed << std::setw(20) << name
		   << "\t" << ftos(obj->getPrice()) + "\n";
	return stream.str();
}

/* Prints out the reciept of the cart to ostream and to a file
	Pre:	
	Post:	"Reciept.txt" holds the reciept
	Return:	A string that holds reciept
*/
std::string ShoppingCart::reciept() {
	std::ofstream outFile("Receipt.txt");
	std::string str;
	
	str = "\t\tFRY'S\n\t\tElectronics\n\n";
	str += "Store #:  1 Reg:  1 Nguyen, Brandon P.\n";
	str += "21250 Stevens Creek Blvd\nCupertino, CA 95014\n\n";
	str += "|||||||||||||||||||||||||||||||||||||||||\n";

	for (auto x : cart) {	
		str += prod(x);
	}
	str += "\n\t\t  SUBTOTAL:\t" + ftos(cost) + "\n";
	str += "\tSALES TAX @ 8.625%:\t" + ftos(cost * 0.08625) + "\n";
	str += "\t\t TOTAL DUE:\t" + ftos(cost * 1.08625) + "\n\n";
	str += "\nITEM COUNT  " + itos(cart.size()) + "\n\n";

	outFile << str;
	outFile.close();	
	return str;
}

