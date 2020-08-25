#pragma once
#include<string>

class Product {
	std::string name, desc, upc, sku, brand, model, deptName, subDept;
	float price;
	int stock, wareQuant; // Warehouse Quantity

public:
	Product(const std::string& n = "", const std::string& d = "", const std::string& upc = "",
		const std::string& sku = "", const std::string& bra = "", const std::string& mod = "",
		const std::string& dpt = "", const std::string& subdpt = "", float p = 0,
		int s = 0, int ware = 0) : name(n), desc(d), upc(upc), sku(sku), brand(bra), model(mod), deptName(dpt), subDept(subdpt), price(p), stock(s), wareQuant(ware) {}


	const std::string& getName() const { return name; }
	const std::string& getDesc() const { return desc; }
	const std::string& getUpc() const { return upc; }
	const std::string& getSku() const { return sku; }
	const std::string& getBrand() const { return brand; }
	const std::string& getModel() const { return model; }
	const std::string& getDept() const { return deptName; }
	const std::string& getSubDept() const { return subDept; }
	float getPrice() const { return price; }
	int getStock() const { return stock; }
	int getWare() const { return wareQuant; }

	void sell(int x = 1);
	void receive(int x = 1) { stock += x; }
	void restock(int = 1);   // takes out amount from warehouse and put it into stock  *prob use when stock reaches zero
	void order();

	operator bool() { return (name != ""); }
	bool operator==(const Product& obj);
	bool operator>=(const Product& obj) { return (name >= obj.name); }
	friend std::ostream& operator << (std::ostream& cout, Product& obj);
};

class CompareName {
public:
	bool operator()(Product* obj1, Product* obj2) {
		return (obj1->getName() >= obj2->getName());
	}
};

class EqualityName {
public:
	bool operator()(Product* obj1, Product* obj2) {
		return (obj1->getName() == obj2->getName());
	}
};

class PrintName {
public:
	std::string operator()(Product* obj) {
		return obj->getName();
	}
};
