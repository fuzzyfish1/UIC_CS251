/* driver.h
 * library for Question and person structs
 * Allows for implementation of survey like software
 * */

#include <math.h>

#include <cassert>
#include <limits>
#include <map>
#include <set>
#include <string>

#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
	string questionText;     // Text of the question
	map<char, int> factors;  // Map from factors to +1 or -1
	friend bool operator<(const Question &lhs, const Question &rhs) {
		return lhs.questionText < rhs.questionText;
	}

	friend bool operator==(const Question &lhs, const Question &rhs) {
		return lhs.questionText == rhs.questionText;
	}

	friend bool operator!=(const Question &lhs, const Question &rhs) {
		return lhs.questionText != rhs.questionText;
	}
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
	string name;            // Name of the person
	map<char, int> scores;  // Map from factors to +1 or -1
	friend bool operator<(const Person &lhs, const Person &rhs) {
		return lhs.name < rhs.name;
	}

	friend bool operator==(const Person &lhs, const Person &rhs) {
		return lhs.name == rhs.name;
	}

	friend bool operator!=(const Person &lhs, const Person &rhs) {
		return lhs.name != rhs.name;
	}
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
 */
Question randomElement(set <Question> &questions) {
	int ind = randomInteger(0, (int) questions.size() - 1);
	int i = 0;

	for (auto e: questions) {
		if (i == ind) {
			return e;
		}
		i++;
	}
	return {};
}

// This will pull a random question from a questionBank
Question randomQuestionFrom(set <Question> &questions) {
	if (questions.empty()) {
		throw 20;
	}

	Question myQuestion = randomElement(questions);
	questions.erase(myQuestion);

	return myQuestion;
}

// find OCEANScores from a map of questions to responses
map<char, int> scoresFrom(map<Question, int> &answers) {
	map<char, int> OCEANScores;

	for (auto &x: answers) {
		map<char, int> factors = x.first.factors;
		int weight = x.second - 3;

		for (auto &i: factors) {
			OCEANScores[i.first] += i.second * weight;
		}
	}

	return OCEANScores;
}

// takes OCEANScores, changes its mag as if they all answered same # questions
map<char, double> normalize(const map<char, int> &scores) {
	map<char, double> normalizedScores;

	double normalFactor = 0;
	for (auto &pair: scores) {
		normalFactor += (pair.second) * (pair.second);
	}

	if (normalFactor == 0) {
		throw 20;
	}

	normalFactor = sqrt(normalFactor);

	for (auto &pair: scores) {
		normalizedScores.insert(std::pair<char, double>(
				pair.first, (double) pair.second / normalFactor));
	}

	return normalizedScores;
}

// returns a number from -1->1 that represents how similar 2 scores are
double cosineSimilarityOf(const map<char, double> &lhs,
						  const map<char, double> &rhs) {
	double dotProd = 0;

	for (auto &pair: lhs) {
		if (rhs.count(pair.first) != 0) {
			dotProd += pair.second * rhs.at(pair.first);
		}
	}

	return dotProd;
}

// finds the person most similar to a score from a set of people
Person mostSimilarTo(map<char, int> &scores, set <Person> &people) {
	if (scores.empty() || people.empty()) {
		throw 20;
	}

	map<char, double> normalizedScores = normalize(scores);

	double matchSimilarity;
	Person match;

	int index = 0;

	for (auto &person: people) {
		double similarity =
				cosineSimilarityOf(normalizedScores, normalize(person.scores));

		if ((similarity > matchSimilarity) || index == 0) {
			matchSimilarity = similarity;
			match = person;
			index++;
		}
	}

	return match;
}
