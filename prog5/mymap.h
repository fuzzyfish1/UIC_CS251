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
 *
 * */
#pragma once

#include <iostream>
#include <sstream>
#include <vector>
//#include <math.h>

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

		NODE* root; // pointer to root node of the BST
		int size;   // # of key/value pairs in the mymap

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

					bool thread = curr->isThreaded;

					curr = curr->right;

					if (!thread) {

						while (curr != nullptr && curr->left != nullptr) {
							curr = curr->left;
						}
					}
					//curr = curr->right;
					return iterator(curr);
				}

				NODE* getNode() {
					return curr;
				}

		};

		/**clearSubTree(NODE* subRoot)\n
			 * a recursive function that clears a subtree on the tree\n
			 * O(n), n is the number of nodes in subTree
			 * @param subRoot root of subTree to be cleared
			 * */
		// these membe funcs gotta move somewhere above the public: thing
		void clearSubTree(NODE* subRoot) {

			if (subRoot == nullptr) {
				return;
			}

			clearSubTree(subRoot->left);

			if (!subRoot->isThreaded) {
				clearSubTree(subRoot->right);
			}

			delete subRoot;

		}

		/** getNode()
 * @param key node with key to search for
 * @returns nullptr if not found
 * @returns a NODE* with key if found
 * */
		NODE* getNode(keyType key) const {

			NODE* searchSpot = this->root;

			while (true) {

				// we ran into a wall
				if (searchSpot == nullptr) {
					return nullptr;
				}

				if (key < searchSpot->key) { // look left
					searchSpot = searchSpot->left;

				} else if (key > searchSpot->key) {// look right
					searchSpot = searchSpot->isThreaded ? nullptr: searchSpot->right;

				} else { // found key
					return searchSpot;
				}
			}
		}

		/**buildTree ()
 * recursive function to buildSubTree
 * @param nodeVec list of nodes to find subTrees on
 * @param leftBound left bound of nodes to look for subTree
 * @param rightBound right bound of nodes to look for subTree
 * @returns root to a subTree
 * @returns nullptr when parent node has no children
 * */
		NODE* buildTreeRecursive(vector<NODE*>& nodeVec, int leftBound, int rightBound) {

			if ((rightBound < 0 || leftBound < 0) || rightBound < leftBound) {
				return nullptr;
			}

			int mid = (leftBound + rightBound) / 2;
			NODE* subRoot = nodeVec.at(mid);

			subRoot->left = buildTreeRecursive(nodeVec, leftBound, mid - 1);
			NODE* right = buildTreeRecursive(nodeVec, mid + 1, rightBound);

			if (right != nullptr) {
				subRoot->isThreaded = false;
				subRoot->right = right;
				subRoot->nR = max(subRoot->right->nR, subRoot->right->nL) + 1;
			} else {
				subRoot->nR = 0;
			}

			if (subRoot->left == nullptr) {
				subRoot->nL = 0;
			} else {
				subRoot->nL = max(subRoot->left->nR, subRoot->left->nR) + 1;
			}

			return subRoot;
		}

		/** rebalance()\n
 * rebuilds a subtree from a violating node\n
 * then reattaches it to the tree
 * O(mlogm), m is total nodes to be rebalanced
 * @param parentViolation parent of the node violating balance
 * @param violation node violating balance
 * @see buildSubTree()
 * @see put()
*/
		void rebalance(NODE* parentViolation, NODE* violation) {
			vector<NODE*> nodesToRebalance = getNodes(violation);

			if (parentViolation == nullptr) {

				this->root = buildSubTree(nodesToRebalance);
			} else if (parentViolation->right == violation) {

				parentViolation->right = buildSubTree(nodesToRebalance);
				parentViolation->nR = max(parentViolation->right->nR, parentViolation->right->nL) + 1;
			} else {

				parentViolation->left = buildSubTree(nodesToRebalance);
				parentViolation->nL = max(parentViolation->left->nR, parentViolation->left->nL) + 1;
			}
		}

		/** buildSubTree()\n
		 * rebuilds a subTree from a vector of nodes\n
		 * @param nodeVec nodes to rebuild
		 * @returns root of new subTree
		 * @see buildTreeRecursive()
		 * */
		NODE* buildSubTree(vector<NODE*>& nodeVec) {
			int mid = (nodeVec.size() - 1) / 2;

			NODE* rightThreadNode = nodeVec.at(nodeVec.size() - 1)->right;
			bool isThreaded = nodeVec.at(nodeVec.size() - 1)->isThreaded;

			// now thread everything
			for (int i = 1; i < nodeVec.size() - 1; i++) {
				nodeVec.at(i - 1)->isThreaded = true;
				nodeVec.at(i - 1)->right = nodeVec.at(i);
			}

			buildTreeRecursive(nodeVec, 0, nodeVec.size() - 1);

			nodeVec.at(nodeVec.size() - 1)->right = rightThreadNode;
			nodeVec.at(nodeVec.size() - 1)->isThreaded = isThreaded;

			return nodeVec.at(mid);
		}

		vector<NODE*> getNodes(NODE* subRoot) {
			vector<NODE*> myVec;

			if (subRoot == nullptr) {
				return myVec;
			}

			NODE* start = subRoot;
			while (start->left != nullptr) {
				start = start->left;
			}

			NODE* end = subRoot;
			while (!end->isThreaded && end->right != nullptr) {
				end = end->right;
			}

			for (iterator i = iterator(start); i != iterator(end); ++i) {
				//cout << "bloop" << endl;
				myVec.push_back(i.getNode());
			}

			myVec.push_back(end);

			return myVec;
		}

	public:

		/** Default Constructor\n
		 * builds an empty mymap obj\n
		 * */
		mymap(): root(nullptr), size(0) {}

		/** copy Constructor\n
		 * every key inside other is now into *this \n
		 * O(nlogn) n = other.size()
		 * @param other a mymap obj that is to be copied
		 * **/
		mymap(const mymap& other): root(nullptr), size(0) {

			if (other.Size() == 0 ) {
				return;
			}

			for (const auto& key: other) {
				put(key, other.get(key));
			}
		}

		/** operator=\n
		 * this is cleared, then other is copied into *this\n
		 * O(nlogn), n = other.size()\n
		 * @param other this will be a copy of other
		 * */
		mymap& operator=(const mymap& other) {

			if (this == &other) {
				return *this;
			}

			// clear what we have
			this->clear();

			for (auto key: other) {
				put(key, other.get(key));
			}

			return *this;
		}

		/** clear()\n
		 * empties and deletes all data associated with tree\n
		 * O(n), n = this->size;\n
		 * @see clearSubTree()
		 * */
		void clear() {
			clearSubTree(root);
			this->root = nullptr;
			this->size = 0;
		}

		/**destructor\n
		 * O(n), n = this->size;\n
		 * this function clears the entire tree
		 * @see clearSubRoot()
		 * */
		~mymap() { this->clear(); }

		/**put()\n
		 * inserts a key value pair into the tree,\n
		 * if key already exists in tree, then value is updated\n
		 * Time: O(logn + mlogm) space: O(1)\n
		 * n = this->size, m is num nodes to rebalance\n
		 * @param key key to insert
		 * @param value value to insert
		 * @see rebalance()
		 * */
		void put( keyType key, valueType value) {

			NODE* searchSpot = this->getNode(key);

			// the case that it already exists
			if (searchSpot != nullptr) {
				searchSpot->value = value;
				return;
			}

			this->size += 1;
			NODE* child = new NODE();

			// give values to your children
			child->nL = 0;
			child->nR = 0;
			child->key = key;
			child->value = value;
			child->left = nullptr;

			NODE* parentViolation = nullptr; // the mother of the violator will be modified for rebalancing
			NODE* parent = nullptr; // the mother of the child we are looking at
			NODE* violation = nullptr; // the violator of balance will be corrected

			searchSpot = root;
			bool wentRight = false;

			while (searchSpot != nullptr) {

				((key < searchSpot->key) ? searchSpot->nL++ : searchSpot->nR++);

				if (violation == nullptr && !isBalanced(searchSpot)) {
					violation = searchSpot;
					parentViolation = parent;
				}

				parent = searchSpot;

				if (key < searchSpot->key) {
					searchSpot = searchSpot->left;
					wentRight = false;

				} else {
					searchSpot = searchSpot->isThreaded ? nullptr : searchSpot->right;
					wentRight = true;
				}
			}

			// add a new child to tree
			if (parent == nullptr) {

				child->isThreaded = false;
				child->right = nullptr;

				this->root = child;

			} else if (wentRight){

				child->isThreaded = parent->isThreaded;
				child->right = parent->right;

				parent->isThreaded = false;
				parent->right = child;

			} else {
				child->isThreaded = true;
				child->right = parent;

				parent->left = child;
			}

			if (violation != nullptr) {
				rebalance(parentViolation, violation);
			}
		}

		/**contains()\n
		 * time: O(logn), n = this->size
		 * @returns true when a node with the key exists in the tree
		 * false if not
		 * @see getNode
		 * */
		bool contains(keyType key) {
			NODE* child = getNode(key);
			return (child != nullptr);
		}

		/**get()\n
		 * time: O(logn)\n
		 * @param key key whos value we need
		 * @returns valueType() when key DNE
		 * @returns value of node with key
		 * */
		valueType get(keyType key) const {

			NODE* child = getNode(key);

			if (child != nullptr) {
				return child->value;
			} else {
				return valueType();
			}
		}

		/**operator[]\n
		 * its like get but adds key to thing if DNE\n
		 * time: O(logn + mlogm) space: O(1)
		 * @param key node with key to look for
		 * @returns valueType() and adds to node if DNE
		 * @returns value from key node if exists
		 * */
		valueType operator[](keyType key) {

			//this->put(key, valueType());
			NODE* nod = getNode(key);

			if (nod == nullptr) {

				this->put(key, valueType());
			}

			return this->get(key);
		}

		/**Size()
		 * time: O(n)
		 * @returns size;
		 * */
		int Size() const { return this->size; }

		/**begin()\n
		 * timeComplexity: O(logn)\n
		 * @returns iterator at least node
		 * */
		iterator begin(NODE* start = nullptr) const {

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
		iterator end(NODE* end = nullptr) const {

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
				s << endl;

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

			s << "key: " << subRoot->key << ", nL: " << subRoot->nL << ", nR: " << subRoot->nR << "\n";

			if (subRoot->left != nullptr) {
				balanceCheckRecursive(subRoot->left, s);
			}

			if (subRoot->right != nullptr && !subRoot->isThreaded) {
				balanceCheckRecursive(subRoot->right, s);
			}

		}

		string checkBalance() const {

			stringstream s("");

			balanceCheckRecursive(this->root, s);

			return s.str();
		}

		bool isBalanced(NODE* nod) const {

			if (nod == nullptr) {
				return true;
			}

			return max(nod->nL, nod->nR) <= 2 * min(nod->nL, nod->nR) + 1;

		}

};
