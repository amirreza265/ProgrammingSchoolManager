#pragma once
#include <string>
using namespace std;
enum class GenderType
{
	male = 'M',
	female = 'F',
	non = 'N'
};
class Person
{
public:
	string Name;
	string Family;
	int Age;
	GenderType Gender;
	Person(string name = "", string family = "", int age = 0, GenderType gender = GenderType::male);
	virtual string ToString(int maxNamelen = maxlenName);
	void SetNameMaxlen(int nml) { maxlenName = nml; }
private:

protected:
	static int maxlenName;
};

int Person::maxlenName = 8;

Person::Person(string name, string family, int age, GenderType gender)
{
	this->Name = name;
	this->Family = family;
	if (age > 99) age = 99;
	this->Age = age;
	this->Gender = gender;
}

inline string Person::ToString(int maxNamelen)
{
	if (Age > 99) Age = 99;

	string age = to_string(Age);
	int space = 2 - age.length();
	string result = "Name : " +  Name + string(maxNamelen - Name.length(), ' ') + " , Family : " + Family + string(maxNamelen - Family.length(), ' ') +
		" , Gender : " + char(Gender) + " , Age :" + to_string(Age)+ string(space,' ');
	return result;
}



