#include "student.h"
#include <string>

// Constructors
Student::Student() : id(0), name(""), score(0) {}

Student::Student(int idNum, std::string studName, int courseScore) : id(idNum), name(studName), score(courseScore) {}

// Setters
void Student::setId(int idNum) {
	id = idNum;
}

void Student::setName(std::string studName) {
	name = studName;
}

void Student::setScore(int courseScore) {
	score = courseScore;
}

// Getters
int Student::getId() {
	return id;
}

std::string Student::getName() {
	return name;
}

int Student::getScore() {
	return score;
}
