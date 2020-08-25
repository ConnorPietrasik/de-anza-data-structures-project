#include "FileManager.h"
#include<iostream>
#include<fstream>
#include"Product.h"
#include"TArray.h"

//Constructor, tries the default file location
FileManager::FileManager() : path(""){
	worked = read("Fry_Database.tsv");
} 

//Destructor, deletes the products
FileManager::~FileManager() {
	for (int i = 0; i < pRay.size(); i++) delete pRay[i];
}

//Tries to read the given file, returns true on success and false on failure
bool FileManager::read(std::string input) {
	std::string name, desc, upc, sku;
	std::string brand, model, deptName, subDept;
	std::string price, stock, wareQuant;

	std::ifstream inFile(input);

	if (inFile) {
		try {
			while (!inFile.eof()) {
				std::getline(inFile, name, '\t');
				std::getline(inFile, desc, '\t');
				std::getline(inFile, upc, '\t');
				std::getline(inFile, sku, '\t');
				std::getline(inFile, brand, '\t');
				std::getline(inFile, model, '\t');
				std::getline(inFile, deptName, '\t');
				std::getline(inFile, subDept, '\t');
				std::getline(inFile, price, '\t');
				std::getline(inFile, stock, '\t');
				std::getline(inFile, wareQuant, '\n');


				float z = stof(price);
				int a = stoi(stock);
				int b = stoi(wareQuant);

				Product* obj;
				obj = new Product(name, desc, upc, sku, brand, model, deptName, subDept, z, a, b);	//*add
				pRay.push_back(obj);
			}
			if (path == "") path = input;
		}
		catch (...) {
			worked = false;
			return false;
		}
		inFile.close();
		worked = true;
		return true;
	}
	worked = false;
	return false;
}

//Saves the database to the same file as before, or returns false to get a file path
bool FileManager::save(std::string altPath) {
	if (altPath != "") path = altPath;
	std::ofstream outFile(path);
	if (outFile) {

		//Can't just do all of them because if the last one prints a newline then it adds blank spaces
		int size = pRay.size();
		if (size) {
			for (int i = 0; i < size - 1; i++) {
				outFile << pRay[i]->getName() << '\t' << pRay[i]->getDesc() << '\t' << pRay[i]->getUpc() << '\t' << pRay[i]->getSku() << '\t' << pRay[i]->getBrand() << '\t' << pRay[i]->getModel() << '\t' << pRay[i]->getDept() << '\t' << pRay[i]->getSubDept() << '\t' << pRay[i]->getPrice() << '\t' << pRay[i]->getStock() << '\t' << pRay[i]->getWare() << '\n';
			}
			outFile << pRay[size - 1]->getName() << '\t' << pRay[size - 1]->getDesc() << '\t' << pRay[size - 1]->getUpc() << '\t' << pRay[size - 1]->getSku() << '\t' << pRay[size - 1]->getBrand() << '\t' << pRay[size - 1]->getModel() << '\t' << pRay[size - 1]->getDept() << '\t' << pRay[size - 1]->getSubDept() << '\t' << pRay[size - 1]->getPrice() << '\t' << pRay[size - 1]->getStock() << '\t' << pRay[size - 1]->getWare();
		}
		outFile.close();
		worked = true;
		return true;
	}
	worked = false;
	return false;
}

//Sends all the products to the ostream
std::ostream& operator <<(std::ostream& out, FileManager& obj) {
	for (int i = 0; i < obj.getSize(); i++) {
		out << "Product " << i + 1 << obj[i];
	}	return out;
}

//Returns a pointer to the product list
TArray<Product*>* FileManager::getProductList() {
	return &pRay;
}


