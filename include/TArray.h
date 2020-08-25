#pragma once
#include<string>

//Its own namespace so it doesn't conflict with the default functors in the other data structures
namespace TArr {
	//Default functors, only the ones where the thing in enable_if is true are compiled (so it works with pointers and non-pointers)
	class Greater {
	public:
		template<typename T>
		bool operator()(const T& obj1, const T& obj2, typename std::enable_if<std::is_pointer<T>::value>::type* = 0) {
			return (*obj1 > *obj2);
		}

		template<typename T>
		bool operator()(const T& obj1, const T& obj2, typename std::enable_if<!std::is_pointer<T>::value>::type* = 0) {
			return (obj1 > obj2);
		}
	};

	class Equality {
	public:
		template<typename T>
		bool operator()(const T& obj1, const T& obj2, typename std::enable_if<std::is_pointer<T>::value>::type* = 0) {
			return (*obj1 == *obj2);
		}

		template<typename T>
		bool operator()(const T& obj1, const T& obj2, typename std::enable_if<!std::is_pointer<T>::value>::type* = 0) {
			return (obj1 == obj2);
		}
	};

	class PrintValue {
	public:
		template<typename T>
		typename std::remove_pointer<T>::type operator()(const T& obj, typename std::enable_if<std::is_pointer<T>::value>::type* = 0) {
			return *obj;
		}

		template<typename T>
		T operator()(const T& obj, typename std::enable_if<!std::is_pointer<T>::value>::type* = 0) {
			return obj;
		}
	};
}

template<typename T, class C = TArr::Greater, class E = TArr::Equality, class P = TArr::PrintValue>
class TArray {
	C comp;
	E equal;

	T* arr;
	int arrSize;
	T* pEnd;
	void indexCheck(int) const;
	int partition_low(T*, int, int);
	int partition_high(T*, int, int);
	void quicksort(T*, int, int, bool);
public:
	TArray();
	TArray(int);
	TArray(const TArray&);
	~TArray();
	TArray& operator= (const TArray&);

	int size() const;
	int getCreatedSize() const;
	T* begin();
	T* end();
	void push_back(const T&);
	void pop_back();
	void push_front(const T&);
	void pop_front();
	void clear();

	template<typename ST, class SearchEquality = E>
	int find(const ST&, int = 0, SearchEquality = SearchEquality());
	void erase(int);
	void swap(int, int);
	void resize(int);
	void shrink_to_fit();
	void sort(bool = 0);
	void reverse();
	void replace(const T&, const T&);

	T& operator[](int);
	const T& operator[](int) const;
};

/* Makes sure an index is valid, and throws an exception if it isn't
	Pre:	x – index for the array
	Post:   If x is out of bounds, throw an exception saying it's out of bounds
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::indexCheck(int x) const {
	arr;
	if (x >= arrSize || x < 0) {
		std::string s = "Index out of bounds, 0-";
		s += (arrSize - 1);
		s += " okay.";
		throw s;
	}
}

/* Puts arr[high] in the right place and puts all lesser to the left, greater to the right
	Pre:	arr – the non-empty array of integers, strings, or Dollar objects
			low - the lower limit for the j index and i index
			high - index for the arr array
	Post:
	Return: returns the index i
*/
template<typename T, class C, class E, class P>
int TArray<T, C, E, P>::partition_low(T* arr, int low, int high) {
	int i = low - 1;
	for (int j = low; j <= high - 1; j++) {
		if (comp(arr[j], arr[high])) {
			swap(++i, j);
		}
	}
	swap(++i, high);
	return i;
}

/* Puts arr[high] in the right place and puts all lesser to the left, lesser to the right
	Pre:	arr – the non-empty array of integers, strings, or Dollar objects
			low - the lower limit for the j index and i index
			high - index for the arr array
	Post:
	Return: returns the index i
*/
template<typename T, class C, class E, class P>
int TArray<T, C, E, P>::partition_high(T* arr, int low, int high) {
	int i = low - 1;
	for (int j = low; j <= high - 1; j++) {
		if (!comp(arr[j], arr[high])) {
			swap(++i, j);
		}
	}
	swap(++i, high);
	return i;
}

/* Quicksort
	Pre:	arr – the non-empty array of integers, strings, or Dollar objects
			low - lower index for the arr array
			high - higher index for the arr array
			highToLow - bool indicating sort direction
	Post:	The arr array gets recursively sorted in the specified direction
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::quicksort(T* arr, int low, int high, bool highToLow) {
	if (low < high) {
		int i;
		if (highToLow) i = partition_high(arr, low, high);
		else i = partition_low(arr, low, high);
		quicksort(arr, low, i - 1, highToLow);
		quicksort(arr, i + 1, high, highToLow);
	}
}

/*Initializes the array of T objects
	Pre:
	Post: The array is initialized as size 1
*/
template<typename T, class C, class E, class P>
TArray<T, C, E, P>::TArray() {
	arrSize = 1;
	arr = new T[1];
	arr[0] = T();
	pEnd = arr;
}

/*Initializes the array of T objects to arrSize
	Pre: x - an int which is to be the array size
	Post: The array is initialized to that size of blank values
*/
template<typename T, class C, class E, class P>
TArray<T, C, E, P>::TArray(int x) {
	if (x <= 0) {
		std::string s = "Cannot initialize TArray with 0 or less size";
		throw s;
	}
	arrSize = x;
	arr = new T[x];
	for (int i = 0; i < arrSize; i++) arr[i] = T();
	pEnd = arr + arrSize;
}

/*Copies the values in obj to this TArray
	Pre: obj - a reference to the TArray that is to be copied
	Post: The array is initialized as a copy of obj
*/
template<typename T, class C, class E, class P>
TArray<T, C, E, P>::TArray(const TArray& obj) {
	arrSize = obj.arrSize;
	arr = new T[arrSize];
	pEnd = arr + obj.size();
	for (int i = 0; i < obj.size(); i++) arr[i] = obj[i];
}

//Assignment operator, first checks to make sure it's not assigning to itself and then basically does the copy constructor
template<typename T, class C, class E, class P>
TArray<T, C, E, P>& TArray<T, C, E, P>::operator= (const TArray& obj) {
	if (this == &obj) return *this;
	delete[] arr;
	arrSize = obj.arrSize;
	arr = new T[arrSize];
	pEnd = arr + obj.size();
	for (int i = 0; i < obj.size(); i++) arr[i] = obj[i];
	return *this;
}

/*Deletes the array
	Pre:
	Post: the array is deleted
*/
template<typename T, class C, class E, class P>
TArray<T, C, E, P>::~TArray() {
	delete[] arr;
}

/* Get the current TArray used size
	Pre:
	Post:
	Return: The integer size
*/
template<typename T, class C, class E, class P>
int TArray<T, C, E, P>::size() const {
	return (pEnd - arr);
}

/* Get the current size of the created array
	Pre:
	Post:
	Return: The integer size
*/
template<typename T, class C, class E, class P>
int TArray<T, C, E, P>::getCreatedSize() const {
	return arrSize;
}

/* Returns a pointer to the beginning of the array
	Pre:
	Post:
	Return: The T pointer
*/
template<typename T, class C, class E, class P>
T* TArray<T, C, E, P>::begin() {
	return arr;
}

/* Returns a pointer to the end of the array (one after the last value)
	Pre:
	Post:
	Return: The T pointer
*/
template<typename T, class C, class E, class P>
T* TArray<T, C, E, P>::end() {
	return pEnd;
}

/* Adds the obj to the array at the end, and doubles the size of the array if need be
	Pre: obj - the T object
	Post: The arr array will hold one more T object and has double the array size if it was full
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::push_back(const T& obj) {
	if (size() >= arrSize - 1) {
		int oldSize = arrSize;
		arrSize *= 2;
		T* newArr = new T[arrSize];
		for (int i = 0; i < oldSize; i++) newArr[i] = arr[i];
		delete[] arr;
		arr = newArr;
		pEnd = arr + oldSize - 1;
	}
	*(pEnd++) = obj;
}

/* Removes the last object
	Pre:
	Post:	The arr array will hold 1 less T object
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::pop_back() {
	if (size() == 0) {
		std::string s = "Cannot pop empty TArray";
		throw s;
	}
	*(--pEnd) = T();
}

/*Doubles the array if need be, shifts the array over by one and puts obj in the first space
	Pre:	obj - the object to be put in the array
	Post:	The array is now shifted by one (and doubled if need be), and obj is in the first spot
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::push_front(const T& obj) {
	if (size() >= arrSize - 1) {
		int oldSize = arrSize;
		arrSize *= 2;
		T* newArr = new T[arrSize];
		for (int i = 0; i < oldSize; i++) newArr[i] = arr[i];
		delete[] arr;
		arr = newArr;
		pEnd = arr + oldSize - 1;
	}
	for (int i = size(); i > 0; i--) {
		arr[i] = arr[i - 1];
	}
	pEnd++;
	arr[0] = obj;
}

/* Removes the first object
	Pre:
	Post:	The first object was erased
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::pop_front() {
	erase(0);
}

/* Clears the TArray
	Pre:
	Post:	The arr array will be an default initialized array
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::clear() {
	for (int i = 0; i < size(); i++) arr[i] = T();
	pEnd = arr + 1;
}

/* Finds the first index where the value matches obj, or returns -1
	Pre:	obj - the T object
			x - lower limit of the integer index
	Post:
	Return: returns the index of the object, or -1 if unsuccessful
*/
template<typename T, class C, class E, class P>
template<typename ST, class SearchEquality>
int TArray<T, C, E, P>::find(const ST& obj, int x, SearchEquality sEqual) {
	indexCheck(x);
	for (int i = x; i < size(); i++) if (sEqual(arr[i], obj)) return i;
	return -1;
}

/* Erases the value stored at x and moves any after to fill the gap
	Pre:	x - integer index
	Post:	delete arr[x] and move every object after it up the array by 1
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::erase(int x) {
	indexCheck(x);
	for (int i = x; i < size() - 1; i++) arr[i] = arr[i + 1];
	pop_back();
}

/* Swaps the values at index a and index b
	Pre:	a - integer index
			b - integer index
	Post:	two of the objects will be swapped in position
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::swap(int a, int b) {
	indexCheck(a);
	indexCheck(b);
	T temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

/* Allows the TArray object to be used like an array
	Pre:	x - integer index
	Post:
	Return: return the object at position x
*/
template<typename T, class C, class E, class P>
T& TArray<T, C, E, P>::operator[](int x) {
	indexCheck(x);
	return arr[x];
}

//Just a const version of above
template<typename T, class C, class E, class P>
const T& TArray<T, C, E, P>::operator[](int x) const {
	indexCheck(x);
	return arr[x];
}

/* Resizes arr to size x
	Pre:	x - integer size
	Post:   the arr array will hold a new array with a different size
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::resize(int x) {
	if (x <= 0) {
		std::string s = "Cannot set TArray size to 0 or less";
		throw s;
	}
	T* newArr = new T[x];
	for (int i = 0; i < x && i < arrSize; i++) newArr[i] = arr[i];
	arrSize = x;
	delete[] arr;
	arr = newArr;
	pEnd = arr + x;
}

/* Shrinks the array to fit only the pushed objects
	Pre:
	Post:   the arr array will hold a new array with only the pushed objects
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::shrink_to_fit() {
	int x = size();
	T* newArr = new T[x];
	for (int i = 0; i < x; i++) newArr[i] = arr[i];
	arrSize = x;
	delete[] arr;
	arr = newArr;
	pEnd = arr + x;
}

/* Quicksorts the array in the specified direction, 0 is min->max and default
	Pre:	hightoLow – a boolean saying if the sorting will be from high to low
	Post:   the arr array will be sorted from low to high or high to low
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::sort(bool highToLow) {
	quicksort(arr, 0, size() - 1, highToLow);
}

/* Reverses the order of the elements in the array
	Pre:
	Post:	The array is reversed (only pushed elements are affected
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::reverse() {
	T* newArr = new T[arrSize];
	for (int i = 0; i < size(); i++) newArr[i] = arr[size() - i];
	delete[] arr;
	arr = newArr;
	pEnd = arr + size();
}

/* Replaces all instances of obj1 with obj2
	Pre:	obj1 - the object to be replaced
			obj2 - the replacement object
	Post:	All instances of obj1 have been replaced with obj2
*/
template<typename T, class C, class E, class P>
void TArray<T, C, E, P>::replace(const T& obj1, const T& obj2) {
	int i = find(obj1);
	while (i != -1) {
		arr[i] = obj2;
		i = find(obj1, i + 1);
	}
}


/* Overloaded << operator to easily print the array
	Pre:	out – an ostream that will output information about the TArray<T, C, E, P> object
			obj - the TArray<T, C, E, P> object
	Post:   the array will have been sent to the ostream
	Return: returns the out ostream
*/
template<typename T, class C, class E, class P>
std::ostream& operator<<(std::ostream& out, TArray<T, C, E, P>& obj) {
	P val;
	for (int i = 0; i < obj.size() - 1; i++) {
		out << "   " << val(obj[i]) << '\n';
	}
	if (obj.size()) out << "   " << val(obj[obj.size() - 1]);
	else out << "TArray is empty.\n";
	return out;
}

