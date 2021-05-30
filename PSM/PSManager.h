#pragma once
#include <vector>
#include <iostream>
#include "Student.h"
#include "Teacher.h"

using namespace std;

enum class PersonType
{
	teacher = 'T',
	student = 'S'
};

class PSManager
{
public:
	vector<Teacher> Teachers;
	vector<Student> Students;

	void AddStudent(Student s);
	void AddTeacher(Teacher t);

	/// <summary>
	/// Find student with name and family
	/// </summary>
	/// <returns>index of student in Students</returns>
	int FirstStudentIndex(string name, string family);
	void EditStudent(int index, string name ="", string family="", int age=-1, GenderType gender = GenderType::non );
	void EditStudent(int index, double mid, double fin, double exer, double proj);
	void RemoveStudent(string name, string family);
	string DisplayStudents();

	/// <summary>
	/// Find teacher with name and family
	/// </summary>
	/// <returns>index of teacher in Teachers</returns>
	int FirstTeacherIndex(string name, string family);
	void EditTeacher(int index, string name = "", string family = "", int age = -1, GenderType gender = GenderType::non);
	void RemoveTeacher(string name, string family);
	string DisplayTeachers();

private:
	int maxNamelenT;
	int maxNamelenS;
};

inline void PSManager::AddStudent(Student s)
{
	if (maxNamelenS < s.Name.length()) maxNamelenS = s.Name.length();
	if (maxNamelenS < s.Family.length()) maxNamelenS = s.Family.length();
	s.SetNameMaxlen(maxNamelenS);
	Students.push_back(s);
}

inline void PSManager::AddTeacher(Teacher t)
{
	if (maxNamelenT < t.Name.length()) maxNamelenT = t.Name.length();
	if (maxNamelenT < t.Family.length()) maxNamelenT = t.Family.length();
	Teachers.push_back(t);
}

inline int PSManager::FirstStudentIndex(string name, string family)
{
	for (int i = 0; i < Students.size(); i++)
	{
		if (Students[i].Name.compare(name) == 0 && Students[i].Family.compare(family) == 0)
			return i;
	}
	return -1;
}

inline void PSManager::EditStudent(int index, string name, string family, int age, GenderType gender)
{
	Students[index].Name   = (name == "")   ? Students[index].Name   : name;
	Students[index].Family = (family == "") ? Students[index].Family : family;
	Students[index].Age    = (age == -1)    ? Students[index].Age    : age;
	Students[index].Gender = (gender == GenderType::non) ? Students[index].Gender : gender;
}

inline void PSManager::EditStudent(int index, double mid, double fin, double exer, double proj)
{
	if (mid  != -1) Students[index].SetMidTerm(mid);
	if (fin  != -1) Students[index].SetFinalTerm(fin);
	if (exer != -1) Students[index].SetExercise(exer);
	if (proj != -1) Students[index].SetProject(proj);
}

inline void PSManager::RemoveStudent(string name, string family)
{
	int index = FirstStudentIndex(name, family);
	Students.erase(Students.begin() + index);
}

inline string PSManager::DisplayStudents()
{
	double avg = 0;
	string str = "";
	str += GetStudentsTable(Students);
	if (Students.size() > 0)
	{
		for (int i = 0; i < Students.size(); i++)
		{
			avg += Students[i].GetMark();
		}
		avg /= Students.size();
		str += "Avrage is :" + to_string(avg, 2) + "\n";
	}
	return str;
}

inline int PSManager::FirstTeacherIndex(string name, string family)
{
	for (int i = 0; i < Teachers.size(); i++)
	{
		if (Teachers[i].Name.compare(name) == 0 && Teachers[i].Family.compare(family) == 0)
			return i;
	}
	return -1;
}

inline void PSManager::EditTeacher(int index, string name, string family, int age, GenderType gender)
{
	Teachers[index].Name = (name == "") ? Teachers[index].Name : name;
	Teachers[index].Family = (family == "") ? Teachers[index].Family : family;
	Teachers[index].Age = (age == -1) ? Teachers[index].Age : age;
	Teachers[index].Gender = (gender == GenderType::non) ? Teachers[index].Gender : gender;
}

inline void PSManager::RemoveTeacher(string name, string family)
{
	int index = FirstTeacherIndex(name, family);
	Teachers.erase(Teachers.begin() + index);
}

inline string PSManager::DisplayTeachers()
{
	long long totalsalalry = 0;
	string str = "";
	str += GetTeachersTable(Teachers);

	if (Teachers.size() > 0)
	{
		for (int i = 0; i < Teachers.size(); i++)
		{
			totalsalalry += Teachers[i].GetSalary();
		}
		str += "Total Salary is : " + to_string(totalsalalry);
	}

	return str;
}