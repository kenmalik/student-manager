// 10597063, Kenji, Longid
// Date: 7.12.23
// Project1, v1

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "student.h"
using namespace std;

struct Course {
	string name;
	int enrollment;
	Student* studArr;
};

// Data processing functions
void readData(string[], Course[], const size_t&);
// Precondition:  Array of filename strings, array to be filled with course data, and size of
//                these arrays inputted. Calls openIfs() and loadCourseData().
// Postcondition: Data structure of courses filled using files specified.

void openIfs(ifstream*, string);
// Precondition:  Pointer to dynamic array of ifstreams created in readData() and name of file to
//                be read is inputted.
// Postcondition: Ifstream for specified file created.

void loadCourseData(ifstream*, Course&);
// Precondition:  Ifstream for data to be loaded into data structure and memory address for Course
//                struct to be loaded into inputted.
// Postcondition: Data loaded into Course structs. Dynamic arrays associated with these structs
//                created to store Student objects.

// Menu functions
int mainMenu();
// Precondition:  Called by control loop switch statement in main().
// Postcondition: Takes input from user and returns selected menu index.

void courseLists(Course[], const size_t&);
// Precondition:  Array of Course structs and size of the array inputted.
// Postcondition: Displays course's names and information of all students in said course.

void allCourseStudents(Course[], const size_t&);
// Precondition:  Array of Course structs and size of the array inputted.
// Postcondition: Displays name and course score data for students who take every class.

Student* growArray(Student[], size_t&);
// Precondition:  Dynamic array to increase size of and the current size of said array inputted.
// Postcondition: Creates new dynamic array of double the size, loads original array's data into it, and
//                returns pointer to new dynamic array.

bool studentIsIn(Course, int);
// Precondition:  Course struct to search and id number for student to search for inputted.
// Postcondition: Returns bool indicating whether or not student id is found in course roster.

void showCourseScores(Course[], const size_t&, int);
// Precondition:  Array of courses, size of the array, and id number of student search for inputted.
// Postcondition: Displays names of courses taken and score student recieved in the course.

void twoCourseStudents(Course[], const size_t&);
// Precondition:  Array of courses and size of the array inputted.
// Postcondition: Calls showExclusiveMatches() for every combination of two courses.

void showExclusiveMatches(Course[], const size_t&, int, int);
// Precondition:  Array of courses, size of the array, and indices of courses to cross-reference inputted.
// Postcondition: Displays name of students that are in two classes exclusively, the names of the
//                courses they take, and the scores they earned in each class. For every combination
//                of two courses, the number of students who take those courses exclusively is shown.

void topThrees(Course[], const size_t&);
// Precondition:  Array of courses and size of the array inputted.
// Postcondition: Selection sorts students by descending score for each class. Outputs id and name for each
//                student who earned the top three scores in the class. Students are grouped by score.

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

	readData(files, courses, fileCount);   // Read file data and put into data structure
	delete[] files;                        // Delete filename array to save space in heap

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

void readData(string fileArr[], Course courseArr[], const size_t& arrLen) {
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

void courseLists(Course courseArr[], const size_t& arrLen) {
	for (int i = 0; i < arrLen; i++) {
		cout << "\nCourse : " << courseArr[i].name << endl
			<< "---------------------------------" << endl;

		for (int j = 0; j < courseArr[i].enrollment; j++) {
			cout << setw(10) << courseArr[i].studArr[j].getId()
				<< setw(15) << courseArr[i].studArr[j].getName()
				<< setw(5) << courseArr[i].studArr[j].getScore() << endl;
		}

		cout << "\n=================================" << endl;
	}
}

void allCourseStudents(Course courseArr[], const size_t& arrLen) {
	size_t matchListSize = courseArr[0].enrollment / 4;
	Student* studentsInAllClasses = new Student[matchListSize];

	int studIdx, courseIdx, matchListIdx = 0;
	for (studIdx = 0; studIdx < courseArr[0].enrollment; studIdx++) { // Iterate through each student in the first course
		bool inAll = 1;
		for (courseIdx = 1; courseIdx < arrLen; courseIdx++) { // Iterate through other courses. If student does not appear in one, they are not added to vector
			if (!studentIsIn(courseArr[courseIdx], courseArr[0].studArr[studIdx].getId())) {
				inAll = 0;
				break;
			}
		}

		if (inAll) {
			studentsInAllClasses[matchListIdx] = courseArr[0].studArr[studIdx];
			if (++matchListIdx == matchListSize)
				studentsInAllClasses = growArray(studentsInAllClasses, matchListSize);
		}
	}

	cout << "\n  There are " << matchListIdx << " students who take all " << arrLen << " courses" << endl
		 << "-----------------------------------------------" << endl;

	// Display data of students that are in every course
	for (int i = 0; i < matchListIdx; i++) { // Check whether the students in the first course appear the other courses
		cout << setw(7) << studentsInAllClasses[i].getId()
			<< setw(10) << studentsInAllClasses[i].getName();
		showCourseScores(courseArr, arrLen, studentsInAllClasses[i].getId());
		cout << endl;
	}
}


Student* growArray(Student arr[], size_t& size) {
	size *= 2;
	Student* tempArr = new Student[size];

	for (int i = 0; i < size / 2; i++)
		tempArr[i] = arr[i];
	delete[] arr;

	arr = new Student[size];
	for (int i = 0; i < size; i++)
		arr[i] = tempArr[i];
	delete[] tempArr;

	return arr;
}

bool studentIsIn(Course course, int searchId) {
	for (int i = 0; i < course.enrollment; i++) // Check if there is an id match in given course
		if (course.studArr[i].getId() == searchId)
			return 1;
	return 0;
}

void showCourseScores(Course courseArr[], const size_t& arrLen, int studId) {
	int i, j;
	for (i = 0; i < arrLen; i++) // Loop through every course
		for (j = 0; j < courseArr[i].enrollment; j++) // Check if id match in the course roster
			if (courseArr[i].studArr[j].getId() == studId) // Display score in course if id matches
				cout << "  " << courseArr[i].name << "(" << courseArr[i].studArr[j].getScore() << ")";
}

void twoCourseStudents(Course courseArr[], const size_t& arrLen) {
	int courseIdx1, courseIdx2;
	for (courseIdx1 = 0; courseIdx1 < arrLen; courseIdx1++) // For loops iterate through each combination of classes
		for (courseIdx2 = courseIdx1 + 1; courseIdx2 < arrLen; courseIdx2++)
			showExclusiveMatches(courseArr, arrLen, courseIdx1, courseIdx2);
}

void showExclusiveMatches(Course courseArr[], const size_t& arrLen, int courseIdx1, int courseIdx2) {
	size_t matchListSize = courseArr[courseIdx1].enrollment / 4;
	Student* studentsInTwo = new Student[matchListSize];
	int matchListIdx = 0;
	
	for (int i = 0; i < courseArr[courseIdx1].enrollment; i++) { // Iterate courseArr[courseIdx1] roster
		if (studentIsIn(courseArr[courseIdx2], courseArr[courseIdx1].studArr[i].getId())) {
			bool otherMatch = 0;
			for (int j = 0; j < arrLen; j++) { // Iterate through other courses to check if student is appears anywhere else
				if (j != courseIdx1 && j != courseIdx2 && studentIsIn(courseArr[j], courseArr[courseIdx1].studArr[i].getId())) {
					otherMatch = 1;
					break;
				}
			}
			if (otherMatch == 0) {
				studentsInTwo[matchListIdx] = courseArr[courseIdx1].studArr[i];
				if (++matchListIdx == matchListSize)
					growArray(studentsInTwo, matchListSize);
			}
		}
	}

	// Output list of students in the two specified courses exclusively
	cout << "\n  There are " << matchListSize << " students that take " << courseArr[courseIdx1].name << " and " << courseArr[courseIdx2].name << endl;
	cout << "------------------------------------------------" << endl;
	for (int i = 0; i < matchListIdx; i++) {
		cout << setw(7) << studentsInTwo[i].getId()
			 << setw(10) << studentsInTwo[i].getName();
		showCourseScores(courseArr, arrLen, studentsInTwo[i].getId());
		cout << endl;
	}
}

void topThrees(Course courseArr[], const size_t& arrLen) {
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