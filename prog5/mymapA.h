/*
 * Project 5 - mymap
 * Author: Angad Ghakal
 *
 * Class: UIC CS251 Data Structures
 * Professor: Pat Troy
 * Lab: Thurs 12 - 1:45pm
 *
 * Abstraction: Map
 * Implementation: SeeSaw Self Balancing Threaded Binary Search Tree
 * */
#pragma once

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

template<typename keyType, typename valueType>
class mymap {
	private:
		struct NODE {
			keyType key;     // used to build BST
			valueType value; // stored data for the map
			NODE* left;      // links to left child
			NODE* right;     // links to right child
			int nL;          // number of nodes in left subtree
			int nR;          // number of nodes in right subtree
			bool isThreaded; // true if right node points upward

		};

		NODE* root = nullptr; // pointer to root node of the BST
		int size = 0;   // # of key/value pairs in the mymap

		// iterator: This iterator is used so that mymap will work with a foreach loop
		struct iterator {
			private:
				NODE* curr; // points to current in-order node for begin/end

			public:
				iterator(NODE* node) { curr = node; }

				keyType operator*() { return curr->key; }

				bool operator==(const iterator& rhs) { return curr == rhs.curr; }

				bool operator!=(const iterator& rhs) { return curr != rhs.curr; }

				bool isDefault() { return !curr; }

				// operator++: This function should advance curr to the next in-order node. O(1)
				iterator operator++() {

					if (curr->isThreaded) {
						curr = curr->right;
					} else {
						curr = getLeftMostNode(curr->right);
					}

					return iterator(curr);
				}

		};

	public:


		// default constructor: Creates an empty mymap.Time complexity: O(1)
		mymap() {}

		// copy constructor:
		// Constructs a new mymap which is a copy of the "other" mymap.
		// Time complexity: O(nlogn), where n = other.size
		mymap(const mymap& other) {

			if (other.Size() != 0 ) {
				vector<pair<keyType, valueType>> otherVec = other.toVector();

				for (auto item: otherVec) {
					put(item.first, item.second);
				}
			}


		}

		// operator=:
		// Clears "this" mymap and then makes a copy of the "other" mymap.
		// Sets all member variables appropriately.
		// Time complexity: O(nlogn), where n is total number of nodes in threaded,
		// self-balancing BST.
		mymap& operator=(const mymap& other) {

			if (this == &other) {
				return *this;
			}

			// clear what we have
			this->clear();

			vector<pair<keyType, valueType>> otherVec = other.toVector();

			for (auto item: otherVec) {
				put(item.first, item.second);
			}

			return *this;
		}

		// clear:
		// Frees the memory associated with the mymap; can be used for testing.
		// Time complexity: O(n), where n is total number of nodes in threaded,
		// self-balancing BST.
		void clear() {
			clearSubTree(root);
			this->root = nullptr;
			this->size = 0;
		}

		void clearSubTree(NODE* node) {

			if (node != nullptr) {
				clearSubTree(node->left);

				if (!node->isThreaded) {
					clearSubTree(node->right);
				}

				delete node;
			}
		}

		// destructor:
		// Frees the memory associated with the mymap.
		// Time complexity: O(n), where n is total number of nodes in threaded,
		// self-balancing BST.
		~mymap() { this->clear(); }

		static NODE* getLeftMostNode(NODE* start){

			if (start == nullptr) {
				return nullptr;
			}

			while (start->left != nullptr) {
				start = start->left;
			}

			return start;
		}

		// put:
		// Inserts the key/value into the threaded, self-balancing BST based on
		// the key.
		// Time complexity: O(logn + mlogm), where n is total number of nodes in the
		// threaded, self-balancing BST and m is the number of nodes in the
		// sub-tree that needs to be re-balanced.
		// Space complexity: O(1)
		void put(const keyType& key, const valueType& value) {

			bool keyExists = contains(key);

			NODE* searchSpot = root;
			NODE* parent = nullptr;
			bool wentRight = false;

			while (true) {

				// we ran into a wall
				if (searchSpot == nullptr || ((wentRight && parent != nullptr) && parent->isThreaded)) {
					break;
				}

				parent = searchSpot;

				if (key < searchSpot->key) { // look left
					searchSpot->nL += (!keyExists) ? 1 : 0;
					searchSpot = searchSpot->left;

					wentRight = false;
				} else if (key > searchSpot->key) { // look right
					searchSpot->nR += (!keyExists) ? 1 : 0;

					searchSpot = searchSpot->right;
					wentRight = true;
				} else {
					searchSpot->value = value;
					return;
				}
			}

			// add a new child to tree
			NODE* child = new NODE();

			child->nL = 0;
			child->nR = 0;

			child->key = key;
			child->value = value;

			child->left = nullptr;
			this->size += 1;

			if (parent == nullptr) {

				child->isThreaded = false;

				this->root = child;
				child->right = nullptr;

			} else if (wentRight) {

				child->right = parent->right;
				parent->right = child;
				child->isThreaded = parent->isThreaded;

				parent->isThreaded = false;

			} else {

				parent->left = child;
				child->right = parent;

				child->isThreaded = true;

			}
/*
			if (violation != nullptr) {
				vector<NODE*> nodesToRebalance = getNodes(violation);

				violation = buildSubTree(nodesToRebalance, 0, nodesToRebalance.size() - 1);

				if (parentViolation == nullptr) {
					this->root = violation;
				} else if (violentRight) {
					parentViolation->right = violation;
				} else {
					parentViolation->left = violation;
				}

				cout << "bloop" << endl;
*/

		}
/*
		// rebuild a perfect subtree from nodeVec, rewires all nodes returns root of subTree
		NODE* buildSubTree(vector<NODE *>& nodeVec, int leftBound, int rightBound) {

			if ((rightBound < 0 || leftBound < 0) || rightBound < leftBound) {
				return nullptr;
			}
 /*
 			if (left == right) {
 				nodeVec.at(left)->nL = 0;
 				nodeVec.at(left)->nR = 0;

 				nodeVec.at(left)->left = nullptr;

 				if (left != nodeVec.size() - 1) {
 					nodeVec.at(left)->isThreaded = true;
 					nodeVec.at(left)->right = nodeVec.at(left + 1);
 				}

 				return nodeVec.at(left);
 			}

			int mid = leftBound + (rightBound - leftBound) / 2;

			NODE *subRoot = nodeVec.at(mid);

			subRoot->left = buildSubTree(nodeVec, leftBound, mid - 1);
			subRoot->right = buildSubTree(nodeVec, mid + 1, rightBound);

			if (subRoot->right == nullptr) {
				subRoot->nR = 0;

				if (mid + 1 < nodeVec.size()) {
					subRoot->isThreaded = true;
					subRoot->right = nodeVec.at(mid + 1);
				}

			} else {
				subRoot->nR = max(subRoot->right->nL, subRoot->right->nR) + 1;
			}

			if (subRoot->left == nullptr) {
				subRoot->nL = 0;
			} else {
				subRoot->nL = max(subRoot->left->nL, subRoot->left->nR) + 1;
			}


			return subRoot;
		}
*/
		// TODO: testing with threading
		// contains:
		// Returns true if the key is in mymap, return false if not.
		// Time complexity: O(logn), where n is total number of nodes in the
		// threaded, self-balancing BST
		bool contains(keyType key) {

			NODE *nodeSearch = root;

			while (true) {

				if (nodeSearch == nullptr) {
					return false;
				}

				if (nodeSearch->key == key) {
					return true;
				} else if (nodeSearch->key > key) {
					nodeSearch = nodeSearch->left;
				} else {

					if (nodeSearch->isThreaded) {
						nodeSearch = nullptr;

					} else {
						nodeSearch = nodeSearch->right;
					}
				}
			}


		}

		// TODO: testing with threading
		// Returns the value for the given key; if the key is not found, the
		// default value, valueType(), is returned (but not added to mymap).
		// Time complexity: O(logn), where n is total number of nodes in the
		// threaded, self-balancing BST
		valueType get(keyType key) const {

			NODE* searchSpot = this->root;
			NODE* parent = nullptr;
			bool wentRight = false;

			while (true) {

				// we ran into a wall
				if (searchSpot == nullptr || ((wentRight && parent != nullptr) && parent->isThreaded)) {
					return valueType();
				}

				parent = searchSpot;

				if (key < searchSpot->key) { // look left
					searchSpot = searchSpot->left;
					wentRight = false;

				} else if (key > searchSpot->key) {// look right
					wentRight = true;
					searchSpot = searchSpot->right;

				} else { // found key
					return searchSpot->value;
				}
			}
		}

		// TODO: Biggus testing
		// operator[]:
		//
		// Returns the value for the given key; if the key is not found,
		// the default value, valueType(), is returned (and the resulting new
		// key/value pair is inserted into the map).
		// Time complexity: O(logn + mlogm), where n is total number of nodes in the
		// threaded, self-balancing BST and m is the number of nodes in the
		// sub-trees that need to be re-balanced.
		// Space complexity: O(1)
		valueType operator[](keyType key) {

			if (!contains(key)) {
				this->put(key, valueType());
			}

			return this->get(key);
		}

		// Size:
		// Returns the # of key/value pairs in the mymap, 0 if empty.
		// O(1)
		// TODO: Biggus testing
		int Size() const { return this->size; }

		// TODO: testing
		// begin:
		// returns an iterator to the first in order NODE.
		// Time complexity: O(logn), where n is total number of nodes in the
		// threaded, self-balancing BST
		iterator begin() const {

			NODE* start = getLeftMostNode(this->root);

			return iterator(start);
		}

		// end:
		// returns an iterator to the last in order NODE.
		// this function is given to you.
		// Time Complexity: O(1)
		// TODO: testing
		iterator end() const {
			return iterator(nullptr);
		}

		// toString:
		// Returns a string of the entire mymap, in order. O(n), n = this->size
		string getString(const NODE* node) {

			if (node == nullptr) {
				return "";
			}

			stringstream s("");

			if (node->left != nullptr) {
				s << getString(node->left);
			}

			s << "key: ";
			s << node->key;
			s << " value: ";
			s << node->value;
			s << "\n";

			if (!node->isThreaded && node->right != nullptr) {
				s << getString(node->right);
			}

			return s.str();
		}

		string toString() {

			stringstream s("");

			for (iterator i = begin(); i != end(); ++i) {

				s << "key: ";
				s << *i;
				s << " value: ";
				s << this->get(*i);
				s << endl;
			}

			return s.str();
		}

		// toVector:
		// Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
		// {{8, 80}, {15, 150}, {20, 200}}
		// Time complexity: O(n), where n is total number of nodes in the
		// threaded, self-balancing BST
		vector<pair<keyType, valueType>> toVector() const {

			vector<pair<keyType, valueType>> myVec;

			pair<keyType, valueType> newPair;

			for (iterator i = begin(); i != end(); ++i) {

				newPair.first = *i;
				newPair.second = this->get(*i);

				myVec.push_back(newPair);
			}

			return myVec;
		}
/*
		vector<NODE*> getNodes(NODE* subRoot) {

			NODE* start = getLeftMostNode(subRoot);

			NODE* end = getRightMostNode(subRoot);

			vector<NODE*> myVec;

			for (iterator i = iterator(start); i != iterator(end); ++i) {
				myVec.push_back(i.getNode());
			}

			myVec.push_back(end);

			return myVec;
		}

*/
		//
		// checkBalance:
		//
		// Returns a string of mymap that verifies that the tree is properly
		// balanced.  For example, if keys: 1, 2, 3 are inserted in that order,
		// function should return a string in this format (in pre-order):
		// "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";
		// Time complexity: O(n), where n is total number of nodes in the
		// threaded, self-balancing BST
		//

		void balanceCheckRecursive(NODE* subRoot, stringstream& s) const {

			if (subRoot == nullptr) {
				return;
			}

			s << "key: ";
			s << subRoot->key;
			s << ", nL: ";
			s << subRoot->nL;
			s << ", nR: ";
			s << subRoot->nR;
			s << "\n";

			balanceCheckRecursive(subRoot->left, s);

			if (!subRoot->isThreaded) {
				balanceCheckRecursive(subRoot->right, s);
			}

		}

		string checkBalance() const {

			stringstream balanceStream("");

			balanceCheckRecursive(this->root, balanceStream);

			return balanceStream.str();
		}

};
