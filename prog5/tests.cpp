#include "mymap.h"
#include <gtest/gtest.h>
#include <map>
#include <random>
#include "myrandom.h"

#define LOW_NUM 0
#define MID_NUM 15

#define HIGH_NUM 100
#define HIGHER_NUM 2000

template<typename T1, typename T2>
void mapCheck(map<T1, T2>& myap, mymap<T1, T2>& mymyap) {

	ASSERT_EQ(mymyap.Size(), myap.size());

	for (int i = 0; i < myap.size(); i++) {

		ASSERT_EQ(mymyap.contains(i), true);
		ASSERT_EQ(myap[i], mymyap.get(i));
	}
}

void pop(map<char, char>& theMap, mymap<char, char>& mymyap, const int n) {
	srand(time(0));

	for (int i = 0; i < n; i++) {

		char key = randomInteger('A', 'Z');
		char value = randomInteger('a', 'z');

		mymyap.put(key, value);

		theMap[key] = value;
	}
}

void pop(map<int, int>& theMap, mymap<int, int>& mymyap, const int n) {
	srand(time(0));

	for (int i = 0; i < n; i++) {

		mymyap.put(i, i);

		if (i > n) {
			cout << "we done a fucky" << endl;
		}
		theMap[i] = i;

	}
}

template<typename T1, typename T2>
void boundaryChecks(map<T1, T2>& myap, mymap<T1, T2>& mymyap) {

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

template<typename T1, typename T2>
void treeOrderCheck(mymap<T1, T2>& mymyap) {
	int i = 0;

	T1 prevKey;

	for (auto key: mymyap) {
		if (i > 0) {
			ASSERT_TRUE(key >= prevKey);
		}

		prevKey = key;
		i++;
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
	mymap<int, int> mymyap;
	map<int, int> myap;

	ASSERT_EQ(mymyap.Size(), 0);

	mapCheck(myap, mymyap);
	boundaryChecks(myap, mymyap);

}

TEST(squaryBrackets, squareyo) {

	mymap<int, int> mymyap;

	int arr[MID_NUM];

	for (int i = 0; i < MID_NUM; i++) {
		arr[i] = randomInteger(1, 5000);

		mymyap.put(arr[i], arr[i]);
	}

	for (int i = 0; i < MID_NUM; i++) {
		ASSERT_EQ(arr[i], mymyap[arr[i]]);
	}

	ASSERT_TRUE(mymyap.Size() >= 1 && mymyap.Size() <= MID_NUM);

	int sizeBefore = mymyap.Size();
	int someValue = mymyap[5000 + 1];

	ASSERT_EQ(someValue, int());
	sizeBefore++;
	ASSERT_EQ(sizeBefore, mymyap.Size());

}

TEST(putOverWrite, chunk1) {
	mymap<char, char> mymyap;

	for (int i = 0; i < 10; i++) {
		int size = min(i + 1, 10);
		char value = i % 10;
		char key = i + 1;

		mymyap.put(key, value);

		ASSERT_TRUE(mymyap.contains(key));
		ASSERT_EQ(mymyap.get(key), value);
		ASSERT_EQ(mymyap.Size(), size);
	}
}

TEST(forEachOrder, defaultConstruct) {

	mymap<int, int> mapMine;
	map<int, int> soln;

	int n = HIGH_NUM;

	int arr[] = {2, 1, 3};

	int order[] = {1, 2, 3};

	for (int i = 1; i <= n; i++) {
		int key = randomInteger(0, 54000);
		int val = randomInteger(0, 54000);

		mapMine.put(key, val);
		soln[key] = val;

	}

	stringstream s("");
	stringstream mine("");

	for (auto pair: soln) {
		s << pair.first;
	}

	for (auto key: mapMine) {
		mine << key;
	}

	ASSERT_EQ(s.str(), mine.str());

}

TEST(putAndGet, defaultConstruct) {
	map<int, int> myap;
	mymap<int, int> mymyap;

	pop(myap, mymyap, 10);

	mapCheck(myap, mymyap);
	boundaryChecks(myap, mymyap);
}

TEST(get, defaultConstruct) {
	map<int, int> myap;
	mymap<int, int> mymyap;

	mapCheck(myap, mymyap);
	boundaryChecks(myap, mymyap);
}

TEST(putUpdate, defaultConstruct) {
	map<int, int> myap;
	mymap<int, int> mymyap;

	pop(myap, mymyap, 10);
	mapCheck(myap, mymyap);
	boundaryChecks(myap, mymyap);

	pop(myap, mymyap, 10);
	mapCheck(myap, mymyap);
	boundaryChecks(myap, mymyap);

}

TEST(sizeAndContains, defaultConstruct) {
	map<int, int> myap;
	mymap<int, int> mymyap;

	pop(myap, mymyap, 60);

	mapCheck(myap, mymyap);
	boundaryChecks(myap, mymyap);

}

TEST(toString, defaultConst) {
	mymap<int, int> mymyap;

	mymyap.put(8, 80);
	mymyap.put(15, 150);
	mymyap.put(20, 200);

	ASSERT_EQ("key: 8 value: 80\nkey: 15 value: 150\nkey: 20 value: 200\n",
			  mymyap.toString());

	ASSERT_EQ(3, mymyap.Size());

}

TEST(squareBrackets, defaultConstruct) {
	map<int, int> myap;
	mymap<int, int> mymyap;

	pop(myap, mymyap, 5000);

	mapCheck(myap, mymyap);
	boundaryChecks(myap, mymyap);

	srand(time(0));

	int n = 100;
	for (int i = 0; i < n; i++) {

		int g = rand();
		ASSERT_EQ(myap.contains(g), mymyap.contains(g));

		if (!myap.contains(g)) {
			ASSERT_EQ(int(), mymyap.get(g));
			ASSERT_FALSE(mymyap.contains(g));

			ASSERT_EQ(int(), mymyap[g]);
			myap[g] = int();
			ASSERT_TRUE(mymyap.contains(g));
		}

	}
}

TEST(iterator, defaultConstruct) {
	map<int, int> myap;
	mymap<int, int> mymyap;

	pop(myap, mymyap, 52000);

	mapCheck(myap, mymyap);
	boundaryChecks(myap, mymyap);

	//checkOrder(myap,mymyap);

}

TEST(forEach, defaultConstruct) {

	mymap<int, int> map;

	int arr[] = {2, 1, 3};

	int order[] = {1, 2, 3};

	for (int i = 0; i < 3; i++) {

		map.put(arr[i], arr[i]);

	}

	int i = 0;

	for (auto key: map) {

		EXPECT_EQ(order[i++], key);

	}

	ASSERT_EQ(map.Size(), 3);
}

TEST(copyOperator, ms3) {
	mymap<int, int> m1;
	mymap<int, int> m2;

	map<int, int> temp;

	pop(temp, m1, 15);

	ASSERT_EQ(m2.Size(), 0);
	ASSERT_EQ(m1.Size(), 15);

	m2 = m1;

	ASSERT_EQ(m2.Size(), 15);
	ASSERT_EQ(m1.Size(), 15);

}

TEST(copyConstruct, ms4) {
	mymap<char, char> m1;
	map<char, char> temp;

	pop(temp, m1, MID_NUM);

	ASSERT_EQ(m1.Size(), temp.size());

	mymap<char, char> m2(m1);
	m2.put('a', 'A');

	ASSERT_EQ(m2.Size(), temp.size() + 1);
	ASSERT_EQ(m1.Size(), temp.size());

}

TEST(copyConstruct2, ms4) {
	mymap<char, char> m1;
	map<char, char> temp;

	// m1 is not initialized as a test case

	ASSERT_EQ(m1.Size(), temp.size());

	mymap<char, char> m2(m1);
	m2.put('a', 'A');

	ASSERT_EQ(m2.Size(), temp.size() + 1);
	ASSERT_EQ(m1.Size(), temp.size());

}

TEST(vecFunc, ms4) {
	// Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
	// {{8, 80}, {15, 150}, {20, 200}}

	mymap<int, string> mymyap;

	mymyap.put(8, "CS211");
	mymyap.put(15, "Sucks");
	mymyap.put(20, "Ass");
	mymyap.put(21, "In");
	mymyap.put(22, "Troy");
	mymyap.put(23, "WE");
	mymyap.put(24, "Trust");


	vector<pair<int, string> > vec = {
			{8,  "CS211"},
			{15, "Sucks"},
			{20, "Ass"},
			{21, "In"},
			{22, "Troy"},
			{23, "WE"},
			{24, "Trust"}
	};

	ASSERT_EQ(7, mymyap.Size());
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
	mymap<int, int> mymyap;
	map<int, int> myap;

	for (int i = 0; i < 200; i++) {
		mymyap.put(i, randomInteger(1, 100));
	}

	ASSERT_EQ(mymyap.Size(), 200);

	mymyap.clear();

	ASSERT_EQ(mymyap.Size(), 0);

}

TEST(orderCheck, chonkyChungusTest) {
	srand(time(0));

	mymap<int, double> mymyap;

	int size = randomInteger(4, 900);

	for (int i = 0; i < size; i++) {
		mymyap.put(
				randomInteger(69, 10000),
				randomInteger(420, 100000)
		);
	}

	treeOrderCheck(mymyap);

	ASSERT_TRUE(mymyap.Size() <= size);

	mymyap.clear();

	ASSERT_EQ(mymyap.Size(), 0);

}

// TO DO: write lots of tests here.

// vv Example Tests from mimir vv

TEST (mimirSamp, stress) {
	mymap<int, int> mapMine;
	map<int, int> mapSol;

	int n = 12345;

	for (int i = 1; i <= n; i++) {
		int key = randomInteger(0, 6009000);
		int val = randomInteger(0, 6009000);

		mapMine.put(key, val);
		mapSol[key] = val;
	}

	EXPECT_EQ(mapMine.Size(), mapSol.size());

	stringstream soln("");

	for (pair<int,int> e: mapSol) {
		soln << "key: " << e.first << " value: " << e.second << endl;
		EXPECT_TRUE(mapMine.contains(e.first));
		EXPECT_EQ(mapMine.get(e.first), e.second);
	}

	EXPECT_EQ(mapMine.toString(), soln.str());

}
