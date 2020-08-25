#pragma once

template<typename T>
class BSTNode {
	T data;
	BSTNode<T>* left;
	BSTNode<T>* right;
	BSTNode<T>* parent;

public:
	BSTNode();
	BSTNode(const T&, BSTNode<T>* = nullptr);
	T& getData();
	void setData(const T&);
	BSTNode<T>* getRight();
	void setRight(BSTNode<T>*);
	BSTNode<T>* getLeft();
	void setLeft(BSTNode<T>*);
	BSTNode<T>* getParent();
	void setParent(BSTNode<T>*);
	void destroyAllChildren();

	operator bool() const;
};

/* Initializes the node to blank
	Pre:
	Post:	node is empty
*/
template<typename T>
BSTNode<T>::BSTNode() {
	data = T();
	right, left, parent = nullptr;
}

/* Initializes the note to obj
	Pre:	The obj to add
	Post:	data is obj
*/
template<typename T>
BSTNode<T>::BSTNode(const T& obj, BSTNode<T>* par) : data(obj), right(nullptr), left(nullptr), parent(par) {}

/* Get the data
	Pre:
	Post:
	Return: data
*/
template<typename T>
T& BSTNode<T>::getData() {
	return data;
}

/* Set the data
	Pre:	x to be data
	Post:	data is x
*/
template<typename T>
void BSTNode<T>::setData(const T& x) {
	data = x;
}

/* Get the right node
	Pre:
	Post:
	Return:	The right node
*/
template<typename T>
BSTNode<T>* BSTNode<T>::getRight() {
	return right;
}

/* Set the right node
	Pre:	x = node to be set
	Post:	right is x
*/
template<typename T>
void BSTNode<T>::setRight(BSTNode<T>* x) {
	right = x;
}

/* Get the left node
	Pre:
	Post:
	Return: left
*/
template<typename T>
BSTNode<T>* BSTNode<T>::getLeft() {
	return left;
}

/* Set the left node
	Pre:	x = node pointer to be set
	Post:	left is x
*/
template<typename T>
void BSTNode<T>::setLeft(BSTNode<T>* x) {
	left = x;
}

/* Get the parent node
	Pre:
	Post:
	Return:	parent
*/
template<typename T>
BSTNode<T>* BSTNode<T>::getParent() {
	return parent;
}

/* Set the parent node
	Pre:	x = node pointer to be set
	Post:	parent is x
*/
template<typename T>
void BSTNode<T>::setParent(BSTNode<T>* x) {
	parent = x;
}

/* Destroys all the child nodes
	Pre:
	Post:	All the children are destroyed
*/
template<typename T>
void BSTNode<T>::destroyAllChildren() {
	if (left) {
		left->destroyAllChildren();
		delete left;
		left = nullptr;
	}
	if (right) {
		right->destroyAllChildren();
		delete right;
		right = nullptr;
	}
}

/* Bool operator overload
	Pre:
	Post:
	Return: Returns false if empty, true otherwise
*/
template<typename T>
BSTNode<T>::operator bool() const {
	return (data != T());
}