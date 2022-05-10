// barchart.h
// TODO:  add header comment here.  Also add function header comments below.

#pragma once

#include <iostream>
#include <algorithm>
#include <map>
#include "myrandom.h" // used in autograder, do not remove
#include "bar.h"

#ifndef barchart_h
#define barchart_h

using namespace std;

// Constants used for bar chart animation.  You will primarily
// use these in barchartanimate.h, but you might find some useful here.
const string BOX = "\u29C8";
const string CLEARCONSOLE = "\033[2J";

// Color codes for Mimir (light mode)
const string RED("\033[1;36m");
const string PURPLE("\033[1;32m");
const string BLUE("\033[1;33m");
const string CYAN("\033[1;31m");
const string GREEN("\033[1;35m");
const string GOLD("\033[1;34m");
const string BLACK("\033[1;37m");
const string RESET("\033[0m");
const vector<string> COLORS = {RED, PURPLE, BLUE, CYAN, GREEN, GOLD, BLACK};

class BarChart {
private:

	Bar *bars;
	int capacity;
	int size;

	string frame;

public:

	// default constructor:
	BarChart() {

		bars = nullptr;

		this->size = 0;
		this->capacity = 0;
		this->frame = "";

	}

	// parameterized constructor:
	// Parameter passed in determines memory allocated for bars.
	BarChart(int n) {

		this->capacity = n;
		this->bars = new Bar[n];
		this->size = 0;
		this->frame = "";

	}

	// copy constructor
	BarChart(const BarChart &other) {

		/*
		if (this == &other) {
			return;
		}
		*/
		this->bars = new Bar[other.capacity];
		this->size = other.size;
		this->capacity = other.capacity;
		this->frame = other.frame;

		for (int i = 0; i < other.size; ++i) {
			this->bars[i] = other.bars[i];
		}

	}

	// frees memory and resets all private member variables to default values.
	void clear() {

		delete[] this->bars;

		this->bars = nullptr;
		this->size = 0;
		this->capacity = 0;
		this->frame = "";

	}

	// Called when you assign one BarChart into another, i.e. this = other;
	BarChart &operator=(const BarChart &other) {

		if (this == &other){
            return *this;
        }
        
        if (this->bars != nullptr){
            delete [] bars;
        }
        
        this->bars = new Bar[other.capacity];
        this ->size = other.size;
        this ->capacity = other.capacity;
        this->frame = other.frame;
        
        for (int i = 0; i < other.size; ++i){
            this->bars[i] = other.bars[i];
        }

		return *this;
	}

	// Called automatically by C++ to free the memory associated by theBarChart.
	virtual ~BarChart() {

		if(bars!=nullptr){
			delete[] bars;
		}

	}

	// setFrame
	void setFrame(string frame) {

		this->frame = frame;

	}

	// getFrame()
	// Returns the frame of the BarChart oboject.
	string getFrame() {

		return this->frame;
	}

	// adds a Bar to the BarChart returns true if successful returns false if there is not room
	bool addBar(string name, int value, string category) {
		if (size == capacity) {

			return false;

		} else {

			Bar b(name, value, category);
			this->bars[size] = b;
			size += 1;

			return true;
		}
	

	}

	// getSize()
	// Returns the size (number of bars) of the BarChart object.
	int getSize() {

		return this->size;
	}

	// operator[]
	// Returns Bar element in BarChart.
	// This gives public access to Bars stored in the Barchart.
	// If i is out of bounds, throw an out_of_range error message:
	// "BarChart: i out of bounds"
	Bar &operator[](int i) {
		//Bar b;

		if (i >= size) {
			throw out_of_range(
					"Index out of range BarChart.h" + to_string(i) + "Accesed from " + to_string(size) + "elements");
		} else {
			return bars[i];
		}


	}

	// dump
	// Used for printing the BarChart object.
	// Recommended for debugging purposes.  output is any stream (cout,
	// file stream, or string stream).
	// Format:
	// "frame: 1
	// aname 5 category1
	// bname 4 category2
	// cname 3 category3" <-newline here
	void dump(ostream &output) {

		output << "frame: " << this->frame << endl;

        sort(this->bars, bars + size, greater<Bar>());

		Bar theBar;

		for (int i = 0; i < this->size; i++) {
			theBar = bars[i];
			output << bars[i].getName() << " " << bars[i].getValue() << " " << bars[i].getCategory() << endl;
		}

	}

	// graph
	// Used for printing out the bar.
	// output is any stream (cout, file stream, string stream)
	// colorMap maps category -> color
	// top is number of bars you'd like plotted on each frame (top 10? top 12?)
	void graph(ostream &output, map<string, string> &colorMap, int top) {

		if (top > this->size || top <= 0) {
			top = this->size;
		}

		int lenMax = 60;  // this is number of BOXs that should be printed
		// for the top bar (max value)

		// e.g. Here I am plotting 60 red BOXs to output
        
       // cout << top << endl;
		//quicksort(this->bars, 0, this->size - 1);
        sort(this->bars, bars + size, greater<Bar>());
        //cout << "Sort FInished" << endl;
		double highVal = (double) this->bars[0].getValue();

		for (int g = 0; g < top; g++) {
			string color;

			if (colorMap.count(bars[g].getCategory())) {
				color = colorMap.at(bars[g].getCategory());
			} else {
				color = BLACK;
			}

			// you should get this from colorMap

			string barstr = "";

			double val = (double) this->bars[g].getValue();

			int numBox = lenMax * (val / highVal);

			for (int i = 0; i < numBox; i++) {
				barstr += BOX;
				//output << BOX;
			}

			output << color << barstr << " " << this->bars[g].getName() << " " << val << endl;

			//output << color << barstr << " " << this->bars[g].getName() << " " << val << endl;
		}
	}
};

#endif
