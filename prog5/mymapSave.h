/*
 * Project 5 - mymap
 * Author: Zain Ali
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
			NODE *left;      // links to left child
			NODE *right;     // links to right child
			int nL;          // number of nodes in left subtree
			int nR;          // number of nodes in right subtree
			bool isThreaded; // true if right node points upward
		};

		NODE *root; // pointer to root node of the BST
		int size;   // # of key/value pairs in the mymap

		// iterator: This iterator is used so that mymap will work with a foreach loop
		struct iterator {
			private:
				NODE *curr; // points to current in-order node for begin/end

			public:
				iterator(NODE* node) { curr = node; }

				keyType operator*() { return curr->key; }

				bool operator==(const iterator &rhs) { return curr == rhs.curr; }

				bool operator!=(const iterator &rhs) { return curr != rhs.curr; }

				bool isDefault() { return !curr; }

				// operator++: This function should advance curr to the next in-order node. O(1)
				iterator operator++() {

					curr = curr->right;
					return iterator(curr);
				}

				NODE* getNode() {
					return curr;
				}

		};

	public:

		// default constructor: Creates an empty mymap.Time complexity: O(1)
		mymap() {

			this->root = nullptr;
			this->size = 0;
		}

		// copy constructor:
		// Constructs a new mymap which is a copy of the "other" mymap.
		// Time complexity: O(nlogn), where n = other.size
		mymap(const mymap& other) {

			this->size = 0;
			this->root = nullptr;

			for (const auto& key: other) {
				put(key, other.get(key));
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
			this->clear(root);
			this->size = 0;

			for (auto key: other) {
				put(key, other.get(key));
			}

			return *this;
		}

		// clear:
		// Frees the memory associated with the mymap; can be used for testing.
		// Time complexity: O(n), where n is total number of nodes in threaded,
		// self-balancing BST.
		void clearSubTree(NODE *node) {

			if (node == nullptr) {
				return;
			}

			clear(node->left);

			if (!node->isThreaded) {
				clear(node->right);
			}

			delete node;

		}

		// destructor:
		// Frees the memory associated with the mymap.
		// Time complexity: O(n), where n is total number of nodes in threaded,
		// self-balancing BST.
		~mymap() {

			this->clear(root);
			this->size = 0;
		}

		// put:
		// Inserts the key/value into the threaded, self-balancing BST based on
		// the key.
		// Time complexity: O(logn + mlogm), where n is total number of nodes in the
		// threaded, self-balancing BST and m is the number of nodes in the
		// sub-tree that needs to be re-balanced.
		// Space complexity: O(1)
		void put(const keyType& key, const valueType& value) {

			vector<NODE*> traversalPath;

			NODE *searchSpot = root;
			NODE *parent = nullptr;
			bool wentRight = false;

			while (true) {

				// we ran into a wall
				if (searchSpot == nullptr || ((wentRight && parent != nullptr) && parent->isThreaded)) {
					break;
				}

				parent = searchSpot;
				traversalPath.push_back(parent);

				if (key < searchSpot->key) { // look left
					searchSpot = searchSpot->left;
					wentRight = false;

				} else if (key > searchSpot->key) { // look right
					searchSpot = searchSpot->right;
					wentRight = true;

				} else { // found key
					return;
				}
			}

			// add a new child to tree if we get here
			NODE *child = new NODE();

			child->nL = 0;
			child->nR = 0;

			child->key = key;
			child->value = value;

			child->left = nullptr;
			this->size += 1;

			traversalPath.push_back(child);

			if (parent != nullptr) {

				if (wentRight) {
					child->isThreaded = parent->isThreaded;

					child->right = parent->right;
					parent->isThreaded = false;

					parent->right = child;

				} else {
					child->isThreaded = true;

					child->right = parent;
					parent->left = child;

				}
			} else {

				child->isThreaded = false;

				child->right = nullptr;
				this->root = child;
			}

			// recompute nL, nR for all nodes in the paths
			recomputeNLNRonPath(traversalPath);

			// balance shit here

			/*
			 *
			 *                         7
			 *                              9
			 *                      6    8
			 *                     5          11
			 *                    4         10  12
			 *
			 *
			 * */

			// rebalance on everynode in traversalPath

			NODE** ptrptrToParent = &root;

			for (int i = 0; i < traversalPath.size() - 1; i++) {

				NODE* nodptr = traversalPath.at(i);

				if (!isBalanced(nodptr)) {

					vector<NODE *> nodesToRebalance = getNodes(nodptr);

					nodptr = buildSubTree(nodesToRebalance, 0, nodesToRebalance.size() - 1);

					*ptrptrToParent = nodptr;

					break;
				}

				if (i + 1 < traversalPath.size() - 1 && traversalPath.at(i)->right == traversalPath.at(i + 1)) {
					ptrptrToParent = &(traversalPath.at(i)->right);
				} else {
					ptrptrToParent = &(traversalPath.at(i)->left);
				}
			}

			cout << "beep" << endl;
		}

		// recompute nL and nR for each node we visited
		void recomputeNLNRonPath(const vector<NODE*>& traversalPath) {
			for (int i = traversalPath.size() - 1; i >= 1; i--) {

				if (traversalPath.at(i - 1)->right == traversalPath.at(i)) {
					traversalPath.at(i - 1)->nR = max(traversalPath.at(i)->nL, traversalPath.at(i)->nR) + 1;

				} else {
					traversalPath.at(i - 1)->nL = max(traversalPath.at(i)->nL, traversalPath.at(i)->nR) + 1;

				}
			}
		}

		// rebuild a perfect subtree from nodeVec, rewires all nodes returns root of subTree
		NODE* buildSubTree(vector<NODE *>& nodeVec, int left, int right) {

			if ((right < 0 || left < 0) || right < left) {
				return nullptr;
			}

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

			int mid = left + (right - left) / 2;

			NODE *subRoot = nodeVec.at(mid);

			subRoot->left = buildSubTree(nodeVec, left, mid - 1);
			subRoot->right = buildSubTree(nodeVec, mid + 1, right);

			if (subRoot->left == nullptr) {
				subRoot->nL = 0;
			} else {
				subRoot->nL = max(subRoot->left->nL, subRoot->left->nR) + 1;
			}

			if (subRoot->isThreaded || subRoot->right == nullptr) {
				subRoot->nR = 0;
			} else {
				subRoot->nR = max(subRoot->right->nL, subRoot->right->nR) + 1;
			}

			return subRoot;
		}

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
		valueType get(keyType key) const{

			NODE *nodeSearch = root;

			while (true) {

				if (nodeSearch == nullptr) {
					return valueType();
				}

				if (nodeSearch->key == key) {
					return nodeSearch->value;

				} else if (key < nodeSearch->key) {
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

			this->put(key, valueType());
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
		iterator begin(NODE *start = nullptr) const {

			if (start == nullptr) {
				start = root;
			}

			while (start->left != nullptr) {
				start = start->left;
			}

			return iterator(start);
		}

		// end:
		// returns an iterator to the last in order NODE.
		// this function is given to you.
		// Time Complexity: O(1)
		// TODO: testing
		iterator end(NODE *end = nullptr) const {

			if (end == nullptr) {
				return iterator(nullptr);

			} else {
				while (end->right != nullptr) {
					end = end->right;
				}

				return iterator(end);
			}
		}

		// toString:
		// Returns a string of the entire mymap, in order. O(n), n = this->size
		string getString(const NODE* node) {

			/** EX: **
			 * Format for 8/80, 15/150, 20/200:
			 * "key: 8 value: 80\nkey: 15 value: 150\nkey: 20 value: 200\n
			 * */

			if (node == nullptr) {
				return "";
			}

			stringstream s("");

			s << getString(node->left);
			s << "key: ";
			s << node->key;
			s << " value: ";
			s << node->value;
			s << "\n";
			s << getString(node->right);

			return s.str();
		}

		string toString() {

			int i = 0;

			stringstream s("");

			for (const auto& key: *this) {
				s << "key: ";
				s << key;
				s << " value: ";
				s << this->get(key);
				s << "\n";

			}

			return s.str();
		}

		// toVector:
		// Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
		// {{8, 80}, {15, 150}, {20, 200}}
		// Time complexity: O(n), where n is total number of nodes in the
		// threaded, self-balancing BST
		vector<pair<keyType, valueType>> toVector() {

			vector<pair<keyType, valueType>> myVec;

			pair<keyType, valueType> newPair;
			for (auto key: *this) {
				newPair.first = key;
				newPair.second = this->get(key);

				myVec.push_back(newPair);

			}


			return myVec;
		}

		vector<NODE *> getNodes(NODE *subRoot) {

			vector<NODE *> myVec;

			for (iterator i = begin(subRoot); i != end(subRoot); ++i) {
				myVec.push_back(i.getNode());
			}

			myVec.push_back(end(subRoot).getNode());

			return myVec;
		}

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

		void balanceCheckRecursive(NODE *subRoot, stringstream &s) {

			s << "key: " << subRoot->key << "nL: " << subRoot->nL << ", nR: " << subRoot->nR << "\n ";

			if (subRoot->left != nullptr) {
				balanceCheckRecursive(subRoot->left);
			}

			if (subRoot->right != nullptr && !subRoot->isThreaded) {
				balanceCheckRecursive(subRoot->right);
			}

		}

		string checkBalance(NODE *nod) {

			stringstream s("");

			s << "key: " << nod->key << "nL: " << nod->nL << ", nR: " << nod->nR << "\n ";

			return s.str();
		}

		bool isBalanced(NODE* nod) {

			if (nod->nL > nod->nR) {
				return nod->nL <= 2* nod->nR + 1;
			} else {
				return nod->nR <= 2* nod->nL + 1;
			}

		}


};
