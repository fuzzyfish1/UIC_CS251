#include <iostream>
#include <string>
#include "myrandom.h" // used in autograder, do not remove

using namespace std;

class Bar {

private:
	
	string name;
	int value;
	string category;

public:

	Bar(string name = "", int value = 0, string category = "") {

		this->name = name;
		this->value = value;
		this->category = category;

	}

	// destructor:
	virtual ~Bar() {

		//delete name;
		// TODO:  Write this destructor or leave empty if not needed.

	}

	// getName:
	string getName() {

		return this->name;
	}

	// getValue:
	int getValue() {
		return this->value;
	}

	// getCategory:
	string getCategory() {
		return this->category;
	}

	// operators
	// objects.  Comparison should be based on the Bar's value.  For example:
	bool operator<(const Bar &other) const {
		return this->value < other.value;
	}

	bool operator<=(const Bar &other) const {
		return this->value <= other.value;
	}

	bool operator>(const Bar &other) const {
		return this->value > other.value;
	}

	bool operator>=(const Bar &other) const {
		return this->value >= other.value;
	}

};

