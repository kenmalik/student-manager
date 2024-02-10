// 10597063, Kenji, Longid
// Date: 7.12.23
// Project1, v1

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "student.h"

struct Course {
	std::string name = "";
	int enrollment = 0;
	Student* studArr = nullptr;
};


// Data processing functions
void readData(std::string[], Course[], const size_t&);
// Precondition:  Array of filename strings, array to be filled with course data, and size of
//                these arrays inputted. Calls openIfs() and loadCourseData().
// Postcondition: Data structure of courses filled using files specified.

void openIfs(std::ifstream*, std::string);
// Precondition:  Pointer to dynamic array of ifstreams created in readData() and name of file to
//                be read is inputted.
// Postcondition: Ifstream for specified file created.

void loadCourseData(std::ifstream*, Course&);
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
	std::string* files;
	Course* courses;

	// Get filename input
	std::cout << "Enter the number of courses : ";
	std::cin >> fileCount;
	courses = new Course[fileCount];

	files = new std::string[fileCount];                      // Create dynamic array to store filenames
	for (int i = 0; i < fileCount; i++) {               // Store user filename input into array
		std::cout << "Enter filename " << i + 1 << " : ";
		std::cin >> files[i];
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
			std::cout << "Not an option";
		}
	}

	return 0;
}

void readData(std::string fileArr[], Course courseArr[], const size_t& arrLen) {
	std::ifstream* ifsArr = new std::ifstream[arrLen];

	for (int i = 0; i < arrLen; i++) {
		openIfs(&ifsArr[i], fileArr[i]); // Open file from user input
		loadCourseData(&ifsArr[i], courseArr[i]); // Fill data structure with data from file
	}
}

void openIfs(std::ifstream* ifs, std::string fileName) {
	(*ifs).open(fileName);
	if ((*ifs).fail()) {
		std::cout << "Failed to open " << fileName;
		exit(1);
	}
}

void loadCourseData(std::ifstream* ifs, Course& course) {
	// General course data
	(*ifs) >> course.name;
	(*ifs) >> course.enrollment;

	// Student data
	course.studArr = new Student[course.enrollment]; // Create dynamic array of students

	int idTemp, scoreTemp;
	std::string nameTemp;
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
	std::cout << "\n\n=====================  Menu  =====================" << std::endl
	     << "  1. Show all course lists" << std::endl
	     << "  2. List of students who take all courses" << std::endl
	     << "  3. List of students who take two courses" << std::endl
	     << "  4. Print out top three scores for each course" << std::endl
	     << "  5. Exit" << std::endl
		 << "---> select : ";
	std::cin >> selection;
	return selection;
}

void courseLists(Course courseArr[], const size_t& arrLen) {
	for (int i = 0; i < arrLen; i++) {
		std::cout << "\nCourse : " << courseArr[i].name << std::endl
			<< "---------------------------------" << std::endl;

		for (int j = 0; j < courseArr[i].enrollment; j++) {
			std::cout << std::setw(10) << courseArr[i].studArr[j].getId()
				<< std::setw(15) << courseArr[i].studArr[j].getName()
				<< std::setw(5) << courseArr[i].studArr[j].getScore() << std::endl;
		}

		std::cout << "\n=================================" << std::endl;
	}
}

void allCourseStudents(Course courseArr[], const size_t& arrLen) {
	size_t matchListSize = courseArr[0].enrollment / 4;
	Student* studentsInAllClasses = new Student[matchListSize];

	int studIdx, courseIdx, matchListIdx = 0;
	for (studIdx = 0; studIdx < courseArr[0].enrollment; studIdx++) { // Iterate through each student in the first course
		bool inAll = 1;
		for (courseIdx = 1; courseIdx < arrLen; courseIdx++) { // Iterate through other courses. If student does not appear in one, they are not added to array
			if (!studentIsIn(courseArr[courseIdx], courseArr[0].studArr[studIdx].getId())) {
				inAll = 0;
				break;
			}
		}

		if (inAll) { // Add student to array if in all classes
			studentsInAllClasses[matchListIdx] = courseArr[0].studArr[studIdx];
			if (++matchListIdx == matchListSize) // Increase size of array at full capacity
				studentsInAllClasses = growArray(studentsInAllClasses, matchListSize);
		}
	}

	std::cout << "\n  There are " << matchListIdx << " students who take all " << arrLen << " courses" << std::endl
		 << "-----------------------------------------------" << std::endl;

	// Display data of students that are in every course
	for (int i = 0; i < matchListIdx; i++) {
		std::cout << std::setw(7) << studentsInAllClasses[i].getId()
			<< std::setw(10) << studentsInAllClasses[i].getName();
		showCourseScores(courseArr, arrLen, studentsInAllClasses[i].getId());
		std::cout << std::endl;
	}

	delete[] studentsInAllClasses;
}


Student* growArray(Student arr[], size_t& size) {
	size *= 2;
	Student* tempArr = new Student[size]; // Temporary array of double size

	for (int i = 0; i < size / 2; i++) // Copy data from original array into temp array
		tempArr[i] = arr[i];
	
	return tempArr;
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
				std::cout << "  " << courseArr[i].name << "(" << courseArr[i].studArr[j].getScore() << ")";
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
			bool inOtherCourse = 0;
			for (int j = 0; j < arrLen; j++) { // Iterate through other courses to check if student is appears anywhere else
				if (j != courseIdx1 && j != courseIdx2 && studentIsIn(courseArr[j], courseArr[courseIdx1].studArr[i].getId())) {
					inOtherCourse = 1;
					break;
				}
			}
			if (!inOtherCourse) {
				studentsInTwo[matchListIdx] = courseArr[courseIdx1].studArr[i];
				if (++matchListIdx == matchListSize)
					growArray(studentsInTwo, matchListSize);
			}
		}
	}

	// Output list of students in the two specified courses exclusively
	std::cout << "\n  There are " << matchListSize << " students that take " << courseArr[courseIdx1].name << " and " << courseArr[courseIdx2].name << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
	for (int i = 0; i < matchListIdx; i++) {
		std::cout << std::setw(7) << studentsInTwo[i].getId()
			 << std::setw(10) << studentsInTwo[i].getName();
		showCourseScores(courseArr, arrLen, studentsInTwo[i].getId());
		std::cout << std::endl;
	}

	delete[] studentsInTwo;
}

void topThrees(Course courseArr[], const size_t& arrLen) {
	for (int courseIdx = 0; courseIdx < arrLen; courseIdx++) {
		std::cout << "\n[ " << courseArr[courseIdx].name << " Top Three Scores ]" << std::endl;

		//Selection sort students descending
		int i, j, maxIdx;
		for (i = 0; i < courseArr[courseIdx].enrollment - 1; i++) {
			maxIdx = i;
			for (j = i + 1; j < courseArr[courseIdx].enrollment; j++)
				if (courseArr[courseIdx].studArr[j].getScore() > courseArr[courseIdx].studArr[maxIdx].getScore())
					maxIdx = j;
			if (maxIdx != i)
				std::swap(courseArr[courseIdx].studArr[maxIdx], courseArr[courseIdx].studArr[i]);
		}

		// Output top students
		for (int studIdx = 0, place = 0; studIdx < courseArr[courseIdx].enrollment; studIdx++) { // Iterate through list of students sorted by descending scores
			if (courseArr[courseIdx].studArr[studIdx].getScore() != courseArr[courseIdx].studArr[studIdx - 1].getScore() || studIdx == 0) { // Print header for each group of students with the same score
				if (++place == 4) break;
				std::cout << place << ". " << courseArr[courseIdx].studArr[studIdx].getScore() << std::endl;
			}

			std::cout << "   " << courseArr[courseIdx].studArr[studIdx].getId() << std::setw(10) << courseArr[courseIdx].studArr[studIdx].getName() << std::endl; // Print student data
		}
	}
}
