#pragma once

template<typename T>
class LinkedNode {
	T data;
	LinkedNode* next;
	LinkedNode* prev;

public:
	LinkedNode();
	LinkedNode(const T&);
	T& getData();
	void setData(const T&);
	LinkedNode<T>* getNext();
	void setNext(LinkedNode*);
	LinkedNode<T>* getPrev();
	void setPrev(LinkedNode*);
	void destroyAllAfter();
};

/* Initializes the node to blank
	Pre:	
	Post:	node is empty
*/
template<typename T>
LinkedNode<T>::LinkedNode() {
	data = T();
	next, prev = nullptr;
}

/* Initializes the note to obj
	Pre:	The obj to add
	Post:	data is obj
*/
template<typename T>
LinkedNode<T>::LinkedNode(const T& obj) {
	data = obj;
	next, prev = nullptr;
}

/* Get the data
	Pre:
	Post:
	Return: data
*/
template<typename T>
T& LinkedNode<T>::getData() {
	return data;
}

/* Set the data
	Pre:	x to be data
	Post:	data is x
*/
template<typename T>
void LinkedNode<T>::setData(const T& x) {
	data = x;
}

/* Get the next node
	Pre:
	Post:
	Return:	The next node
*/
template<typename T>
LinkedNode<T>* LinkedNode<T>::getNext() {
	return next;
}

/* Set the next node
	Pre:	x = node to be set
	Post:	next is x
*/
template<typename T>
void LinkedNode<T>::setNext(LinkedNode* x) {
	next = x;
}

/* Get the previous node
	Pre:
	Post:
	Return: prev
*/
template<typename T>
LinkedNode<T>* LinkedNode<T>::getPrev() {
	return prev;
}

/* Set the previous node
	Pre:	x = node pointer to be set
	Post:	prev is x
*/
template<typename T>
void LinkedNode<T>::setPrev(LinkedNode* x) {
	prev = x;
}

/* Destroys all the nodes after this one
	Pre:
	Post:	All the nodes are deleted
*/
template<typename T>
void LinkedNode<T>::destroyAllAfter() {
	if (next) {
		next->destroyAllAfter();
		delete next;
		next = nullptr;
	}
}

//An iterator so the easier for loops and stuff can be used
template<typename T>
class LinkedIterator {
	LinkedNode<T>* node;
public:
	LinkedIterator(LinkedNode<T>* = nullptr);
	T& operator*();
	void operator++();
	void operator--();
	bool operator==(const LinkedIterator<T>&);
	bool operator!=(const LinkedIterator<T>&);
};

/* Ititializes the iterator
	Pre:
	Post:	contains node pointer
*/
template<typename T>
LinkedIterator<T>::LinkedIterator(LinkedNode<T>* x) : node(x) {}

/* Derefences it
	Pre:
	Post:
	Return:	Returns the node object
*/
template<typename T>
T& LinkedIterator<T>::operator*() {
	if (node) return (node->getData());
}

/* Goes to the next node
	Pre:
	Post:	node is now the next one
*/
template<typename T>
void LinkedIterator<T>::operator++() {
	if (node) node = node->getNext();
}

/* Goes to the previous node
	Pre:
	Post:	node is now the previous one
*/
template<typename T>
void LinkedIterator<T>::operator--() {
	if (node) node = node->getPrev();
}

/* Checks for equality
	Pre:	obj = the thing to compare to
	Post:
	Return:	True if equal, false if not
*/
template<typename T>
bool LinkedIterator<T>::operator==(const LinkedIterator<T>& obj) {
	return (node == obj.node);
}

/* Checks for inequality
	Pre:	obj = the thing to compare to
	Post:
	Return:	False if equal, true if not
*/
template<typename T>
bool LinkedIterator<T>::operator!=(const LinkedIterator<T>& obj) {
	return (node != obj.node);
}


//A const version so some more things won't break
template<typename T>
class Const_LinkedIterator {
	LinkedNode<T>* node;
public:
	Const_LinkedIterator(LinkedNode<T>* = nullptr);
	const T& operator*();
	void operator++();
	void operator--();
	bool operator==(const Const_LinkedIterator<T>&);
	bool operator!=(const Const_LinkedIterator<T>&);
};

/* Ititializes the iterator
	Pre:
	Post:	contains node pointer
*/
template<typename T>
Const_LinkedIterator<T>::Const_LinkedIterator(LinkedNode<T>* x) : node(x) {}

/* Derefences it
	Pre:
	Post:
	Return:	Returns the node object
*/
template<typename T>
const T& Const_LinkedIterator<T>::operator*() {
	if (node) return (node->getData());
	else return 0;
}

/* Goes to the next node
	Pre:
	Post:	node is now the next one
*/
template<typename T>
void Const_LinkedIterator<T>::operator++() {
	if (node) node = node->getNext();
}

/* Goes to the previous node
	Pre:
	Post:	node is now the previous one
*/
template<typename T>
void Const_LinkedIterator<T>::operator--() {
	if (node) node = node->getPrev();
}

/* Checks for equality
	Pre:	obj = the thing to compare to
	Post:
	Return:	True if equal, false if not
*/
template<typename T>
bool Const_LinkedIterator<T>::operator==(const Const_LinkedIterator<T>& obj) {
	return (node == obj.node);
}

/* Checks for inequality
	Pre:	obj = the thing to compare to
	Post:
	Return:	False if equal, true if not
*/
template<typename T>
bool Const_LinkedIterator<T>::operator!=(const Const_LinkedIterator<T>& obj) {
	return (node != obj.node);
}
