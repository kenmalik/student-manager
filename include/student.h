#ifndef CS003A_STUDENT
#define CS003A_STUDENT

#include <string>

class Student {
private:
	int id;
	std::string name;
	int score;
public:
	// Constructors
	Student();
	Student(int idNum, std::string studName, int courseScore);

	// Setters
	void setId(int idNum);
	void setName(std::string studName);
	void setScore(int courseScore);

	// Getters
	int getId();
	std::string getName();
	int getScore();
};

#endif // !CS003A_STUDENT

