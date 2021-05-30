#pragma once
#include<string>
#include"Person.h"
#include"PSManager.h"

using namespace std;

class Teacher : public Person
{
public:
	Teacher();
	Teacher(int hour);
	Teacher(string name = "", string family = "", int age = 0, GenderType gender = GenderType::male) : Person(name, family, age, gender) { oTime = 0; }
	static void ChangeBase(long salary);
	void AddTime(int hour);
	long long GetSalary();
	string ToString(int maxNamelen = maxlenName);
	friend int GetTeacherTime(Teacher t);
	friend int GetTeacherBaseSalary();
	friend string GetTeachersTable(vector<Teacher> Teachers);

private:
	int oTime;
	static long baseSalary;
};

long Teacher::baseSalary = 10000;

Teacher::Teacher()
{
	oTime = 0;
}

inline Teacher::Teacher(int hour)
{
	oTime = hour;
}

inline void Teacher::ChangeBase(long salary)
{
	baseSalary = salary;
}

inline void Teacher::AddTime(int hour)
{
	oTime += hour;
}

inline long long Teacher::GetSalary()
{
	long salary = (oTime>= 20) ? (20 * baseSalary + (oTime-20)*2*baseSalary) : baseSalary * oTime;
	return salary;
}

inline string Teacher::ToString(int maxNamelen)
{
	string tim = to_string(oTime);
	int space = (3 - tim.length() <0)?0:3-tim.length();
	string result = "Name : " + Name + string(maxNamelen - Name.length(), ' ') + " , Family : " + Family + string(maxNamelen - Family.length(), ' ') +
		" , Age :" + to_string(Age) + " , Time : " + tim + string(space, ' ')  + " , Base Salary : " + to_string(baseSalary) +
		" , Total Salary : " + to_string(GetSalary());
	return result;
}

int GetTeacherTime(Teacher t)
{
	return t.oTime;
}

inline int GetTeacherBaseSalary()
{
	return Teacher::baseSalary;
}


string GetTeachersTable(vector<Teacher> Teachers)
{
	vector<string> headers;
	headers.push_back("Name");
	headers.push_back("Family");
	headers.push_back("Age");
	headers.push_back("Gender");
	headers.push_back("Time");
	headers.push_back("Base Salary");
	headers.push_back("Total Salary");
	Table sTable(headers);

	for (int i = 0; i < Teachers.size(); i++)
	{
		vector<string> row;
		row.push_back(Teachers[i].Name);
		row.push_back(Teachers[i].Family);
		row.push_back(to_string(Teachers[i].Age));
		char g = char(Teachers[i].Gender);
		row.push_back(string(1, g));

		string tim = to_string(Teachers[i].oTime);
		string base = to_string(Teachers[i].baseSalary);
		string ttlsl = to_string(Teachers[i].GetSalary());

		row.push_back(tim);
		row.push_back(base);
		row.push_back(ttlsl);

		sTable.AddRow(row);
	}

	return sTable.CreateTable();
}