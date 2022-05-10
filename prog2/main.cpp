/* ---------------------------------------------
Program 2: Personality Tests

Course: CS 251, Spring 2022. Thurs 12pm Lab
System: Windows 10 CLion, WSLS Ubuntu nano
Author: Zain Ali
Description:
---------------------------------------------
*/
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "driver.h"

using namespace std;

// puts a string and data into person struct
void setData(Person& element, string& textData, map<char, int>& data) {
  element.name = textData;
  element.scores = data;
};

// puts a string and data into Question struct
void setData(Question& element, string& textData, map<char, int>& data) {
  element.questionText = textData;
  element.factors = data;
};

// reads a file into a set of People or Questions
template <typename T>
void readFileInSet(string filePath, set<T>& setToGoInto) {
  // preparing the file
  ifstream infile(filePath);

  string value;

  if (!infile.is_open()) {
    cout << "Error: unable to open "
         << "'" << filePath << "'" << endl;
  }

  setToGoInto.clear();

  // reading 1 line every loop
  getline(infile, value);

  while (!infile.eof() && infile.is_open()) {
    // process the name or question text string
    string::size_type pos = value.find('.');

    string textVal = value.substr(0, pos);

    value.erase(0, pos + 2);

    map<char, int> data;

    // process the map data chunk of the line
    while (true) {
      pos = value.find(' ');
      string::size_type endPos = pos == string::npos ? value.size() : pos;

      char letter = value.at(0);

      int number = stoi(value.substr(value.find(':') + 1, endPos));

      data.emplace(std::pair<char, int>(letter, number));

      value.erase(0, endPos + 1);

      if (pos == string::npos) {
        break;
      }
    }

    // add the data to the set
    T element;

    setData(element, textVal, data);

    setToGoInto.emplace(element);

    getline(infile, value);
  }
}

// ask User a number of questions and get a response
void surveyUser(set<Question>& questions, map<Question, int>& responses,
                int& numQuestions) {
  for (int i = 0; i < numQuestions; i++) {
    Question qSurvey = randomQuestionFrom(questions);

    cout << "How much do you agree with this statement?\n\""
         << qSurvey.questionText << ".\"\n\n"
         << "1. Strongly disagree\n"
            "2. Disagree\n"
            "3. Neutral\n"
            "4. Agree\n"
            "5. Strongly agree\n\n"
            "Enter your answer here (1-5): "
         << endl;

    int answer;
    cin >> answer;

    responses.emplace(qSurvey, answer);
  }
}

// processes survey data
int processSurveyData(int answer, set<Person>& people,
                      map<char, int>& OCEANScores) {
  string filePath = "../";

  // find the filepath from the users answer
  switch (answer) {
    case 1:
      filePath += "BabyAnimals";
      break;

    case 2:
      filePath += "Brooklyn99";
      break;

    case 3:
      filePath += "Disney";
      break;

    case 4:
      filePath += "Hogwarts";
      break;

    case 5:
      filePath += "MyersBriggs";
      break;

    case 6:
      filePath += "SesameStreet";
      break;

    case 7:
      filePath += "StarWars";
      break;

    case 8:
      filePath += "Vegetables";
      break;

    case 9:
      filePath += "mine";
      break;

    case 0:
      cout << "Goodbye!" << endl;
      // this stops the program
      return 0;

    default:
      cout << "unknown answer" << endl;
      break;
  }

  // process and display data
  readFileInSet(filePath + ".people", people);

  Person similarDude = mostSimilarTo(OCEANScores, people);

  cout << "You got " << similarDude.name << "!\n" << endl;

  return 1;
}

int main() {
  set<Question> questionBank;
  set<Person> people;
  map<Question, int> responses;

  readFileInSet("../questions.txt", questionBank);

  cout << "Welcome to the Personality Quiz!\n\n"
          "Choose number of questions: "
       << endl;

  int numQuestions;
  cin >> numQuestions;

  surveyUser(questionBank, responses, numQuestions);

  bool running = true;

  map<char, int> OCEANScores = scoresFrom(responses);

  int answer = 1;

  // find match in this file
  while (running) {
    cout << "1. BabyAnimals\n"
            "2. Brooklyn99\n"
            "3. Disney\n"
            "4. Hogwarts\n"
            "5. MyersBriggs\n"
            "6. SesameStreet\n"
            "7. StarWars\n"
            "8. Vegetables\n"
            "9. mine\n"
            "0. To end program.\n\n"
            "Choose test number (1-9, or 0 to end): ";

    cin >> answer;

    running = processSurveyData(answer, people, OCEANScores);
  }

  return 0;
}
