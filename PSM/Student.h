#pragma once
#include "Person.h"
#include "Utility.h"
#include "PSManager.h"
#include "Table.h"
#include <string>
#include <vector>
using namespace std;

class Student : public Person
{
public:
	Student(double midTerm, double finalTerm, double proj, double excer);
	Student(string name = "", string family = "", int age = 0, GenderType gender = GenderType::male) : Person(name, family, age, gender) {
		mid_term = final_term = project = exercise = 0;
	}
	void SetMidTerm(double val);
	friend double GetStudentMidTerm(Student s);

	void SetFinalTerm(double val);
	friend double GetStudentFinalTerm(Student s);

	void SetProject(double val);
	friend double GetStudentProject(Student s);

	void SetExercise(double val);
	friend double GetStudentExercise(Student s);

	double GetMark();
	string ToString(int maxNamelen = maxlenName);
	friend void PrintTopStudents(int count, vector<Student> Students);
	friend void PrintProvisionalStudents(vector<Student> Students);
	friend string GetStudentsTable(vector<Student> Students);

private:
	double mid_term;
	double final_term;
	double project;
	double exercise;

};
inline Student::Student(double midTerm, double finalTerm, double proj, double excer)
{
	SetMidTerm(midTerm);
	SetFinalTerm(finalTerm);
	SetExercise(excer);
	SetProject(proj);
}

inline void Student::SetMidTerm(double val)
{
	if (val >= 5) mid_term = 5;
	else if (val <= 0) mid_term = 0;
	else mid_term = val;
}

//inline double Student::GetMidTerm()
//{
//	return mid_term;
//}

inline void Student::SetFinalTerm(double val)
{
	if (val >= 10) final_term = 10;
	else if (val <= 0) final_term = 0;
	else final_term = val;
}

//inline double Student::GetFinalTerm()
//{
//	return final_term;
//}

inline void Student::SetProject(double val)
{
	if (val >= 3) project = 3;
	else if (val <= 0) project = 0;
	else project = val;
}

//inline double Student::GetProject()
//{
//	return project;
//}

inline void Student::SetExercise(double val)
{
	if (val >= 2) exercise = 2;
	else if (val <= 0) exercise = 0;
	else exercise = val;
}

//inline double Student::GetExercise()
//{
//	return exercise;
//}

inline double Student::GetMark()
{
	double total = (mid_term + final_term + project + exercise);
	if (total >= 20) total = 20.0;
	return total;
}

inline string Student::ToString(int maxNamelen)
{
	string mid = to_string(mid_term, 2);
	string fin = to_string(final_term,2);
	string prj = to_string(project,2);
	string exr = to_string(exercise,2);
	string ttl = to_string(GetMark(), 2);
	string result = Person::ToString(maxNamelen) + " , Middle Term : " + mid + ", Final : " + fin + string(5-fin.length(),' ') + " , Project : " + prj +
					" , Exercise : " + exr + " , Total Score : " + ttl;

	return result;
}

inline double GetStudentMidTerm(Student s)
{
	return s.mid_term;
}

inline double GetStudentFinalTerm(Student s)
{
	return s.final_term;
}

inline double GetStudentProject(Student s)
{
	return s.project;
}

inline double GetStudentExercise(Student s)
{
	return s.exercise;
}

//freind func 
void PrintTopStudents(int count, vector<Student> Students)
{
	vector<Student> topStudents;
	if (count > Students.size()) count = Students.size();
	for (int tsi = 0; tsi < count; tsi++)
	{
		Student topS;
		int topIndex = 0;
		double maxScore = 0;
		for (int i = 0; i < Students.size(); i++)
		{
			if (maxScore <Students[i].GetMark())
			{
				topS = Students[i];
				maxScore = topS.GetMark();
				topIndex = i;
			}
		}
		topStudents.push_back(topS);
		Students.erase(Students.begin() + topIndex);
	}

	double marks = 0;
	for (int i = 0; i < count; i++)
	{
		marks += topStudents[i].GetMark();
	}
	cout << GetStudentsTable(topStudents) << endl;
	cout << "Avrage of " << count << " top students is :" << (marks / count) << endl;
}

void PrintProvisionalStudents(vector<Student> Students)
{
	vector<Student> ps;
	for (int i = 0; i < Students.size(); i++)
	{
		if (Students[i].GetMark() >= 10 && Students[i].GetMark() <= 12)
			ps.push_back(Students[i]);
	}

	cout << GetStudentsTable(ps) << endl;
}

string GetStudentsTable(vector<Student> Students)
{
	vector<string> headers;
	headers.push_back("Name");
	headers.push_back("Family");
	headers.push_back("Age");
	headers.push_back("Gender");
	headers.push_back("MidTerm");
	headers.push_back("Final");
	headers.push_back("Project");
	headers.push_back("Exercise");
	headers.push_back("Total");
	Table sTable(headers);

	for (int i = 0; i < Students.size(); i++)
	{
		vector<string> row;
		row.push_back(Students[i].Name);
		row.push_back(Students[i].Family);
		row.push_back(to_string(Students[i].Age));
		char g = char(Students[i].Gender);
		row.push_back(string(1,g));

		string mid = to_string(Students[i].mid_term, 2);
		string fin = to_string(Students[i].final_term, 2);
		string prj = to_string(Students[i].project, 2);
		string exr = to_string(Students[i].exercise, 2);
		string ttl = to_string(Students[i].GetMark(), 2);

		row.push_back(mid);
		row.push_back(fin);
		row.push_back(prj);
		row.push_back(exr);
		row.push_back(ttl);

		sTable.AddRow(row);
	}

	return sTable.CreateTable();
}