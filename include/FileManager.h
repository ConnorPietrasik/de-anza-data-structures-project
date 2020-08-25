#pragma once

#include<string>
#include "Product.h"
#include "TArray.h"


class FileManager {
	TArray<Product*> pRay;
	bool worked;
	std::string path;

public:
	FileManager();
	~FileManager();

	int getSize() const { return pRay.size(); }

	bool save(std::string altPath = "");
	bool read(std::string input);
	bool hasData() { return worked; }

	Product operator[](int x) { return *pRay[x]; }

	friend std::ostream& operator <<(std::ostream&, FileManager&);

	TArray<Product*>* getProductList();
};



