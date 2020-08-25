#pragma once
#include "LinkedNode.h"
#include <string>

//Its own namespace so it doesn't conflict with the default functors in the other data structures
namespace List {
	//Default functors, only the ones where the thing in enable_if is true are compiled (so it works with pointers and non-pointers)
	class Greater {
	public:
		template<typename T>
		bool operator()(const T& obj1, const T& obj2, typename std::enable_if<std::is_pointer<T>::value>::type* = 0) {
			if (obj1 && obj2) return (*obj1 > * obj2);
			else return false;
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
			if (obj1 && obj2) return (*obj1 == * obj2);
			else return false;
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

template<typename T, class C = List::Greater, class E = List::Equality, class P = List::PrintValue>
class LinkedList {
protected:
	int counter;
	LinkedNode<T> *head;
	LinkedNode<T> *tail;
	int sort;
	C comp;
	E equal;

public:
	LinkedList(int = 0);
	~LinkedList();

	void add(T, int = 0);
	void add(LinkedNode<T>*, int = 0);
	void add(LinkedNode<T>, int = 0);
	bool erase(int = 0, T = T());
	int find(T, int = 0);

	int count();
	void empty();

	LinkedNode<T>* getHead();
	LinkedNode<T>* getTail();

	LinkedIterator<T> begin();
	LinkedIterator<T> end();

	Const_LinkedIterator<T> begin() const;
	Const_LinkedIterator<T> end() const;
};

/* Constructor of a LinkedList object with a sort variable
	Pre:	sort - int saying whether to sort
					 
	Post:   head and tail point to null
			sort has a value
				0 - Unordered (default)
				1 - sorted
*/
template<typename T, class C, class E, class P>
LinkedList<T, C, E, P>::LinkedList(int sortDir) {
	counter = 0;
	head = nullptr;
	tail = nullptr;
	sort = sortDir;
}

/* Destructor of a LinkedList object
	Pre:	
	Post:   empties the list
*/
template<typename T, class C, class E, class P>
LinkedList<T, C, E, P>::~LinkedList() {
	empty();
}


/* Adds an object
	Pre:	data - an object
			x - the position, default 0
	Post:   data is in the list
*/
template<typename T, class C, class E, class P>
void LinkedList<T, C, E, P>::add(T data, int x) {
	LinkedNode<T>* newPtr = new LinkedNode<T>(data);
	LinkedNode<T>* prevPtr = nullptr;
	LinkedNode<T>* currPtr = head;
	if (counter == 0) {
		head = newPtr;
		tail = newPtr;
	}
	else if (sort == 1) {
		while (currPtr != tail && comp(data, currPtr->getData())) {
			prevPtr = currPtr;
			currPtr = currPtr->getNext();
		}
		if (currPtr == tail && comp(data, currPtr->getData())) {
			currPtr->setNext(newPtr);
			tail = newPtr;
		}
		else {
			if (!prevPtr) {
				newPtr->setNext(head);
				head = newPtr;
			}
			else {
				prevPtr->setNext(newPtr);
				newPtr->setNext(currPtr);
			}
		}
	}
	else if (sort == 0) {
		for (int i = 0; i < x; i++) {
			prevPtr = currPtr;
			if (currPtr) currPtr = currPtr->getNext();
		}
		if (!prevPtr) {
			newPtr->setNext(head);
			head = newPtr;
		}
		else {
			newPtr->setNext(currPtr);
			prevPtr->setNext(newPtr);
			if (!currPtr) tail = newPtr;
		}
	}
	counter++;
}


/* Adds a LinkedNode pointer
	Pre:	data - a node
			x - the position, default 0
	Post:   new node is in the list
*/
template<typename T, class C, class E, class P>
void LinkedList<T, C, E, P>::add(LinkedNode<T>* data, int x) {
	LinkedNode<T>* newPtr = data;
	LinkedNode<T>* prevPtr = nullptr;
	LinkedNode<T>* currPtr = head;
	if (counter == 0) {
		head = newPtr;
		tail = newPtr;
	}
	else if (sort == 1) {
		while (currPtr != tail && comp(data->getData(), currPtr->getData())) {
			prevPtr = currPtr;
			currPtr = currPtr->getNext();
		}
		if (currPtr == tail && comp(data->getData(), currPtr->getData())) {
			currPtr->setNext(newPtr);
			tail = newPtr;
		}
		else {
			if (!prevPtr) {
				newPtr->setNext(head);
				head = newPtr;
			}
			else {
				prevPtr->setNext(newPtr);
				newPtr->setNext(currPtr);
			}
		}
	}
	else if (sort == 0) {
		for (int i = 0; i < x; i++) {
			prevPtr = currPtr;
			if (currPtr) currPtr = currPtr->getNext();
		}
		if (!prevPtr) {
			newPtr->setNext(head);
			head = newPtr;
		}
		else {
			newPtr->setNext(currPtr);
			prevPtr->setNext(newPtr);
			if (!currPtr) tail = newPtr;
		}
	}
	counter++;
}

/* Adds a LinkedNode object
	Pre:	data - a node
			x - the position, default 0
	Post:   new node is in the list
*/
template<typename T, class C, class E, class P>
void LinkedList<T, C, E, P>::add(LinkedNode<T> data, int x) {
	LinkedNode<T>* newPtr = new LinkedNode<T>(data.getData());
	LinkedNode<T>* prevPtr = nullptr;
	LinkedNode<T>* currPtr = head;
	if (counter == 0) {
		head = newPtr;
		tail = newPtr;
	}
	else if (sort == 1) {
		while (currPtr != tail && comp(data.getData(), currPtr->getData())) {
			prevPtr = currPtr;
			currPtr = currPtr->getNext();
		}
		if (currPtr == tail && comp(data.getData(), currPtr->getData())) {
			currPtr->setNext(newPtr);
			tail = newPtr;
		}
		else {
			if (!prevPtr) {
				newPtr->setNext(head);
				head = newPtr;
			}
			else {
				prevPtr->setNext(newPtr);
				newPtr->setNext(currPtr);
			}
		}
	}
	else if (sort == 0) {
		for (int i = 0; i < x; i++) {
			prevPtr = currPtr;
			if (currPtr) currPtr = currPtr->getNext();
		}
		if (!prevPtr) {
			newPtr->setNext(head);
			head = newPtr;
		}
		else {
			newPtr->setNext(currPtr);
			prevPtr->setNext(newPtr);
			if (!currPtr) tail = newPtr;
		}
	}
	counter++;
}

/* Erases a LinkedNode containing an object or at position
Pre:	data - an object
		index - position to be deleted
Post:   LinkedNode containing the data will no longer be in the list
		The previous node will point to the node after the deleted node
*/
template<typename T, class C, class E, class P>
bool LinkedList<T, C, E, P>::erase(int index, T data) {
	LinkedNode<T>* prevPtr = nullptr;
	LinkedNode<T>* currPtr = head;

	if (index == 0) {
		currPtr = head->getNext();
		delete head;
		head = currPtr;
		counter--;
		return true;
	}

	if (equal(data, T())) {
		for (int i = 0; i < index; i++) {
			prevPtr = currPtr;
			if (currPtr) currPtr = currPtr->getNext();
		}
		if (!prevPtr) {
			currPtr = head->getNext();
			delete head;
			head = currPtr;
			counter--;
			return true;
		}
		else {
			LinkedNode<T>* temp = currPtr->getNext();
			delete currPtr;
			prevPtr->setNext(temp);
			counter--;
			return true;
		}
	}
	else {
		int pos = find(data, index);
		if (pos != -1) {
			if (pos == 0) {
				currPtr = head->getNext();
				delete head;
				head = currPtr;
				counter--;
				return true;
			}
			for (int i = 0; i < pos; i++) {
				prevPtr = currPtr;
				currPtr = currPtr->getNext();
			}
			LinkedNode<T>* temp = currPtr->getNext();
			delete currPtr;
			prevPtr->setNext(temp);
			counter--;
			return true;
		}
		return false;
	}
}

/* Finds a LinkedNode in an LinkedList object
Pre:	data - an int, string, or Ruppee
Post:	Prints out on which nodes the data is in
*/
template<typename T, class C, class E, class P>
int LinkedList<T, C, E, P>::find(T data, int index) {
	int nodeCounter = 0;
	bool dataFound = false;

	LinkedNode<T> *currPtr;
	currPtr = head;

	for (int i = 0; i < index; i++) if(currPtr) currPtr = currPtr->getNext();

	while (currPtr && !dataFound) {
		if (equal(data, currPtr->getData())) {
			dataFound = true;
		}
		else {
			currPtr = currPtr->getNext();
			nodeCounter++;
		}
	}
	if (dataFound) {
		return nodeCounter;
	}
	else {
		return -1;
	}
}

/* Returns the amount of LinkedNodes in the List
	Pre:	
	Post:   
	Return: An integer count
*/
template<typename T, class C, class E, class P>
int LinkedList<T, C, E, P>::count() {
	return counter;
}

/* Empties all the LinkedNodes from the List
	Pre:
	Post: Link list is deleted
*/
template<typename T, class C, class E, class P>
void LinkedList<T, C, E, P>::empty() {
	if (head) {
		head->destroyAllAfter();
		delete head;
	}
	head = nullptr;
	tail = nullptr;
	counter = 0;
}

/* Returns the LinkedList head pointer
	Pre:
	Post:
	Return: The LinkedNode head
*/
template<typename T, class C, class E, class P>
LinkedNode<T> * LinkedList<T, C, E, P>::getHead() {
	return head;	
}

/* Returns the LinkedList tail pointer
Pre:
Post:
Return: The LinkedNode tail
*/
template<typename T, class C, class E, class P>
LinkedNode<T> * LinkedList<T, C, E, P>::getTail() {
	return tail;
}

/* Returns an iterator to the beginning
	Pre:	
	Post:	
	Return: An iterator from the head
*/
template<typename T, class C, class E, class P>
LinkedIterator<T> LinkedList<T, C, E, P>::begin() {
	return LinkedIterator<T>(head);
}

/* Returns an iterator to nullptr because that's how the easy for loops like it (remember not the same as getTail)
	Pre:
	Post:
	Return:	An iterator to nullptr
*/
template<typename T, class C, class E, class P>
LinkedIterator<T> LinkedList<T, C, E, P>::end() {
	return LinkedIterator<T>(nullptr);
}

/* Returns an iterator to the beginning
	Pre:
	Post:
	Return: An iterator from the head
*/
template<typename T, class C, class E, class P>
Const_LinkedIterator<T> LinkedList<T, C, E, P>::begin() const {
	return Const_LinkedIterator<T>(head);
}

/* Returns an iterator to nullptr because that's how the easy for loops like it (remember not the same as getTail)
	Pre:
	Post:
	Return:	An iterator to nullptr
*/
template<typename T, class C, class E, class P>
Const_LinkedIterator<T> LinkedList<T, C, E, P>::end() const {
	return Const_LinkedIterator<T>(nullptr);
}

/*	Prints the linked list
	Pre:	out - an ostream
			obj - The LinkedlList object
	Post:
	Return: Returns the ostream
*/
template<typename T, class C, class E, class P>
std::ostream& operator<<(std::ostream& out, LinkedList<T, C, E, P>& obj) {
	P pr;
	LinkedNode<T>* currPtr = obj.getHead();

	for (int i = 0; currPtr; i++) {
		out << "Node " << i << ":\t"<< pr(currPtr->getData()) << "\n";
		currPtr = currPtr->getNext();
	}

	return out;
}