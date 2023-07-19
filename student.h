#ifndef CS003A_STUDENT
#define CS003A_STUDENT

#include <string>
using namespace std;

class Student {
private:
	int id;
	string name;
	int score;
public:
	// Constructors
	Student();
	Student(int idNum, string studName, int courseScore);

	// Setters
	void setId(int idNum);
	void setName(string studName);
	void setScore(int courseScore);

	// Getters
	int getId();
	string getName();
	int getScore();
};

#endif // !CS003A_STUDENT

