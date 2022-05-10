// barchartanimate.h
// TO DO:  add header comment here.  Also add function header comments below.
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <unistd.h>
#include "myrandom.h" // used in autograder, do not remove
#include "barchart.h"

#ifndef barchartanimate_h
#define barchartanimate_h

using namespace std;
//
// BarChartAnimate
//
class BarChartAnimate {
private:
    //
    // Private member variables for the abstraction.
    // This implementation uses a low-level C array, bars, to store a list of
    // BarCharts.  As a result, you must also keep track of the number of
    // elements stored (size) and the capacity of the array (capacity).
    // This IS dynamic array, so it must expand automatically, as needed.
    
    BarChart *barcharts;  // pointer to a C-style array
    int size;
    int capacity;
    map<string, string> colorMap;
    int colorIndex = 0;
    string title, xlabel, source;
public:
    
    // a parameterized constructor:
    // Like the ourvector, the barcharts C-array should be constructed here
    // with a capacity of 4.
    BarChartAnimate(string title, string xlabel, string source) {

        this->size = 0;
        this->capacity = 4;
		this->barcharts = new BarChart[this->capacity];

        this->title = title;
        this->xlabel = xlabel;
        this->source = source;
        
    }
    
    //
    // destructor:
    //
    // Called automatically by C++ to free the memory associated
    // by BarChartAnimate.
    //


	void resize() {
		this->capacity *= 2;

		BarChart* newCharts = new BarChart[this->capacity];

		for (int g = 0;g < this->size; g++) {
			newCharts[g] = this->barcharts[g];
		}

		delete[] this->barcharts;

		this->barcharts = newCharts;
	}

    virtual ~BarChartAnimate() {
        
        for (int i = 0; i < this->size; i++){
            //this->barcharts[i].clear();
            BarChart bc = barcharts[i];
            bc.clear();
        }
        delete[] barcharts;
    }
    
    // addFrame:
    // adds a new BarChart to the BarChartAnimate object from the file stream.
    // if the barcharts has run out of space, double the capacity (see
    // ourvector.h for how to double the capacity).
    // See application.cpp and handout for pre and post conditions.
    void addFrame(ifstream &file) {
        
        string value;

		if (this->size + 1 > this->capacity) {
			resize();
		}

        if (!file.is_open()) {
            cout << "Error: unable to open file addFrame () 79" << endl;
            return;
        }

        getline(file, value);
        getline(file, value);
        
        int num = 0;
        if (!file.eof() && file.is_open()){
            num = stoi(value);
        }
        else{
            return;
        }

        BarChart bc(num);
        
        for (int i = 0; i < num && !file.eof(); i++) {
            
            getline(file, value, ',');
            bc.setFrame(value);
            
            string name, country, category, stVal;
            int value;
            
            getline(file, name, ',');
            getline(file, country, ',');
            getline(file, stVal, ',');
            value = stoi(stVal);
            getline(file, category);

			bc.addBar(name, value, category);

            if (this->colorMap.count(category) == 0) {
                //colorIndex %= (COLORS.size());
                this->colorMap.emplace(
						pair<string,string>(
								category,
								COLORS.at(colorIndex % COLORS.size())));
                colorIndex += 1;
            }
            
        }
        
        barcharts[this->size] = bc;
        
        this->size++;
    }
    
    //}
    // animate:
    // this function plays each frame stored in barcharts.  In order to see the
    // animation in the terminal, you must pause between each frame.  To do so,
    // type:  usleep(3 * microsecond);
    // Additionally, in order for each frame to print in the same spot in the
    // terminal (at the bottom), you will need to type: output << CLEARCONSOLE;
    // in between each frame.
    void animate(ostream &output, int top, int endIter) {
        
        if (endIter == -1 || endIter > this->size) {
            endIter = this->size;
        }
        
        unsigned int microsecond = 50000;
        
        int delay = 3;
        
        for (int i = 0; i < endIter; i++) {
            
            usleep(delay* microsecond);

            output << CLEARCONSOLE << BLACK << this->title << endl << this->source << endl;
            
            this->barcharts[i].graph(output, this->colorMap, top);
            
            output << BLACK << this->xlabel << endl << "Frame: " << this->barcharts[i].getFrame() << endl;
            
        }
        
    }
    
    // Returns the size of the BarChartAnimate object.
    int getSize() {
        
        return this->size;  // TO DO:  update this, it is only here so code compiles.
        
    }
    
    BarChart &operator[](int i) {

		if (i < 0 || i > this->size) {
			throw out_of_range("BarChart [" + to_string(i) +"]" + "Called out of range");
		}

        return this->barcharts[i];
    }
};
#endif
