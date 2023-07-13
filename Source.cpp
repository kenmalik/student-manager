// 10597063, Kenji, Longid
// Date: 7.12.23
// Project1, v1

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

class Student {
public:
	int id;
	string name;
	int score;
};

struct Course {
	string name;
	int enrollment;
	Student* studArr;
};

// Data processing functions
void readData(string[], Course[]);
void openIfs(ifstream*, string);
void loadCourseData(ifstream*, Course[]);

// Menu functions
int mainMenu();

void courseLists(Course[], int);
void showAllCourseData(Course);

void threeCourseStudents(Course[], int); // TODO
bool studentIsIn(Course, int);
void showCourseScores(Course[], int, int);

void twoCourseStudents(); // TODO

void topThrees(); // TODO

int main() {
	int fileCount;
	string* files;
	Course* courses;

	// Get filename input
	cout << "Enter the number of courses : ";
	cin >> fileCount;
	if (fileCount != 3) { // Error text if user inputs number other than 3
		cout << "Can only handle 3 files";
		exit(1);
	}
	courses = new Course[fileCount];

	files = new string[fileCount];                      // Create dynamic array to store filenames
	for (int i = 0; i < fileCount; i++) {               // Store user filename input into array
		cout << "Enter filename " << i + 1 << " : ";
		cin >> files[i];
	}

	readData(files, courses); // Read file data and put into data structure
	delete[] files;           // Delete filename array to save space in heap

	while (1) { // Menu loop
		switch (mainMenu()) {
		case 1:
			courseLists(courses, fileCount);
			break;
		case 2:
			threeCourseStudents(courses, fileCount);
			break;
		case 3:
			twoCourseStudents();
			break;
		case 4:
			topThrees();
			break;
		case 5:
			exit(0);
		default:
			cout << "Not an option";
		}
	}

	return 0;
}

void readData(string fileArr[], Course courseArr[]) {
	ifstream ifs1, ifs2, ifs3;

	// Open files
	openIfs(&ifs1, fileArr[0]);
	openIfs(&ifs2, fileArr[1]);
	openIfs(&ifs3, fileArr[2]);

	// Put file data into data structure
	loadCourseData(&ifs1, courseArr);
	loadCourseData(&ifs2, courseArr);
	loadCourseData(&ifs3, courseArr);
}

void openIfs(ifstream* ifs, string fileName) {
	(*ifs).open(fileName);
	if ((*ifs).fail()) {
		cout << "Failed to open " << fileName;
		exit(1);
	}
}

void loadCourseData(ifstream* ifs, Course courseArr[]) {
	string courseNameTemp;
	Course* activeCourse;

	// Determine which course struct to load to
	(*ifs) >> courseNameTemp;
	if (courseNameTemp == "C++")
		activeCourse = &courseArr[0];
	else if (courseNameTemp == "Java")
		activeCourse = &courseArr[1];
	else
		activeCourse = &courseArr[2];

	// Begin filling data structure
	(*activeCourse).name = courseNameTemp;
	(*ifs) >> (*activeCourse).enrollment;

	(*activeCourse).studArr = new Student[(*activeCourse).enrollment]; // Create dynamic array of students

	for (int i = 0; i < (*activeCourse).enrollment; i++) // Fill student dynamic arrays with data
		(*ifs) >> (*activeCourse).studArr[i].id 
		       >> (*activeCourse).studArr[i].name 
		       >> (*activeCourse).studArr[i].score;

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

void courseLists(Course courseArr[], int arrLen) {
	for (int i = 0; i < arrLen; i++)
		showAllCourseData(courseArr[i]);
}

void showAllCourseData(Course course) {
	cout << "\nCourse : " << course.name << endl
		 << "---------------------------------" << endl;

	for (int i = 0; i < course.enrollment; i++) {
		cout << setw(10) << course.studArr[i].id
			 << setw(15) << course.studArr[i].name
			 << setw(5) << course.studArr[i].score << endl;
	}

	cout << "\n=================================" << endl;
}

void threeCourseStudents(Course courseArr[], int arrLen) {
	// Selection sort course array by ascending enrollment count
	int i, j, minIdx;
	for (i = 0; i < arrLen - 1; i++) {
		minIdx = i;
		for (j = i + 1; j < arrLen; j++) {
			if (courseArr[j].enrollment < courseArr[minIdx].enrollment)
				minIdx = j;
		}
		if (minIdx != i)
			swap(courseArr[minIdx], courseArr[i]);
	}

	// Count amount of students that are in every course
	int count = 0;
	for (int i = 0; i < courseArr[0].enrollment; i++) // Check whether the students in the smallest course appear the other courses
		if (studentIsIn(courseArr[1], courseArr[0].studArr[i].id) && studentIsIn(courseArr[2], courseArr[0].studArr[i].id))
			count++;
	cout << "  There are " << count << " students who take all " << arrLen << " courses" << endl
		 << "-----------------------------------------------" << endl;

	// Display data of students in every course
	for (int i = 0; i < courseArr[0].enrollment; i++) // Check whether the students in the smallest course appear the other courses
		if (studentIsIn(courseArr[1], courseArr[0].studArr[i].id) && studentIsIn(courseArr[2], courseArr[0].studArr[i].id)) {
			cout << setw(7) << courseArr[0].studArr[i].id
				<< setw(10) << courseArr[0].studArr[i].name;
			showCourseScores(courseArr, arrLen, courseArr[0].studArr[i].id);
			cout << endl;
		}
}

bool studentIsIn(Course course, int searchId) {
	for (int i = 0; i < course.enrollment; i++) // Check if there is an id match in given course
		if (course.studArr[i].id == searchId)
			return 1;
	return 0;
}

void showCourseScores(Course courseArr[], int arrLen, int studId) {
	int i, j;
	for (i = 0; i < arrLen; i++) { // Loop through every course
		for (j = 0; j < courseArr[i].enrollment; j++) { // Check if id match in the course roster
			if (courseArr[i].studArr[j].id == studId) // Display score in course if id matches
				cout << "  " << courseArr[i].name << "(" << courseArr[i].studArr[j].score << ")";
		}
	}
}

void twoCourseStudents() {
	cout << "Two Course Menu" << endl;
}
void topThrees() {
	cout << "Top Three Menu" << endl;
}