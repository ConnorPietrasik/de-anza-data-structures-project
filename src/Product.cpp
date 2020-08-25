#include "Product.h"
#include<iomanip>

//Sells the product, and updates the stock if it runs out (warehouse can be negative, it's okay)
void Product::sell(int x) {
	stock -= x;
	if (stock < 1) {
		stock += 5;
		wareQuant -= 5;
	}
}

//Checks to make sure the products are equal
bool Product::operator==(const Product& obj) {
	return (name == obj.getName() && desc == obj.getDesc() && sku == obj.getSku() && model == obj.getModel() && upc == obj.getUpc());
}

//Sends the product to the ostream
std::ostream& operator<<(std::ostream& out, Product& obj) {
	out.precision(2);
	out << "\n Name:     " << obj.getName()
		<< "\n Desc:     " << obj.getDesc()
		<< "\n UPC:      " << obj.getUpc()
		<< "\n SKU:      " << obj.getSku()
		<< "\n Brand:    " << obj.getBrand()
		<< "\n Model:    " << obj.getModel()
		<< "\n Dept:     " << obj.getDept()
		<< "\n SubDept:  " << obj.getSubDept()
		<< "\n Price:    $" << std::fixed << obj.getPrice()
		<< "\n Stock:    " << obj.getStock()
		<< "\n WareStk:  " << obj.getWare() << "\n\n";
	return out;
}