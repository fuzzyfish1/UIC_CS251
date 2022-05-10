#pragma once

#include <iostream>
#include "barchartanimate.h"
#include "barchart.h"

using namespace std;

bool testBarDefaultConstructor (int& totalFail) {

	Bar b;
	cout << "Bar b;" << endl;

    if (!b.getName().empty()) {
        cout << "\tgetName failed" << endl;
        totalFail += 1;
    } else {
		cout << "\tgetName Pass" << endl;
	}

	if (b.getValue() != 0) {
        cout << "\tgetValue failed" << endl;
		totalFail += 1;
    } else {
		cout << "\tgetValue passed" << endl;
	}

	if (!b.getCategory().empty()) {
        cout << "\tgetCategory failed" << endl;
		totalFail += 1;
    } else {
		cout << "\tgetCategory Pass" << endl;
	}

	cout << endl;

    return true;
}

bool testBarParamConstructor(int& totalFail) {

    Bar b("Chicago", 9234324, "US");
	cout << "Bar b(\"Chicago\", 9234324, \"US\");" << endl;

    if (b.getName() != "Chicago") {
        cout << "\tgetName failed" << endl;
        totalFail += 1;
    } else {
		cout << "\tgetName Pass" << endl;
	}

	if (b.getValue() != 9234324) {
        cout << "\tgetValue failed" << endl;
		totalFail += 1;
    } else {
		cout << "\tgetValue pass" << endl;
	}

	if (b.getCategory() != "US") {
        cout << "\tgetCategory failed" << endl;
		totalFail += 1;
    } else {
		cout << "\tgetCategory pass" << endl;
	};

	cout << endl;
    return true;
}

bool testBargreater(int& totalFail) {

    Bar b1("Chicago", 1023430, "US");
    Bar b2("NYC", 923430, "US");

	cout << "Bar::operator > ()" << endl;

    bool lessThanT = (b1 > b2);

    if (!lessThanT) {
        cout << "\t Fail True" << endl;
		totalFail += 1;
    } else {
		cout << "\t Pass True" << endl;
	}

	bool lessThanF = !(b2 > b1);

    if (!lessThanF) {
		cout << "\t Fail False" << endl;
		totalFail += 1;
    } else {
		cout << "\t Pass False" << endl;
	}

	cout << endl;

    return true;
}

bool testBarLess(int& totalFail) {

    Bar b1("Chicago", 92343, "US");
    Bar b2("NYC", 923430, "US");

	cout << "Bar:: < () {}" << endl;

    bool lessThanT = (b1 < b2);

    if (!lessThanT) {
        cout << "\tTrue Fail" << endl;
        totalFail += 1;
    } else {
		cout << "\tTrue Pass" << endl;
	}

	bool lessThanF = !(b2 < b1);

    if (!lessThanF) {
        cout << "\tFalse Fail" << endl;
        totalFail += 1;
    } else {
		cout << "\tFalse Fail" << endl;
	}
	cout << endl;
    return true;
}

bool testBarLessEq(int& totalFail) {

    Bar b1("Chicago", 92343, "US");
    Bar b2("NYC", 923430, "US");
    Bar b3("BingusLand", 923430, "US");

	cout << "Bar:: operator <= ()" << endl;

    bool lessThanT = (b1 <= b2);

    if (!lessThanT) {
        cout << "\t < True Fail" << endl;
        totalFail += 1;
    } else {
		cout << "\t< True Pass" << endl;
	}

	bool lessThanF = !(b2 <= b1);

    if (!lessThanF) {
        cout << "\t== chunk False Fail" << endl;
        totalFail += 1;
    } else {
		cout << "\t== chunk False Pass" << endl;
	}

	bool lessThanEqT = (b3 <= b2);


    if (!lessThanEqT) {
        cout << "\t == chunk Fail" << endl;
        totalFail += 1;
    } else {
		cout << "\t == chunk Pass" << endl;
	}

	bool lessThanEqTr = (b2 <= b3);

    if (!lessThanEqTr) {
		cout << "\t == chunk Fail" << endl;
        totalFail += 1;
    } else {
		cout << "\t== chunk Pass" << endl;
	}
	cout << endl;
    return true;
}

bool testBargreaterEq(int& totalFail) {

    Bar b1("Chicago", 92343, "US");
    Bar b2("NYC", 923430, "US");
    Bar b3("BingusLand", 923430, "US");

	cout << "Bar:: operator >= ()" << endl;

    bool lessThanT = (b2 >= b1);

    if (!lessThanT) {
        cout << " > True Fail" << endl;
        totalFail += 1;
    } else {
		cout << " > True Pass" << endl;
	}

	bool lessThanF = !(b1 >= b2);
	cout << "> False";

	if (!lessThanF) {
        cout << "\tFail" << endl;
        totalFail += 1;
    } else {
		cout << "\tPass" << endl;
	}

	bool lessThanEqT = (b3 >= b2);
	cout << "less ==";

	if (!lessThanEqT) {
		cout << "\t == Fail" << endl;
		totalFail += 1;
	} else {
		cout << "\t == Pass" << endl;
	}

	bool lessThanEqTr = (b2 >= b3);
	cout << "\tless == reverse";

	if (!lessThanEqTr) {
		cout << "\t == Fail" << endl;
		totalFail += 1;
	} else {
		cout << "\t == Pass" << endl;
	}

    return true;
}

bool testBarChartConstruct(int& totalFail) {

    BarChart b;
	cout << "BarChart b" << endl;

	cout << "getName ()";
    if (b.getSize() != 0) {
        cout << "\tFail" << endl;
        totalFail += 1;
    } else {
		cout << "Pass" << endl;
	}

	cout << "getFrame()";

	if (!b.getFrame().empty()) {
		cout << "\tFail" << endl;
		totalFail += 1;
	} else {
		cout << "\tPass" << endl;
	}

	cout << endl;

    return true;
}

bool compFile(ofstream f1, ofstream f2) {

}

bool testBarChartConstructN(int& totalFail) {

    BarChart fill(2);
	cout << " BarChart fill(2)";

    stringstream strAfter;
    stringstream strBefore;

    fill.addBar("Chicago", 92343, "US");
    fill.addBar("NYC", 923430, "US");

    fill.dump(strBefore);

    fill.addBar("BingusLand", 923430, "US");

    fill.dump(strAfter);

    if (strAfter.str() != strBefore.str()) {
        cout << "Fail" << endl;
        totalFail += 1;
    } else {
		cout << "Pass" << endl;
	}

    cout << endl;

    return true;

}

bool testBarChartCopy(int& totalFail) {

	stringstream strAfter;
	stringstream strBefore;

	BarChart b1(1);

	b1.addBar("Miami",2, "US");
	b1.addBar("chicago", 4, "US");

	BarChart b2(b1);
	b2.dump(strBefore);

	b1.addBar("bingus", 6, "US");

	b1.dump(strAfter);

	cout << "";

	if (b1.getSize() != 3) {

		totalFail += 1;
	}

	if (b2.getSize() != 2) {
		totalFail += 1;
	}

	if (strAfter.str() == strBefore.str()) {
		totalFail += 1;
	}

}

bool testBarChartClear(int& totalFail) {

	BarChart b1(10);
	b1.addBar("asdf;klj",19,"asdfasdf");

	b1.clear();

	if (b1.getFrame() != "") {
		cout << "getFucked" << endl;
	}

	if (b1.getSize() != 0) {}

	if (b1.getFrame() != "") {}

	return true;

}

bool testBarChartOpEqual(int& totalFail) {

    //1
    BarChart c1(2);
    BarChart c2(3);

	cout << "BarChart::operator =()" << endl;

    stringstream strAfter;
    stringstream strBefore;

    c1.addBar("Chicago", 92343, "US");
    c1.addBar("NYC", 923430, "US");
    c2.addBar("BingusLand", 923430, "US");
    c1.dump(strBefore);

    c2 = c1;
    c2.dump(strAfter);

	cout << "add, set =, compare";
    if (strAfter.str() != strBefore.str()) {
        cout << "\tFail" << endl;
        totalFail += 1;
    } else {
		cout << "\tPass" << endl;
	}

    //2
    BarChart c3(10);
    BarChart c4(10);

    stringstream actual1;
    stringstream correct1;

    c3.addBar("Chicago", 92343, "US");
    c3.addBar("NYC", 923430, "US");

	c4.addBar("BingusLand", 923430, "US");
    c4 = c3;
    c4.dump(correct1);

    c3.addBar("Chicagoy", 923431, "USy");
    c3.addBar("NYCy", 9234301, "USy");
    c3.dump(actual1);

    cout << "add to both, set =, add to one, compare" << endl;

	if (actual1.str() == correct1.str()) {
		cout << "\tFail" << endl;
        totalFail += 1;
    } else {
		cout << "\tPass" << endl;
	}

	cout << "getSize()" << endl;
    if (c4.getSize() != 2) {
        cout << "\tFail" << endl;
        totalFail += 1;
    } else {
		cout << "\tPass" << endl;
	}


    if (c3.getSize() != 4) {
        cout << "\tFail" << endl;
        totalFail += 1;
    } else {
		cout << "\tPass" << endl;
	}

    //3
    BarChart c5(10);

    stringstream after2;
    stringstream before2;

    c5.addBar("Chicago", 92343, "US");
    c5.addBar("NYC", 923430, "US");
    c5.addBar("BingusLand", 923430, "US");
    c5.dump(before2);

	cout << c5.getSize() << endl;
    c5 = c5;

	cout << c5.getSize() << endl;
	cout << "c5 = c5";

    if (after2.str() != before2.str()) {
        cout << "\tFail" << endl;
        totalFail += 1;
    } else {
		cout << "\tPass" << endl;
	}

    cout << endl;

    return true;



    //ostringstream iSS;
/*
    map<string, string> colorMap;
    colorMap.emplace("beep", BLUE);
    map<string, string> colorMap2;
    colorMap2.emplace("beep", PURPLE);

    BarChart myChart(15);

    myChart.addBar("1000", 10000, "beep");
    */
    //myChart.addBar("9997", 9997, "beep");
    //myChart.addBar("9996", 9996, "beep");
    //myChart.addBar("9995", 9995, "beep");
    //myChart.addBar("9993", 9993, "beep");
    //myChart.addBar("9991", 9991, "beep");
    //myChart.addBar("9990", 9990, "beep");
    //myChart.addBar("9989", 9989, "beep");
    //myChart.addBar("9987", 9987, "beep");
    //myChart.addBar("9986", 9986, "beep");
    //myChart.addBar("9985", 9985, "beep");
    //myChart.addBar("9983", 9983, "beep");
/*
    BarChart fill2 = myChart;

    myChart.graph(cout, colorMap, 12);
    cout << endl;
    fill2.graph(cout, colorMap2, 12);

    if (myChart.getSize() != 1) {
        cout << " Size Death" << endl;
        totalFail += 1;

    }
	/* else if (fill.getCapacity() <= 0) {
        cout << " Capacity Death" << endl;
        totalFail += 1;

    }

    cout << " all passed!" << endl;

    return true;
*/
    
}

bool setgetBarChartFrame(int& totalFail) {

	BarChart b1;

	b1.setFrame("bleep");

	if (b1.getFrame() != "bleep") {}

}

//add Bar, getSize
bool BarChartAddBar(int& totalFail) {


	int size = 0;
	BarChart b1;

	b1.addBar("bingus", 10, "bingusBoop");
	size ++;
	if (b1.getSize() != size) {}

	b1.addBar("bingu", 1, "bingusBoo");
	size ++;
	if (b1.getSize() != size) {}

	b1.addBar("bing", 0, "bingusBo");
	size ++;
	if (b1.getSize() != size) {}

	b1.addBar("bin", 0, "bingusB");
	size ++;
	if (b1.getSize() != size) {}

	b1.addBar("bi", 0, "bingus");

	size ++;
	if (b1.getSize() != size) {}

	return true;

}

bool BarChartOpAt(int& totalFail) {

	Bar bar1("bingus", 10, "bingusBoop");
	BarChart b1;

	b1.addBar("bingus", 10, "bingusBoop");

	if (b1[0].getName() != bar1.getName()) {

	}
	cout << "end b1[0] 1" << endl;
	if (b1[0].getValue() != bar1.getValue()) {

	}
	cout << "end b1[0] 2" << endl;
	if (b1[0].getCategory() != bar1.getCategory()) {

	}

	cout << "end b1[0] 3" << endl;
	return true;

}

bool BarChartGraph(int& totalFail) {
	return true;
}

bool BarChartAnimateConstruct(int& totalFail) {

	BarChartAnimate bca1("beep","beep","beep");
	return true;
}

bool deleteBarCharAnimate(int& totalFail) {
	return true;
}

// also getSize
bool BarChartAnimateAddFrame(int& totalFail) {
	return true;
}

bool BarChartAnimateAnimate(int& totalFail) {
	return true;
}

bool barChartAnimateat(int& totalFail) {
	return true;
}

int main() {

	int fail = 0;

	//expiriment();

    testBarDefaultConstructor(fail);
    testBarParamConstructor(fail);

    testBargreater(fail);
    testBarLess(fail);
    testBarLessEq(fail);
    testBargreaterEq(fail);

	testBarChartConstruct(fail);
	testBarChartConstructN(fail);
	testBarChartOpEqual(fail);
	testBarChartCopy(fail);

	testBarChartClear(fail);
	//deleteBarChart(fail);
	setgetBarChartFrame(fail);

	BarChartAddBar(fail);
	BarChartOpAt(fail);
	//BarChartDump(fail);
	BarChartGraph(fail);

	BarChartAnimateConstruct(fail);
	deleteBarCharAnimate(fail);

	BarChartAnimateAddFrame(fail);
	BarChartAnimateAnimate(fail);
	barChartAnimateat(fail);

	cout << "fuck this shit" << endl;



	cout << "totalFail: " << fail;

	/*
    testBarChartConstruct();
    testBarChartConstructN();
    testBarChartOpEqual();
    
    string filename = "/Users/angadgakhal/Downloads/Project 3 - Animated Bar Chart Starter Code/cities.txt";
    ifstream inFile(filename);
    string title;
    getline(inFile, title);
    string xlabel;
    getline(inFile, xlabel);
    string source;
    getline(inFile, source);
    BarChartAnimate bca(title, xlabel, source);
    while (!inFile.eof()) {
        bca.addFrame(inFile);
    }
   cout << (bca.getSize());
*/
    /*
    string filename = "./football.txt";
    ifstream inFile(filename);
    string title;
    getline(inFile, title);
    string xlabel;
    getline(inFile, xlabel);
    string source;
    getline(inFile, source);
    BarChartAnimate bca(title, xlabel, source);

	while (!inFile.eof()) {
        bca.addFrame(inFile);
    }

	bca.animate(cout, 12, 10);
    */
    
    //testBarChartCopy();


    //testBarChartOpEqual();




/*
    map<string, string> colorMap;
    colorMap.emplace("cat", CYAN);

    BarChart myChart(5);
    myChart.addBar("name", 10, "cat");

    myChart.graph(cout,colorMap, 2);
*/
    //myChart.graph(cout, );

    //ifstream file("./cities.txt");
    //string title, source, units, val;
    //getline(file, title);
    //getline(file, units);
    //getline(file, source);
    //getline(file, val);
//
    //BarChartAnimate myAnime(title, units, source);
    //myAnime.addFrame(file);
//
    //myAnime.animate(cout, 12, -1);

    //cout << "test2" << endl;

    return 0;

}
