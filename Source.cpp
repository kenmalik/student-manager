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
void readData(string**, Course*, Course*, Course*);
void openIfs(ifstream*, string);
void loadCourseData(ifstream*, Course*, Course*, Course*);

// Menu functions
int mainMenu();

void courseLists(Course&, Course&, Course&);
void showAllCourseData(Course);

void threeCourseStudents(Course, Course, Course); // TODO
bool studentIsIn(Course, int);

void twoCourseStudents(); // TODO

void topThrees(); // TODO

int main() {
	int fileCount;
	string* files;
	Course cpp, java, python;

	// Get filename input
	cout << "Enter the number of courses : ";
	cin >> fileCount;
	if (fileCount != 3) { // Error text if user inputs number other than 3
		cout << "Can only handle 3 files";
		exit(1);
	}

	files = new string[fileCount];                      // Create dynamic array to store filenames
	for (int i = 0; i < fileCount; i++) {               // Store user filename input into array
		cout << "Enter filename " << i + 1 << " : ";
		cin >> files[i];
	}

	// TODO: Handle files inputted in different order
	readData(&files, &cpp, &java, &python); // Read file data and put into data structure
	delete[] files;                      // Delete filename array to save space in heap

	while (1) { // Menu loop
		switch (mainMenu()) {
		case 1:
			courseLists(cpp, java, python);
			break;
		case 2:
			threeCourseStudents();
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

void readData(string** fileArrPtr, Course* cppCourse, Course* javaCourse, Course* pythonCourse) {
	ifstream ifs1, ifs2, ifs3;

	// Open files
	openIfs(&ifs1, (*fileArrPtr)[0]);
	openIfs(&ifs2, (*fileArrPtr)[1]);
	openIfs(&ifs3, (*fileArrPtr)[2]);

	// Put file data into data structure
	loadCourseData(&ifs1, cppCourse, javaCourse, pythonCourse);
	loadCourseData(&ifs2, cppCourse, javaCourse, pythonCourse);
	loadCourseData(&ifs3, cppCourse, javaCourse, pythonCourse);
}

void openIfs(ifstream* ifs, string fileName) {
	(*ifs).open(fileName);
	if ((*ifs).fail()) {
		cout << "Failed to open " << fileName;
		exit(1);
	}
}

void loadCourseData(ifstream* ifs, Course* cppCourse, Course* javaCourse, Course* pythonCourse) {
	string courseNameTemp;
	Course* activeCourse;

	// Determine which course struct to load to
	(*ifs) >> courseNameTemp;
	if (courseNameTemp == "C++")
		activeCourse = cppCourse;
	else if (courseNameTemp == "Java")
		activeCourse = javaCourse;
	else
		activeCourse = pythonCourse;

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

void courseLists(Course& course1, Course& course2, Course& course3) {
	showAllCourseData(course1);
	showAllCourseData(course2);
	showAllCourseData(course3);
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

void threeCourseStudents(Course cppCourse, Course javaCourse, Course pythonCourse) {


	cout << "All Course Menu" << endl;
}

bool studentIsIn(Course course, int id) {

}

void twoCourseStudents() {
	cout << "Two Course Menu" << endl;
}
void topThrees() {
	cout << "Top Three Menu" << endl;
}