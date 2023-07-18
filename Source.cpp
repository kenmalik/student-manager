// 10597063, Kenji, Longid
// Date: 7.12.23
// Project1, v1

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include "student.h"
using namespace std;

struct Course {
	string name;
	int enrollment;
	Student* studArr;
};

// Data processing functions
void readData(string[], Course[], const int&);
void openIfs(ifstream*, string);
void loadCourseData(ifstream*, Course&);

// Menu functions
int mainMenu();

void courseLists(Course[], const int&);
void showAllCourseData(Course);

void allCourseStudents(Course[], const int&);
bool studentIsIn(Course, int);
void showCourseScores(Course[], const int&, int);

void twoCourseStudents(Course[], const int&);
void showExclusiveMatches(Course[], const int&, int, int);

void topThrees(Course[], const int&); // TODO

int main() {
	int fileCount;
	string* files;
	Course* courses;

	// Get filename input
	cout << "Enter the number of courses : ";
	cin >> fileCount;
	courses = new Course[fileCount];

	files = new string[fileCount];                      // Create dynamic array to store filenames
	for (int i = 0; i < fileCount; i++) {               // Store user filename input into array
		cout << "Enter filename " << i + 1 << " : ";
		cin >> files[i];
	}

	readData(files, courses, fileCount); // Read file data and put into data structure
	delete[] files;           // Delete filename array to save space in heap

	while (1) { // Menu loop
		switch (mainMenu()) {
		case 1:
			courseLists(courses, fileCount);
			break;
		case 2:
			allCourseStudents(courses, fileCount);
			break;
		case 3:
			twoCourseStudents(courses, fileCount);
			break;
		case 4:
			topThrees(courses, fileCount);
			break;
		case 5:
			exit(0);
		default:
			cout << "Not an option";
		}
	}

	return 0;
}

void readData(string fileArr[], Course courseArr[], const int& arrLen) {
	ifstream* ifsArr = new ifstream[arrLen];

	for (int i = 0; i < arrLen; i++) {
		openIfs(&ifsArr[i], fileArr[i]); // Open file from user input
		loadCourseData(&ifsArr[i], courseArr[i]); // Fill data structure with data from file
	}
}

void openIfs(ifstream* ifs, string fileName) {
	(*ifs).open(fileName);
	if ((*ifs).fail()) {
		cout << "Failed to open " << fileName;
		exit(1);
	}
}

void loadCourseData(ifstream* ifs, Course& course) {
	// General course data
	(*ifs) >> course.name;
	(*ifs) >> course.enrollment;

	// Student data
	course.studArr = new Student[course.enrollment]; // Create dynamic array of students

	int idTemp, scoreTemp;
	string nameTemp;
	for (int i = 0; i < course.enrollment; i++) { // Fill student dynamic arrays with data
		(*ifs) >> idTemp >> nameTemp >> scoreTemp;
		course.studArr[i].setId(idTemp);
		course.studArr[i].setName(nameTemp);
		course.studArr[i].setScore(scoreTemp);
	}

	(*ifs).close();
}

int mainMenu() {
	int selection;
	cout << "\n\n=====================  Menu  =====================" << endl
	     << "  1. Show all course lists" << endl
	     << "  2. List of students who take all courses" << endl
	     << "  3. List of students who take two courses" << endl
	     << "  4. Print out top three scores for each course" << endl
	     << "  5. Exit" << endl
		 << "---> select : ";
	cin >> selection;
	return selection;
}

void courseLists(Course courseArr[], const int& arrLen) {
	for (int i = 0; i < arrLen; i++)
		showAllCourseData(courseArr[i]);
}

void showAllCourseData(Course course) {
	cout << "\nCourse : " << course.name << endl
		 << "---------------------------------" << endl;

	for (int i = 0; i < course.enrollment; i++) {
		cout << setw(10) << course.studArr[i].getId()
			 << setw(15) << course.studArr[i].getName()
			 << setw(5) << course.studArr[i].getScore() << endl;
	}

	cout << "\n=================================" << endl;
}

void allCourseStudents(Course courseArr[], const int& arrLen) {
	vector<Student> studentsInAllClasses;

	int studIdx, courseIdx;
	for (studIdx = 0; studIdx < courseArr[0].enrollment; studIdx++) { // Iterate through each student in the first course
		bool inAll = 1;
		for (courseIdx = 1; courseIdx < arrLen; courseIdx++) { // Iterate through other courses. If student does not appear in one, they are not added to vector
			if (!studentIsIn(courseArr[courseIdx], courseArr[0].studArr[studIdx].getId())) {
				inAll = 0;
				cout << courseArr[0].studArr[studIdx].getName() << " is not in " << courseArr[courseIdx].name << endl;
				break;
			}
		}

		if (inAll)
			studentsInAllClasses.push_back(courseArr[0].studArr[studIdx]);
	}

	cout << "\n  There are " << studentsInAllClasses.size() << " students who take all " << arrLen << " courses" << endl
		 << "-----------------------------------------------" << endl;

	// Display data of students that are in every course
	for (int i = 0; i < studentsInAllClasses.size(); i++) { // Check whether the students in the first course appear the other courses
		cout << setw(7) << studentsInAllClasses[i].getId()
			<< setw(10) << studentsInAllClasses[i].getName();
		showCourseScores(courseArr, arrLen, studentsInAllClasses[i].getId());
		cout << endl;
	}
}

bool studentIsIn(Course course, int searchId) {
	for (int i = 0; i < course.enrollment; i++) // Check if there is an id match in given course
		if (course.studArr[i].getId() == searchId)
			return 1;
	return 0;
}

void showCourseScores(Course courseArr[], const int& arrLen, int studId) {
	int i, j;
	for (i = 0; i < arrLen; i++) // Loop through every course
		for (j = 0; j < courseArr[i].enrollment; j++) // Check if id match in the course roster
			if (courseArr[i].studArr[j].getId() == studId) // Display score in course if id matches
				cout << "  " << courseArr[i].name << "(" << courseArr[i].studArr[j].getScore() << ")";
}

void twoCourseStudents(Course courseArr[], const int& arrLen) {
	int courseIdx1, courseIdx2;
	for (courseIdx1 = 0; courseIdx1 < arrLen; courseIdx1++) // For loops iterate through each combination of classes
		for (courseIdx2 = courseIdx1 + 1; courseIdx2 < arrLen; courseIdx2++)
			showExclusiveMatches(courseArr, arrLen, courseIdx1, courseIdx2);
}

void showExclusiveMatches(Course courseArr[], const int& arrLen, int courseIdx1, int courseIdx2) {
	vector<Student> students;
	
	for (int i = 0; i < courseArr[courseIdx1].enrollment; i++) { // Iterate courseArr[courseIdx1] roster
		if (studentIsIn(courseArr[courseIdx2], courseArr[courseIdx1].studArr[i].getId())) {
			bool otherMatch = 0;
			for (int j = 0; j < arrLen; j++) { // Iterate through other courses to check if student is appears anywhere else
				if (j != courseIdx1 && j != courseIdx2 && studentIsIn(courseArr[j], courseArr[courseIdx1].studArr[i].getId())) {
					otherMatch = 1;
					break;
				}
			}
			if (otherMatch == 0)
				students.push_back(courseArr[courseIdx1].studArr[i]);
		}
	}

	// Output vector of students in the two specified courses exclusively
	cout << "\n  There are " << students.size() << " students that take " << courseArr[courseIdx1].name << " and " << courseArr[courseIdx2].name << endl;
	cout << "------------------------------------------------" << endl;
	for (int i = 0; i < students.size(); i++) {
		cout << setw(7) << students[i].getId()
			 << setw(10) << students[i].getName();
		showCourseScores(courseArr, arrLen, students[i].getId());
		cout << endl;
	}
}

void topThrees(Course courseArr[], const int& arrLen) {
	for (int courseIdx = 0; courseIdx < arrLen; courseIdx++) {
		cout << "\n[ " << courseArr[courseIdx].name << " Top Three Scores ]" << endl;

		//Selection sort students descending
		int i, j, maxIdx;
		for (i = 0; i < courseArr[courseIdx].enrollment - 1; i++) {
			maxIdx = i;
			for (j = i + 1; j < courseArr[courseIdx].enrollment; j++)
				if (courseArr[courseIdx].studArr[j].getScore() > courseArr[courseIdx].studArr[maxIdx].getScore())
					maxIdx = j;
			if (maxIdx != i)
				swap(courseArr[courseIdx].studArr[maxIdx], courseArr[courseIdx].studArr[i]);
		}

		// Output top students
		for (int studIdx = 0, place = 0; studIdx < courseArr[courseIdx].enrollment; studIdx++) { // Iterate through list of students sorted by descending scores
			if (courseArr[courseIdx].studArr[studIdx].getScore() != courseArr[courseIdx].studArr[studIdx - 1].getScore() || studIdx == 0) { // Print header for each group of students with the same score
				if (++place == 4) break;
				cout << place << ". " << courseArr[courseIdx].studArr[studIdx].getScore() << endl;
			}

			cout << "   " << courseArr[courseIdx].studArr[studIdx].getId() << setw(10) << courseArr[courseIdx].studArr[studIdx].getName() << endl; // Print student data
		}
	}
}