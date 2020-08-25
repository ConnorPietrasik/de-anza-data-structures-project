#pragma once

#include"BSTNode.h"
#include<string>
#include<sstream>
#include"TArray.h"
#include"LinkedList.h"

//Its own namespace so it doesn't conflict with the default functors in the other data structures
namespace Tree {
	//Default functors, only the ones where the thing in enable_if is true are compiled (so it works with pointers and non-pointers)
	class Greater {
	public:
		template<typename T>
		bool operator()(const T& obj1, const T& obj2, typename std::enable_if<std::is_pointer<T>::value>::type* = 0) {
			return (*obj1 >= *obj2);
		}

		template<typename T>
		bool operator()(const T& obj1, const T& obj2, typename std::enable_if<!std::is_pointer<T>::value>::type* = 0) {
			return (obj1 >= obj2);
		}

		template<typename T1, typename T2>
		bool operator()(const T1& obj1, const T2& obj2, typename std::enable_if<std::is_pointer<T1>::value>::type* = 0, typename std::enable_if<!std::is_pointer<T2>::value>::type* = 0) {
			return (*obj1 >= obj2);
		}

		template<typename T1, typename T2>
		bool operator()(const T1& obj1, const T2& obj2, typename std::enable_if<!std::is_pointer<T1>::value>::type* = 0, typename std::enable_if<std::is_pointer<T2>::value>::type* = 0) {
			return (obj1 >= *obj2);
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

		template<typename T1, typename T2>
		bool operator()(const T1& obj1, const T2& obj2, typename std::enable_if<std::is_pointer<T1>::value>::type* = 0, typename std::enable_if<!std::is_pointer<T2>::value>::type* = 0) {
			return (*obj1 == obj2);
		}

		template<typename T1, typename T2>
		bool operator()(const T1& obj1, const T2& obj2, typename std::enable_if<!std::is_pointer<T1>::value>::type* = 0, typename std::enable_if<std::is_pointer<T2>::value>::type* = 0) {
			return (obj1 == *obj2);
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

template<typename T, class C = Tree::Greater, class E = Tree::Equality, class P = Tree::PrintValue>
class BST {
	bool update;
	int printMode;
	std::string print;
	BSTNode<T>* root;
	C comp;
	E equal;
	P val;
	void addNode(const T&, BSTNode<T>*, BSTNode<T>*, bool = true);
	void printOrder(std::stringstream&, BSTNode<T>*, int = 0);
	void printPre(std::stringstream&, BSTNode<T>*, int = 0);
	void printPost(std::stringstream&, BSTNode<T>*, int = 0);

	template<typename ST, class SearchEquality = E, class SearchComparison = C>
	void find(const ST&, BSTNode<T>*&, BSTNode<T>*, SearchEquality&, SearchComparison&);
	void bigCheck(BSTNode<T>*, BSTNode<T>*&);
	void smallCheck(BSTNode<T>*, BSTNode<T>*&);
	void delLeaf(BSTNode<T>*);
	void delRoot();
	void delBranch(BSTNode<T>*);
	void swap(BSTNode<T>*, BSTNode<T>*);

	void dataRetrieve(TArray<T, C, E, P>&, BSTNode<T>*);

public:
	BST();
	~BST();
	void clear();
	void add(const T&);
	void setPrint(int = 0);
	const std::string& getPrint() const;
	bool getUpdate() const;
	int getPrintMode() const;

	template<typename ST, class SearchEquality = E, class SearchComparison = C>
	BSTNode<T>* search(const ST&, SearchEquality = SearchEquality(), SearchComparison = SearchComparison());

	template<class SearchEquality = E, class SearchComparison = C>
	void remove(const T&, SearchEquality = SearchEquality(), SearchComparison = SearchComparison());
	BSTNode<T>* largest(BSTNode<T>* = nullptr);
	BSTNode<T>* smallest(BSTNode<T>* = nullptr);

	template<typename ST, class SearchEquality = E, class SearchComparison = C>
	TArray<T, C, E, P> findAll(const ST&, SearchEquality = SearchEquality(), SearchComparison = SearchComparison());

	template<typename ST, class SearchEquality = E>
	TArray<T, C, E, P> findAllBrute(const ST&, SearchEquality = SearchEquality());

	TArray<T, C, E, P> getAllData();
};

/* Gets the update status
	Pre:	
	Post:
	Return:	The update status
*/
template<typename T, class C, class E, class P>
bool BST<T, C, E, P>::getUpdate() const {
	return update;
}

/* Gets the current print mode
	Pre:
	Post:
	Return:	The print mode
*/
template<typename T, class C, class E, class P>
int BST<T, C, E, P>::getPrintMode() const {
	return printMode;
}


/* Default initialization
	Pre:	c = a function pointer to a comparison function
			e = a function pointer to an equality function
	Post:	comp = C, E, Pqual = e, root = nullptr, print = ""
*/
template<typename T, class C, class E, class P>
BST<T, C, E, P>::BST() : root(nullptr), print(""), printMode(0), update(false) {}

/* Default destructor
	Pre:
	Post:	All the nodes are destroyed properly
*/
template<typename T, class C, class E, class P>
BST<T, C, E, P>::~BST() {
	clear();
}

/* Clears the tree
	Pre:
	Post:	The tree is now empty
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::clear() {
	if (root) {
		root->destroyAllChildren();
		delete root;
		root = nullptr;
	}
}

/* Adds to the tree
	Pre:	x = obj to be added
	Post:	x is in the tree
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::add(const T& x) {
	update = true;
	if (root) {
		if (comp(x, root->getData())) addNode(x, root->getRight(), root);
		else addNode(x, root->getLeft(), root, false);
	}
	else {
		root = new BSTNode<T>(x);
	}
}

/* Adds to the tree recursively
	Pre:	x = data to be added
			curNode = pointer to current node
			parNode = pointer to parent node (made before I added getParent, not worth changing)
			isRight = bool, true if the data is greater than parent (or whatever comparison is set to)
	Post:	x is in the tree properly
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::addNode(const T& x, BSTNode<T>* curNode, BSTNode<T>* parNode, bool isRight) {
	if (curNode) {
		if (comp(x, curNode->getData())) addNode(x, curNode->getRight(), curNode);
		else addNode(x, curNode->getLeft(), curNode, false);
	}
	else {
		if (isRight) parNode->setRight(new BSTNode<T>(x, parNode));
		else parNode->setLeft(new BSTNode<T>(x, parNode));
	}
}

/* Sets the print string in the specified way
	Pre:	printType = int saying how to set it
				-1 = pre-order
				0 = in order
				1 = post-order
	Post:	print will hold the tree display
	Return:	
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::setPrint(int printType) {
	printMode = printType;
	std::stringstream ss;
	switch (printType) {
	case -1: printPre(ss, root); break;
	case 0: printOrder(ss, root); break;
	case 1: printPost(ss, root); break;
	}
	print = ss.str();
	update = false;
}

/* Recursively sets the print string using In-Order Traversal
	Pre:	ss = stringstream to throw data into
			curNode = pointer to current node
			depth = int for how many spaces should be added
	Post:	ss now has the proper string
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::printOrder(std::stringstream& ss, BSTNode<T>* curNode, int depth) {
	if (curNode) {
		printOrder(ss, curNode->getLeft(), depth + 15);
		for (int i = 0; i < depth; i++) ss << ' ';
		ss << val(curNode->getData()) << '\n';
		printOrder(ss, curNode->getRight(), depth + 15);
	}
}

/* Recursively sets the print string using Pre-Order Traversal
	Pre:	ss = stringstream to throw data into
			curNode = pointer to current node
			depth = int for how many spaces should be added
	Post:	ss now has the proper string
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::printPre(std::stringstream& ss, BSTNode<T>* curNode, int depth) {
	if (curNode) {
		for (int i = 0; i < depth; i++) ss << ' ';
		ss << val(curNode->getData()) << '\n';
		printPre(ss, curNode->getLeft(), depth + 3);
		printPre(ss, curNode->getRight(), depth + 3);
	}
}

/* Recursively sets the print string using Post-Order Traversal
	Pre:	ss = stringstream to throw data into
			curNode = pointer to current node
			depth = int for how many spaces should be added
	Post:	ss now has the proper string
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::printPost(std::stringstream& ss, BSTNode<T>* curNode, int depth) {
	if (curNode) {
		printPost(ss, curNode->getLeft(), depth + 3);
		printPost(ss, curNode->getRight(), depth + 3);
		for (int i = 0; i < depth; i++) ss << ' ';
		ss << val(curNode->getData()) << '\n';
	}
}

/* Returns the print string previously set
	Pre:
	Post:
	Return:	The print string
*/
template<typename T, class C, class E, class P>
const std::string& BST<T, C, E, P>::getPrint() const { return print; }

/* Searches for the specified data in the tree
	Pre:	x = the obj to be searched for
	Post:
	Return:	A pointer to the node containing the object / nullptr if not found
*/
template<typename T, class C, class E, class P>
template<typename ST, class SearchEquality, class SearchComparison>
BSTNode<T>* BST<T, C, E, P>::search(const ST& x, SearchEquality sEqual, SearchComparison sComp) {
	BSTNode<T>* ret = nullptr;
	find(x, ret, root, sEqual, sComp);
	return ret;
}

/* Recursively searches for the specified object
	Pre:	x = obj to be searched for
			ret = node to be returned
			curNode = the current node
	Post:	ret now contains the address of the node containing the data
*/
template<typename T, class C, class E, class P>
template<typename ST, class SearchEquality, class SearchComparison>
void BST<T, C, E, P>::find(const ST& x, BSTNode<T>*& ret, BSTNode<T>* curNode, SearchEquality& sEqual, SearchComparison& sComp) {
	if (curNode) {
		if (sEqual(x, curNode->getData())) {
			ret = curNode;
			return;
		}
		if (sComp(x, curNode->getData())) find(x, ret, curNode->getRight(), sEqual, sComp);
		else find(x, ret, curNode->getLeft(), sEqual, sComp);
	}
}

/* Removes the node with the specified data from the tree
	Pre:	obj = the obj to be removed
	Post:	The node containing the specified obj is now gone
*/
template<typename T, class C, class E, class P>
template<class SearchEquality, class SearchComparison>
void BST<T, C, E, P>::remove(const T& obj, SearchEquality sEqual, SearchComparison sComp) {
	auto node = search(obj, sEqual, sComp);
	if (node) {
		update = true;
		auto parent = node->getParent();
		if (parent) {
			if (!node->getRight() && !node->getLeft()) {
				delLeaf(node);
				node = nullptr;
			}
			else if (node->getRight() && !node->getLeft()) {
				auto temp = node->getRight();
				temp->setParent(parent);
				if (node == parent->getRight()) {
					delete node;
					node = nullptr;
					parent->setRight(temp);
				}
				else {
					delete node;
					node = nullptr;
					parent->setLeft(temp);
				}
			}
			else if (!node->getRight() && node->getLeft()) {
				auto temp = node->getLeft();
				temp->setParent(parent);
				if (node == parent->getRight()) {
					delete node;
					node = nullptr;
					parent->setRight(temp);
				}
				else {
					delete node;
					node = nullptr;
					parent->setLeft(temp);
				}
			}
			else {
				delBranch(node);
			}
		}
		else {
			delRoot();
			node = nullptr;
		}
	}
}

/* Deletes a leaf node
	Pre:	node = a pointer to the node to be deleted
	Post:	node is now gone
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::delLeaf(BSTNode<T>* node) {
	auto parent = node->getParent();
	if (parent) {
		if (node == parent->getRight()) {
			delete node;
			parent->setRight(nullptr);
		}
		else if (node == parent->getLeft()){
			delete node;
			parent->setLeft(nullptr);
		}
	}
	else delRoot();
}

/* Deletes the root
	Pre:
	Post:	The root is now gone
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::delRoot() {
	if (!root->getRight() && !root->getLeft()) {
		delete root;
		root = nullptr;
	}
	else if (!root->getRight() && root->getLeft()) {
		auto temp = root->getLeft();
		delete root;
		root = temp;
	}
	else if (root->getRight() && !root->getLeft()) {
		auto temp = root->getRight();
		delete root;
		root = temp;
	}
	else {
		delBranch(root);
	}
}

/* Deletes a branch node (recursively if neccessary)
	Pre:	node = the node to be deleted
	Post:	the node is gone
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::delBranch(BSTNode<T>* node) {
	BSTNode<T>* temp = nullptr;
	if (node->getLeft()) temp = largest(node->getLeft());
	if (temp) {
		if (!temp->getLeft()) {
			swap(node, temp);
			delLeaf(temp);
			return;
		}
	}
	temp = nullptr;
	if (node->getRight()) temp = smallest(node->getRight());
	if (temp) {
		if (!temp->getRight()) {
			swap(node, temp);
			delLeaf(temp);
		}
		else {
			swap(node, temp);
			delBranch(temp);
		}
	}
}

/* Swaps the data of the two node pointers
	Pre:	obj1 and obj2 are objects to be swapped
	Post:	obj1's and obj2's data have been swapped
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::swap(BSTNode<T>* obj1, BSTNode<T>* obj2) {
	auto temp = obj1->getData();
	obj1->setData(obj2->getData());
	obj2->setData(temp);
}

/* Returns a pointer to the largest node in the subtree (or the whole tree)
	Pre:	start = the root of the subtree
	Post:
	Return:	Returns the largest
*/
template<typename T, class C, class E, class P>
BSTNode<T>* BST<T, C, E, P>::largest(BSTNode<T>* start) {
	if (!start) start = root;
	BSTNode<T>* ret = nullptr;
	bigCheck(start, ret);
	return ret;
}

/* Recursively gets the biggest node in the subtree
	Pre:	cur = the current node
			ret = the node to be returned
	Post:	ret now has the address of the biggest node
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::bigCheck(BSTNode<T>* cur, BSTNode<T>*& ret) {
	if (cur->getRight()) {
		bigCheck(cur->getRight(), ret);
	}
	else ret = cur;
}

/* Returns the smallest node in the subtree
	Pre:	start = the root of the subtree
	Post:
	Return: a pointer to the smallest node
*/
template<typename T, class C, class E, class P>
BSTNode<T>* BST<T, C, E, P>::smallest(BSTNode<T>* start) {
	if (!start) start = root;
	BSTNode<T>* ret = nullptr;
	smallCheck(start, ret);
	return ret;
}

/* Recursively gets the smallest node in the subtree
	Pre:	cur = the current node
			ret = the node to be returned
	Post:	ret now has the address of the smallest node
*/
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::smallCheck(BSTNode<T>* cur, BSTNode<T>*& ret) {
	if (cur->getLeft()) {
		smallCheck(cur->getLeft(), ret);
	}
	else ret = cur;
}

//Returns a TArray of all T's in the array, if none are found it brute forces it
template<typename T, class C, class E, class P>
template<typename ST, class SearchEquality, class SearchComparison>
TArray<T, C, E, P> BST<T, C, E, P>::findAll(const ST& x, SearchEquality sEqual, SearchComparison sComp) {
	BSTNode<T>* node = nullptr;
	find(x, node, root, sEqual, sComp);
	TArray<T, C, E, P> ret;

	bool left = false;
	BSTNode<T>* next = node;
	while (node) {
		next = nullptr;
		ret.push_back(node->getData());
		if (node->getRight()) next = node->getRight();
		else if (node->getLeft() && !left) {
			next = node->getLeft();
			left = true;
		}
		node = nullptr;
		if (next) find(x, node, next, sEqual, sComp);
	}
	if (ret.size() == 0) {
		ret = findAllBrute(x, sEqual);
	}

	return ret;
}

//Checks every node in the tree using the given functor, neccessary if not searching using same key as sorted
template<typename T, class C, class E, class P>
template<typename ST, class SearchEquality>
TArray<T, C, E, P> BST<T, C, E, P>::findAllBrute(const ST& x, SearchEquality sEqual) {
	LinkedList<BSTNode<T>*> nodes;
	TArray<T, C, E, P> arr;
	if (root) {
		nodes.add(root);
		while (nodes.count()) {
			if (sEqual(x, nodes.getHead()->getData()->getData())) {
				arr.push_back(nodes.getHead()->getData()->getData());
			}
			BSTNode<T>* right = nodes.getHead()->getData()->getRight();
			BSTNode<T>* left = nodes.getHead()->getData()->getLeft();
			if (right) nodes.add(right, nodes.count());
			if (left) nodes.add(left, nodes.count());
			nodes.erase();
		}
	}
	return arr;
}

//Returns a TArray containing all the data in tree
template<typename T, class C, class E, class P>
TArray<T, C, E, P> BST<T, C, E, P>::getAllData() {
	TArray<T, C, E, P> ret;
	dataRetrieve(ret, root);
	return ret;
}

//Fills the TArray with all the data in the tree using in-order traversal
template<typename T, class C, class E, class P>
void BST<T, C, E, P>::dataRetrieve(TArray<T, C, E, P>& ret, BSTNode<T>* curNode) {
	if (curNode) {
		dataRetrieve(ret, curNode->getLeft());
		ret.push_back(curNode->getData());
		dataRetrieve(ret, curNode->getRight());
	}
}

/* Prints the tree
	Pre:	out = the ostream
			obj = the tree
	Post:
	Return: returns the ostream
*/
template<typename T, class C, class E, class P>
std::ostream& operator<<(std::ostream& out, BST<T, C, E, P>& obj) {
	if (obj.getUpdate()) obj.setPrint(obj.getPrintMode());
	out << obj.getPrint();
	return out;
}