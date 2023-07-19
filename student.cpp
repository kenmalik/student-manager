#include "student.h"
#include <string>
using namespace std;

// Constructors
Student::Student() : id(0), name(""), score(0) {}

Student::Student(int idNum, string studName, int courseScore) : id(idNum), name(studName), score(courseScore) {}

// Setters
void Student::setId(int idNum) {
	id = idNum;
}

void Student::setName(string studName) {
	name = studName;
}

void Student::setScore(int courseScore) {
	score = courseScore;
}

// Getters
int Student::getId() {
	return id;
}

string Student::getName() {
	return name;
}

int Student::getScore() {
	return score;
}