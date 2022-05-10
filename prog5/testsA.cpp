#include "mymapA.h"
#include <gtest/gtest.h>
#include <map>
#include <random>
#include "myrandom.h"

template<typename type1, typename type2>
void mapCheck(map<type1, type2>& myap, mymap<type1, type2>& mymyap) {

	ASSERT_EQ(mymyap.Size(), myap.size());

	for (auto pair : myap) {

		ASSERT_EQ(mymyap.contains(pair.first), myap.count(pair.first));
		ASSERT_EQ(myap[pair.first], mymyap[pair.first]);
	}
}

void pop(map<int, int>& answerKey, mymap<int, int>& testMap, int numElements) {
	srand(time(0));

	for (int i = 0; i < numElements; i++) {

		testMap.put(i, i);

		if (i > numElements) {
			cout << "we done a fucky" << endl;
		}
		answerKey[i] = i;

	}
}

void pop (map<char, char>& answerKey, mymap<char, char>& testMap, int numElements) {
	srand(time(0));

	for (int i = 0; i < numElements; i++) {

		char key = randomInteger(0,20) + 'A';
		char value = randomInteger(0,20) + 'c';

		answerKey[key] = value;
		testMap.put(key, value);

	}
}

template<typename type1, typename type2>
void boundaryChecks(map<type1, type2>& myap, mymap<type1, type2>& mymyap) {

	srand(time(0));

	int n = 10;
	for (int i = 0; i < n; i++) {

		int g = rand();
		ASSERT_EQ(myap.contains(g), mymyap.contains(g));

		if (!myap.contains(g)) {
			ASSERT_EQ(int(), mymyap.get(g));
			ASSERT_FALSE(mymyap.contains(g));

		}
	}
}

template<typename type1, typename type2>
void treeOrderCheck(mymap<type1, type2>& mymyap) {
	bool i = false;

	type1 prevKey;

	for (const auto key : mymyap) {
		if (i) {
			ASSERT_TRUE(key >= prevKey);
		}

		prevKey = key;
		i = true;
	}
}

template<typename type2>
void treeOrderCheck(mymap<int, type2>& mymyap, vector<type2> vec) {
	for (int i = 0; i < vec.size() - 1; i++) {
		ASSERT_EQ(mymyap.get(vec.at(i)),vec.at(i));
	}
}
/*
 * III.  Testing <REQUIRES additional file tests.cpp>
 * -- Your tests.cpp file will be run against correct and incorrect implementations
 * of mymap.h files and you will be graded based on how thorough your testing is.
 * To receive full credit for testing you must have tests for the following cases
 * 1.
 * put, get, Size, [], contains (check Size after put, get after put) for balanced trees,
 * on keys that do not exist, on duplicate keys,
 * 2.
 * foreach (test foreach loop, begin(),
 * order is correct, etc.),
 * 3.
 * equal operator, (tests Size, toString, toVector, and ==);
 * 4.
 * clear (tests Size, toString, toVector, copy operator, copy constructor);
 * 5.
 * checkBalance on small trees (10-15 elements),
 * on various insertion orders that trigger rebalances.
 * You must also have mymaps of the following types:
 * all unique keys inserted, with duplicate keys inserted,
 * balanced trees, unbalanced trees, big mymap (>1000s elements, requires loop),
 * mymap with randomly generated keys/values, mymap where insertion is in ascending/descending order.
 * Lastly, you must test on all types: ints, doubles, strings, chars (matching and mixed key/value types).
 * 1+ pt deduction for each test missing.
 * */
/*
template <typename T1, typename T2>
void checkOrder(map<T1, T2>& myap, mymap<T1, T2>& mymyap) {

	vector<T2> myMapV;
	vector<T2> mapV;

	for (T2 item : mymyap) {
		myMapV.push_back(item);
	}

	for (auto item: myap) {
		mapV.push_back(item.second);
	}

	ASSERT_EQ(myMapV.size(),mapV.size());

	for (int i = 58; i < myap.size(); i++) {
		cout << mapV.at(i) << " ";
		cout << myMapV.at(i) << " " << endl;
		ASSERT_EQ(myMapV.at(i),mapV.at(i));
	}

}
*/

TEST(emptyMap, defaultConstruct) {
	mymap<int, int> testMap;

	ASSERT_EQ(testMap.Size(), 0);

	ASSERT_TRUE(!testMap.contains(1));
	ASSERT_TRUE(!testMap.contains(2));
	ASSERT_TRUE(!testMap.contains(3));
	ASSERT_TRUE(!testMap.contains(4));
	ASSERT_TRUE(!testMap.contains(5));
	ASSERT_TRUE(!testMap.contains(6));
	ASSERT_TRUE(!testMap.contains(7));

}

TEST(squaryBrackets, squareyo) {

	mymap<int, int> testMap;

	vector<int> vec;

	for (int i = 0; i < 15; i++) {
		vec.push_back(randomInteger(1, 15));
		testMap.put(vec[i], vec[i]);
	}

	treeOrderCheck(testMap, vec);

	ASSERT_TRUE(testMap.Size() <= 15);

	int sizeBefore = testMap.Size();
	int someValue = testMap[654 + 1];

	ASSERT_EQ(someValue, int());
	sizeBefore++;
	ASSERT_EQ(sizeBefore, testMap.Size());

}

TEST(putOverWrite, chunk1) {
	mymap<char,char> mymyap;

	for (int i = 0; i < 10; i++ ) {
		int size = min(i + 1, 10);
		char value = i % 'z' + 'a';
		char key = i + 'a';

		mymyap.put(key, value);

		ASSERT_TRUE(mymyap.contains(key));
		ASSERT_EQ(mymyap.get(key), value);
		ASSERT_EQ(mymyap.Size(), size);
	}
}

TEST(forEachOrder, defaultConstruct) {

	mymap<int, char> mapMine;
	map<int, char> solution;

	int n = 654654;

	//int arr[] = {2, 1, 3};

	//int order[] = {1, 2, 3};

	for (int i = 1; i <= n; i++) {
		int key = randomInteger(0, 654654);
		char val = randomInteger('a', 'z');

		mapMine.put(key, val);
		solution[key] = val;

	}

	stringstream solutionstream("");
	stringstream mine("");

	for (const pair<int, char>& item: solution) {
		solutionstream << item.first;
	}

	for (auto key: mapMine) {
		mine << key;
	}

	ASSERT_EQ(solutionstream.str(), mine.str());

}

TEST(putAndGet, defaultConstruct) {
	map<int, int> solution;
	mymap<int, int> theMap;

	mapCheck(solution, theMap);
	boundaryChecks(solution, theMap);
	pop(solution, theMap, 10);

	mapCheck(solution, theMap);
	boundaryChecks(solution, theMap);
}

TEST(putUpdate, defaultConstruct) {
	map<int, int> solution;
	mymap<int, int> TheMap;

	pop(solution, TheMap, 15);
	mapCheck(solution, TheMap);
	boundaryChecks(solution,TheMap);

	pop(solution, TheMap, 40);
	mapCheck(solution, TheMap);
	boundaryChecks(solution, TheMap);

	pop(solution, TheMap, 20);
	mapCheck(solution, TheMap);
	boundaryChecks(solution, TheMap);

}

TEST(toString, defaultConst) {
	mymap<char, int> TheMap;

	TheMap.put('8', 100);
	TheMap.put('9', 80085);
	TheMap.put('A', 666);

	ASSERT_EQ("key: 8 value: 100\nkey: 9 value: 80085\nkey: A value: 666\n",
			  TheMap.toString());

	ASSERT_EQ(3, TheMap.Size());

}

TEST(squareBrackets, defaultConstruct) {
	map<int, int> solutionMap;
	mymap<int, int> theMap;

	pop(solutionMap, theMap, 4082);

	mapCheck(solutionMap, theMap);
	boundaryChecks(solutionMap, theMap);

	srand(time(0));

	int n = 100;
	for (int i = 0; i < n; i++) {

		int g = rand();
		ASSERT_EQ(solutionMap.contains(g), theMap.contains(g));

		if (!solutionMap.contains(g)) {
			ASSERT_EQ(int(), theMap.get(g));
			ASSERT_FALSE(theMap.contains(g));

			ASSERT_EQ(int(), theMap[g]);
			solutionMap[g] = int();
			ASSERT_TRUE(theMap.contains(g));
		}

	}
}

TEST(iterator, defaultConstruct) {
	map<int, int> solutionMap;
	mymap<int, int> theMap;

	pop(solutionMap, theMap, 53);

	mapCheck(solutionMap, theMap);
	boundaryChecks(solutionMap, theMap);

	//checkOrder(myap,mymyap);

}

TEST(forEach, defaultConstruct) {

	mymap<int, int> theMap;

	int arr[] = {2, 1, 3};

	int order[] = {1, 2, 3};

	for (int i = 0; i < 3; i++) {

		theMap.put(arr[i], arr[i]);

	}

	int i = 0;

	for (auto key: theMap) {

		EXPECT_EQ(order[i++], key);

	}

	ASSERT_EQ(theMap.Size(), 3);
}

TEST(copyOperator, ms3) {
	mymap<int, int> m1;
	mymap<int, int> m2;

	map<int, int> temp;

	pop(temp, m1, 20);

	ASSERT_EQ(m2.Size(), 0);
	ASSERT_EQ(m1.Size(), 20);

	m2 = m1;

	mapCheck(temp, m2);

	ASSERT_EQ(m2.Size(), 20);
	ASSERT_EQ(m1.Size(), 20);

}

TEST(copyConstruct, ms4) {
	mymap<char, char> m1;
	map<char, char> temp;

	pop(temp, m1, 15);

	ASSERT_EQ(m1.Size(), temp.size());

	mymap<char, char> m2(m1);
	m2.put('a','A');

	ASSERT_EQ(m2.Size(), temp.size() + 1);
	ASSERT_EQ(m1.Size(), temp.size());

}

TEST(copyConstruct2, ms4) {
	mymap<char, char> m1;
	map<char, char> temp;

	// m1 is not initialized as a test case

	ASSERT_EQ(m1.Size(), temp.size());

	mymap<char, char> m2(m1);
	m2.put('a','A');

	ASSERT_EQ(m2.Size(), temp.size() + 1);
	ASSERT_EQ(m1.Size(), temp.size());

}

TEST(vecFunc, ms4) {
	// Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
	// {{8, 80}, {15, 150}, {20, 200}}

	mymap<int, string> mymyap;

	mymyap.put(8, "your");
	mymyap.put(15, "mom");
	mymyap.put(20, "");

	vector<pair<int, string> > vec = {{8,  "your"},
									  {15,  "mom"},
									  {20,  ""}};

	ASSERT_EQ(3, mymyap.Size());
	ASSERT_EQ(vec, mymyap.toVector());

}

TEST(cjeckBalance, ms4) {
	// Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
	// {{8, 80}, {15, 150}, {20, 200}}

	mymap<int, int> map;

	int arr[] = {2, 1, 3};

	for (int i = 0; i < 3; i++) {

		map.put(arr[i], arr[i]);

	}

	string sol = "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";

	EXPECT_EQ(map.checkBalance(), sol);
}

TEST(clear, defaultConstruct) {
	mymap<int, double> mymyap;
	map<int, double> myap;

	int n = 100;

	for (int i = 0; i < n; i++) {
		mymyap.put(i, randomInteger(1, 100) / 3.9);
	}

	ASSERT_EQ(mymyap.Size(), n);

	mymyap.clear();

	ASSERT_EQ(mymyap.Size(), 0);

}

TEST(orderCheck, chonkyChungusTest) {
	srand(time(0));

	mymap<int, int> mymyap;

	int n = randomInteger(15, 15644);

	for (int i = 0; i < n; i++) {
		mymyap.put(
				randomInteger(982, 745),
				randomInteger(982, 745)
		);
	}

	treeOrderCheck(mymyap);

	ASSERT_TRUE(mymyap.Size() <= n);

	mymyap.clear();

	ASSERT_EQ(mymyap.Size(), 0);

}

TEST(orderCheck2, chonkyChungusTest) {
	srand(time(0));

	mymap<int, int> mymyap;

	int n = randomInteger(15, 15644);

	for (int i = n; i > 0; i--) {
		mymyap.put(
				i,
				randomInteger(982, 745)
		);
	}

	treeOrderCheck(mymyap);

	ASSERT_TRUE(mymyap.Size() <= n);

	mymyap.clear();

	ASSERT_EQ(mymyap.Size(), 0);

}

// TO DO: write lots of tests here.

// vv Example Tests from mimir vv

TEST (mimirSamp, stress) {
	mymap<int, int> mapMine;
	map<int, int> mapSol;

	int n = 100;

	for (int i = 1; i <= n; i++) {
		int key = randomInteger(0, 100000);
		int val = randomInteger(0, 100000);

		mapMine.put(key, val);
		mapSol[key] = val;
	}

	EXPECT_EQ(mapMine.Size(), mapSol.size());

	stringstream soln("");

	for (auto e: mapSol) {
		soln << "key: " << e.first << " value: " << e.second << endl;
		EXPECT_TRUE(mapMine.contains(e.first));
		EXPECT_EQ(mapMine.get(e.first), e.second);
	}

	EXPECT_EQ(mapMine.toString(), soln.str());

}
