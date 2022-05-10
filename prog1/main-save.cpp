/* ---------------------------------------------
Program 1: DNA analyzer
Course: CS 251, Spring 2022. Thurs 12pm lab
System: Windows 10 CLion
Author: Zain Ali

program commands
	load_db
		load DNA counts for various people
	load_DNA
		load full DNA of one person
	display
 		see files loaded, and DNA processed
		and see total DNA counts for count_all

vv commands require load_db and load_DNA first vv

	process
		find longest DNA segment streaks in loaded DNA
	search
		find matching DNA
	count_all *creative component*
		find counts of all 4letter DNA segment counts for loaded DNA
		use display to see results

---------------------------------------------
*/

#include <iostream>
#include <fstream>
#include <string>
#include "ourvector.h"

using namespace std;

// function separates data from line with delimiters
string delimit(string& thingToDelimit, char delimiter = ',') {
	string::size_type pos = thingToDelimit.find(delimiter);

	string delimitedString;

	if (pos != string::npos) {
		delimitedString = thingToDelimit.substr(0, pos);
		thingToDelimit.erase(0, pos + 1);
	} else {
		delimitedString = thingToDelimit;
		thingToDelimit.erase();
	}

	return delimitedString;
}

// class that holds data for a person
class personData {
	public:
	ourvector<char> name;
	ourvector<int> dnaCounts;

	personData() = default;

	// constructs a personData obj with a line from the file
	personData(string lineFromFile) {
		string nameString = delimit(lineFromFile);

		for (char letter : nameString) {
			name.push_back(letter);
		}

		while (!lineFromFile.empty()) {
			this->dnaCounts.push_back(stoi(delimit(lineFromFile)));
		}
	}

	// prints name for the obj
	void printName() {
		for (char letter : this->name) {
			cout << letter;
		}
	}

	// prints name and DNA counts for obj
	void print() {
		this->printName();

		for (int dnaCount : this->dnaCounts) {
			cout << " " << dnaCount;
		}

		cout << endl;
	}

	// checks if DNA counts match this person
	bool match(ourvector<int>& strCounts) {
		for (int i = 0; i < strCounts.size(); i++) {
			if (strCounts.at(i) != this->dnaCounts.at(i)) {
				return false;
			}
		}
		return true;
	}
};

// class that holds data for a DNA sequence
class DNAsequence {
	public:
	ourvector<char> strLetters;

	DNAsequence() = default;

	// create DNAsequence from a string from file
	DNAsequence(string strString) {
		for (char letter : strString) {
			strLetters.push_back(letter);
		}
	}

	// print letters
	void printLetters() {
		for (char letter : this->strLetters) {
			cout << letter;
		}
	}

	// return true if sequence is empty
	bool empty() {
		return strLetters.size() == 0;
	}
	// returns total number of letters in sequence
	int size() {
		return strLetters.size();
	}

	// returns a specific character at an index of sequence
	char at(int i) {
		return strLetters.at(i);
	}

	// this is to check if these things are equal
	bool operator==(DNAsequence& thingToCompare) {
		if (strLetters.size() != thingToCompare.size()) {
			return false;
		}

		for (int i = 0; i < thingToCompare.size(); i++) {
			if (thingToCompare.at(i) != this->strLetters.at(i)) {
				return false;
			}
		}

		return true;
	}

	// this is to check if a small segment of dna matches this one starting at an index
	bool compare(int startIndex, DNAsequence& thingToCompare) {
		for (int index = 0; index < thingToCompare.size(); index++) {
			if (thingToCompare.at(index) != this->strLetters.at(index + startIndex)) {
				return false;
			}
		}

		return true;
	}

	// clears the segment of DNA
	void clear() {
		this->strLetters.clear();
	}
};

// loadDB, loads a database file with names and dna segment counts
void loadDB(
		string filePath,
		ourvector<DNAsequence> &personSegments,
		ourvector<personData>& fileData
		) {
	// handle file finding
	ifstream infile(filePath);

	cout << "Loading database..." << endl;

	string value;
	infile >> value;

	if (!infile.is_open()) {
		cout << "Error: unable to open " << "'" << filePath << "'" << endl;
	}

	personSegments.clear();
	fileData.clear();

	// parse header
	string header = value;

	delimit(header);

	while (!header.empty()) {
		DNAsequence str = DNAsequence(delimit(header));

		personSegments.push_back(str);
	}

	// parse every line after header
	infile >> value;

	while (!infile.eof() && infile.is_open()) {
		personData myDude = personData(value);

		fileData.push_back(myDude);

		infile >> value;
	}

}

// loads a DNA file
void loadDNA(string filePath, DNAsequence& loadedDNA) {
	ifstream infile(filePath);

	cout << "Loading DNA..." << endl;

	if (!infile.is_open()) {
		cout << "Error: unable to open " << "'" << filePath << "'" << endl;

	} else {
		loadedDNA.clear();

		string value;
		infile >> value;

		loadedDNA = DNAsequence(value);
	}

}

// displays the progress of this program eg. files loaded and processed
void display(
		ourvector<personData>& fileData,
		DNAsequence& loadedDNA,
		ourvector<int>& segmentCount,
		ourvector<DNAsequence>& segments
		) {
	// print database
	if (fileData.size() > 0) {
		cout << "Database loaded: " << endl;

		for (personData dude : fileData) {
			dude.print();
		}

	} else {
		cout << "No database loaded.";
	}

	cout << endl;

	// print loaded DNA
	if (!loadedDNA.empty()) {
		cout << "DNA loaded: " << endl;
		loadedDNA.printLetters();

	} else {
		cout << "No DNA loaded.";
	}

	cout << endl;

	// print process results
	if (segmentCount.size() != 0) {
		cout << "DNA processed, STR counts: " << endl;

		for (int i = 0; i < segments.size(); i++) {
			segments.at(i).printLetters();
			cout << ": " << segmentCount.at(i) << endl;
		}

	} else {
		cout << endl <<
			 "No DNA has been processed.";
	}

	cout << endl;
}

// this thing uses already processed DNA and searches the peopleDB for a DNA match
void search(
		ourvector<int>& strCounts,
		ourvector<personData>& fileData,
		DNAsequence& loadedDNA
		) {
	// check prerequisites
	if (fileData.size() == 0) {
		cout << "No database loaded." << endl;

		return;

	} else if (loadedDNA.empty()) {
		cout << "No DNA loaded." << endl;

		return;

	} else if (strCounts.size() == 0) {
		cout << "No DNA processed." << endl;

		return;
	}

	cout << "Searching database...\n";

	// search the database for person with matching DNA streaks
	for (auto & person : fileData) {
		if (person.match(strCounts)) {
			cout << "Found in database!  DNA matches: ";
			person.printName();
			cout << endl;
			return;
		}
	}

	cout << "Not found in database." << endl;
}

// finds the longest streak of a segment in a larger chunk of DNA
int const getLongestStreak(DNAsequence& segment, DNAsequence& DNA) {
	int longestStreak = 0;
	int currentStreak = 0;

	for (int searchIndex = 0; searchIndex < DNA.size() - segment.size() + 1;) {
		if (DNA.compare(searchIndex, segment)) {
			searchIndex += segment.size();
			currentStreak += 1;

		} else {
			searchIndex += 1;

			if (currentStreak > longestStreak) {
				longestStreak = currentStreak;
			}

			currentStreak = 0;
		}
	}

	if (currentStreak > longestStreak) {
		longestStreak = currentStreak;
	}

	return longestStreak;
}

// DNA segment counts in from loaded DNA sequence, display to see results
void process(DNAsequence& DNA, ourvector<DNAsequence>& strs, ourvector<int>& data) {

	// check prerequisites
	if (strs.size() == 0) {
		cout << "No database loaded." << endl;

		return;
	} else if (DNA.empty()) {
		cout << "No DNA loaded." << endl;

		return;
	}

	cout << "Processing DNA..." << endl;

	// count the streaks
	for (DNAsequence& str : strs) {
		data.push_back(getLongestStreak(str, DNA));
	}

}

// counts total # of all 4 char DNA segments, display to see results
void countAll(
		DNAsequence& DNA,
		ourvector<int>& strCounts,
		ourvector<DNAsequence>& strs
		) {
	strs.clear();
	strCounts.clear();

	ourvector<int> lastCounted;

	// for every DNA segment we find, record its existence and add 1 to existing things
	for (int searchIndex = 0; searchIndex < DNA.size() - 3; searchIndex++) {
		string strString = "";

		// construct DNAsequence we are looking at
		for (int i = 0; i < 4; i++) {
			strString += (char)DNA.at(searchIndex + i);
		}

		DNAsequence thingToAdd(strString);

		int index = 0;

		// if we found this segment before add one to its count
		for (; index < strs.size(); index++) {
			if (strs.at(index) == thingToAdd) {
				if (abs(lastCounted.at(index) - searchIndex) > 3) {
					strCounts.at(index) += 1;
					lastCounted.at(index) = searchIndex;
				}
				break;
			}
		}

		// if we did not find this segment before, add it to the thing
		if (index == strs.size()) {
			strs.push_back(thingToAdd);
			strCounts.push_back(1);
			lastCounted.push_back(searchIndex);
		}
	}
}

int main() {
	cout << "Welcome to the DNA Profiling Application." << endl;

	// loaded Data is all stored and modified here
	ourvector<personData> personDB;
	DNAsequence loadedDNA;
	ourvector<int> segmentCounts;
	ourvector<DNAsequence> personSegments;

	string option = "default";

	// menu selector
	while (option != "#") {
		cout << "Enter command or # to exit: ";

		cin >> option;
		string filePath;

		// option.find helps with whitespace
		 if (option.find("display") != string::npos) {
			display(personDB, loadedDNA, segmentCounts, personSegments);

		} else if (option.find("process") != string::npos) {
			process(loadedDNA, personSegments, segmentCounts);

		} else if (option.find("search") != string::npos) {
			search(segmentCounts, personDB, loadedDNA);

		} else if (option.find("load_dna") != string::npos) {
			cin >> filePath;
			loadDNA(filePath, loadedDNA);

		} else if (option.find("load_db") != string::npos) {
			cin >> filePath;
			 loadDB(filePath, personSegments, personDB);

		} else if (option.find("count_all") != string::npos) {
			countAll(loadedDNA, segmentCounts, personSegments);
		}
	}

	return 0;
}
