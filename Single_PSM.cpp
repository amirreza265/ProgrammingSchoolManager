#include <iostream>
#include <string>
#include <vector>
#include<stdio.h>
#include<Windows.h>
#include<fstream>
#include <string>
using namespace std;

class Table;
class FileManager;
class Command;
class CommandProvider;
enum class GenderType;
class Person;
class Student;
class Teacher;
class PSManager;
class PSCommandProvider;

//Utility.h
namespace std
{
	/// <summary>
	/// Convert Double to string 
	/// with arbitrary number of decimals
	/// </summary>
	/// <param name="val">: Value</param>
	/// <param name="decimalCount">: number of decimals</param>
	static string to_string(double val, int decimalCount)
	{
		string sub = to_string(val);
		return sub.substr(0, sub.find('.') + decimalCount + 1);
	}

	static string substrBetween(string c, const char first, const char last, int& startIndex, int& endIndex)
	{
		startIndex = c.find_first_of(first, endIndex);
		endIndex = c.find_first_of(last, startIndex);
		if (endIndex - startIndex - 1 <= 0) return "";
		string substr = c.substr(startIndex + 1, endIndex - startIndex - 1);
		return substr;
	}

	static vector<string> substrsBetween(string c, const char first, const char last, int startIndex, int count)
	{
		int endIndex = startIndex;
		vector<string> strs;

		for (int i = 0; i < count; i++)
		{
			strs.push_back(substrBetween(c, first, last, startIndex, endIndex));
		}

		return strs;
	}
}

//ConsoleManager.h
enum class ConsoleColors
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    AQUA = 3,
    RED = 4,
    PURPLE = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_AQUA = 11,
    LIGHT_RED = 12,
    LIGHT_PURPLE = 13,
    LIGHT_YELLOW = 14,
    LIGHT_WHITE = 15
};

void setConsoleTextColor(const ConsoleColors foreground)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int k = int(foreground);
    SetConsoleTextAttribute(hConsole, k);
}

//FileManager.h
class FileManager
{
protected:
	ofstream* oFile;
	ifstream* iFile;
public:
	FileManager(string fileName);
	~FileManager();

	string ReadLine();
	string ReadAll();
	void WriteLine(string str);
	void Write(string str);

	void CloseAll();
};

FileManager::FileManager(string fileName)
{
	oFile = new ofstream(fileName, ios::app);
	iFile = new ifstream(fileName);
}

FileManager::~FileManager()
{
	oFile->close();
	iFile->close();
	delete oFile, iFile;
}

inline string FileManager::ReadLine()
{
	string line;
	if (std::getline(*iFile, line))
		return line;
	else
		return "";
}

inline string FileManager::ReadAll()
{
	string text;
	string line;
	while (std::getline(*iFile, line))
	{
		text += line + '\n';
		line = "";
	}
	return text;
}

inline void FileManager::WriteLine(string str)
{
	(*oFile) << str << endl;
}

inline void FileManager::Write(string str)
{
	(*oFile) << str;
}

inline void FileManager::CloseAll()
{
	oFile->close();
	iFile->close();
}


//Table.h
class Table
{
public:
	Table(vector<string> headers);
	void AddRow(vector<string> row);
	string CreateTable();
	void SetTableBorder(const char wall = '|', const char ceiling = '-', const char ceilinginter = 'o', const char intersection = 'o', const char rowCeiling = '.');
private:
	string GetCeiling(const char ceiling = '-', char intersection = 'o');
	vector<string> _headers;
	int _columnCount;
	vector<vector<string>> _records;
	vector<int> _maxColLen;
	char _wall = '|';
	char _ceiling = '-';
	char _rowCeiling = '.';
	char _ceilinginter = 'o';
	char _intersection = ':';
	string _table;
};

Table::Table(vector<string> headers)
{
	_columnCount = headers.size();
	_headers = headers;
	for (int i = 0; i < _headers.size(); i++)
	{
		_maxColLen.push_back(_headers[i].length());
	}
}

inline void Table::AddRow(vector<string> row)
{
	_records.push_back(row);

	//resize max len for each colomn
	int len = (row.size() > _columnCount) ? _columnCount : row.size();
	for (int i = 0; i < len; i++)
	{
		if (row[i].length() > _maxColLen[i])
			_maxColLen[i] = row[i].length();
	}
}

inline string Table::CreateTable()
{
	string ceil = "";
	ceil += GetCeiling(_ceiling, _ceilinginter);
	for (int i = 0; i < _columnCount; i++)
	{
		ceil += _headers[i] + string(_maxColLen[i] - _headers[i].length(), ' ');
		ceil += (i < _columnCount - 1) ? _wall : '\n';
	}
	ceil += GetCeiling(_ceiling, _ceilinginter);
	_table = ceil;

	for (int index = 0; index < _records.size(); index++)
	{
		vector<string> rowrec = _records[index];
		string row = "";
		for (int j = 0; j < rowrec.size(); j++)
		{
			row += rowrec[j] + string(_maxColLen[j] - rowrec[j].length(), ' ');
			row += (j < _columnCount - 1) ? _wall : '\n';
		}
		row += GetCeiling(_rowCeiling, _intersection);
		_table += row;
	}
	return _table;
}

inline void Table::SetTableBorder(const char wall, const char ceiling, const char ceilinginter, const char intersection, const char rowCeiling)
{
	_wall = wall;
	_ceiling = ceiling;
	_ceilinginter = ceilinginter;
	_intersection = intersection;
	_rowCeiling = rowCeiling;
}

inline string Table::GetCeiling(const char ceiling, char intersection)
{
	string ceil = "";
	for (int i = 0; i < _columnCount; i++)
	{
		ceil += string(_maxColLen[i], ceiling);
		ceil += (i < _columnCount - 1) ? intersection : '\n';
	}
	return ceil;
}

//Command Provider
class Command
{
public:
	Command() { Name = "-1", func = [](string a) -> void {}; }
	Command(string name, void (*commandFunc)(string command)) { Name = name; func = commandFunc; }

	string Name;
	void (*func)(string command);
};

class CommandProvider
{
public:
	CommandProvider();
	void AddCommand(Command comand);
	int FindCommandIndex(string name);
	void RemoveCommand(int index);
	void RemoveCommand(string name);
	void RunCommand(int index, string commandArgumants);
	void RunCommand(string command);
	void RunCommand(string commandName, string commandArguments);
	void Clear();
	void RunProgramm();
	static void ShowErrore(string errorMessage);
	static void ShowInfo(string infoMessage);

	//friend void CommandProvider_RunCommand(CommandProvider cp ,string commandName, string command);
	//friend void CommandProvider_RunCommand(CommandProvider cp, string command);

private:
	vector<Command> commands;
};

CommandProvider::CommandProvider()
{
	commands.push_back(Command("Help", [](string a) -> void {
		cout << "This is a help of the Commands" << endl;
		}));
}

inline void CommandProvider::AddCommand(Command comand)
{
	if (FindCommandIndex(comand.Name) == -1)
	{
		commands.push_back(comand);
		CommandProvider::ShowInfo("command was added : " + comand.Name);
	}
}

inline int CommandProvider::FindCommandIndex(string name)
{
	for (int i = 0; i < commands.size(); i++)
	{
		if (commands[i].Name.compare(name) == 0)
			return i;
	}
	return -1;
}

inline void CommandProvider::RemoveCommand(int index)
{
	if (index < commands.size())
		commands.erase(commands.begin() + index);
}

inline void CommandProvider::RemoveCommand(string name)
{
	int index = FindCommandIndex(name);

	if (index != -1)
		RemoveCommand(index);
}

inline void CommandProvider::RunCommand(int index, string command)
{
	setConsoleTextColor(ConsoleColors::WHITE);
	command = command.substr(command.find(':') + 1);
	commands[index].func(command);
}

inline void CommandProvider::Clear()
{
	commands.clear();
}

inline void CommandProvider::RunProgramm()
{
	while (true)
	{
		setConsoleTextColor(ConsoleColors::GRAY);
		cout << "\nEnter your Command: \nTo see the program guide Write <<help:>>" << endl;
		string command;
		setConsoleTextColor(ConsoleColors::LIGHT_GREEN);
		getline(cin, command);
		string commandName = command.substr(0, command.find(':'));
		int commandIndex = FindCommandIndex(commandName);

		if (commandIndex == -1)
			if (command.compare("close") == 0)
			{
				break;
			}
			else
			{
				setConsoleTextColor(ConsoleColors::LIGHT_RED);
				cout << "Command Not Found ...." << endl;
				continue;
			}

		RunCommand(commandIndex, command.substr(command.find(':') + 1));
	}
}

inline void CommandProvider::ShowErrore(string errorMessage)
{
	setConsoleTextColor(ConsoleColors::LIGHT_RED);
	cout << errorMessage << endl;
}

inline void CommandProvider::ShowInfo(string infoMessage)
{
	setConsoleTextColor(ConsoleColors::LIGHT_AQUA);
	cout << infoMessage << endl;
}

void CommandProvider::RunCommand(string commandName, string commandArguments)
{
	int index = FindCommandIndex(commandName);

	if (index != -1)
		RunCommand(index, commandArguments);
	else
		CommandProvider::ShowErrore("Command not found : " + commandName);
}

void CommandProvider::RunCommand(string command)
{
	string commandName = command.substr(0, command.find(':'));
	string commandArguments = command.substr(command.find(':') + 1);

	int index = FindCommandIndex(commandName);

	if (index != -1)
		RunCommand(index, commandArguments);
	else
		CommandProvider::ShowErrore("Command not found : " + commandName);
}

//Person.h
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
	string result = "Name : " + Name + string(maxNamelen - Name.length(), ' ') + " , Family : " + Family + string(maxNamelen - Family.length(), ' ') +
		" , Gender : " + char(Gender) + " , Age :" + to_string(Age) + string(space, ' ');
	return result;
}


//Student.h
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

	string ToString(int maxNamelen = maxlenName);
	friend void PrintTopStudents(int count, vector<Student> Students);
	friend void PrintProvisionalStudents(vector<Student> Students);
	friend string GetStudentsTable(vector<Student> Students);

private:
	double mid_term;
	double final_term;
	double project;
	double exercise;
	double GetMark();

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

inline void Student::SetFinalTerm(double val)
{
	if (val >= 10) final_term = 10;
	else if (val <= 0) final_term = 0;
	else final_term = val;
}

inline void Student::SetProject(double val)
{
	if (val >= 3) project = 3;
	else if (val <= 0) project = 0;
	else project = val;
}

inline void Student::SetExercise(double val)
{
	if (val >= 2) exercise = 2;
	else if (val <= 0) exercise = 0;
	else exercise = val;
}

inline double Student::GetMark()
{
	double total = (mid_term + final_term + project + exercise);
	if (total >= 20) total = 20.0;
	return total;
}

inline string Student::ToString(int maxNamelen)
{
	string mid = to_string(mid_term, 2);
	string fin = to_string(final_term, 2);
	string prj = to_string(project, 2);
	string exr = to_string(exercise, 2);
	string ttl = to_string(GetMark(), 2);
	string result = Person::ToString(maxNamelen) + " , Middle Term : " + mid + ", Final : " + fin + string(5 - fin.length(), ' ') + " , Project : " + prj +
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

//freind functions s
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
			if (maxScore < Students[i].GetMark())
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
		row.push_back(string(1, g));

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
	string str = sTable.CreateTable();
	double avg = 0;
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


//Teacher.h
class Teacher : public Person
{
public:
	Teacher();
	Teacher(int hour);
	Teacher(string name = "", string family = "", int age = 0, GenderType gender = GenderType::male) : Person(name, family, age, gender) { oTime = 0; }
	static void ChangeBase(long salary);
	void AddTime(int hour);
	string ToString(int maxNamelen = maxlenName);
	friend int GetTeacherTime(Teacher t);
	friend int GetTeacherBaseSalary();
	friend string GetTeachersTable(vector<Teacher> Teachers);

private:
	int oTime;
	static long baseSalary;
	long long GetSalary();
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
	long salary = (oTime >= 20) ? (20 * baseSalary + (oTime - 20) * 2 * baseSalary) : baseSalary * oTime;
	return salary;
}

inline string Teacher::ToString(int maxNamelen)
{
	string tim = to_string(oTime);
	int space = (3 - tim.length() < 0) ? 0 : 3 - tim.length();
	string result = "Name : " + Name + string(maxNamelen - Name.length(), ' ') + " , Family : " + Family + string(maxNamelen - Family.length(), ' ') +
		" , Age :" + to_string(Age) + " , Time : " + tim + string(space, ' ') + " , Base Salary : " + to_string(baseSalary) +
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

		string tim = to_string(Teachers[i].oTime);
		string base = to_string(Teachers[i].baseSalary);
		string ttlsl = to_string(Teachers[i].GetSalary());

		row.push_back(tim);
		row.push_back(base);
		row.push_back(ttlsl);

		sTable.AddRow(row);
	}

	string table = sTable.CreateTable();
	long long totalsalalry = 0;
	if (Teachers.size() > 0)
	{
		for (int i = 0; i < Teachers.size(); i++)
		{
			totalsalalry += Teachers[i].GetSalary();
		}
		table += "Total Salary is : " + to_string(totalsalalry);
	}

	return table;
}


//PSManager.h
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
	void EditStudent(int index, string name = "", string family = "", int age = -1, GenderType gender = GenderType::non);
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
	Students[index].Name = (name == "") ? Students[index].Name : name;
	Students[index].Family = (family == "") ? Students[index].Family : family;
	Students[index].Age = (age == -1) ? Students[index].Age : age;
	Students[index].Gender = (gender == GenderType::non) ? Students[index].Gender : gender;
}

inline void PSManager::EditStudent(int index, double mid, double fin, double exer, double proj)
{
	if (mid != -1) Students[index].SetMidTerm(mid);
	if (fin != -1) Students[index].SetFinalTerm(fin);
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
	string str = "";
	str += GetStudentsTable(Students);
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
	string str = "";
	str += GetTeachersTable(Teachers);
	return str;
}


//PSCommandProvider.h
class PSCommandProvider : public CommandProvider, public PSManager
{
public:
	PSCommandProvider(bool addCommand = true);
	static PSCommandProvider psInstance;
private:

};

PSCommandProvider PSCommandProvider::psInstance = PSCommandProvider(false);

PSCommandProvider::PSCommandProvider(bool addCommand)
{
	Clear();
	if (!addCommand) return;

	AddCommand(Command("help", [](string command) -> void {
		cout << "Commands :" << endl;
		cout << "close <= close the program" << endl;
		cout << "add-teacher: (name) (family) (age) (gender M/F)" << endl;
		cout << "add-student: (name) (family) (age) (gender M/F)" << endl;
		cout << "add-teacher-time:(name)(family)(time:hour)" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << "show-teachers: " << endl;
		cout << "show-students: " << endl;
		cout << "show-students-prov: <= Show Provisional Students" << endl;
		cout << "show-students-top: (count=1)" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << "edit-student-score:(name)(family)(midTerm)(final)(exercise)(projects) \n (-1) for each element means dont change score" << endl;
		cout << "edit-teacher-base-salary:(salary)" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << "seed-students: (count=1) <= add some students to PSM for test" << endl;
		cout << "seed-teachers: (count=1) <= add some teachers to PSM for test" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << "backup-all:(file path)      <= write commands in file for getting backup from teachers and students" << endl;
		cout << "backup-teachers:(file path) <= write commands in file for getting backup from teachers" << endl;
		cout << "backup-students:(file path) <= write commands in file for getting backup from students" << endl;
		cout << "execute-file:(file path)    <= Read the commands inside the file to execute them on PSManager" << endl;
		}));

	AddCommand(Command("add-teacher", [](string c) -> void {
		//get variables
		int startIndex = 0;
		int endIndex = 0;
		vector<string> strs = substrsBetween(c, '(', ')', startIndex, 4);
		string name = strs[0];

		string family = strs[1];

		string a = strs[2];
		if (a == "") a = "0";
		int age = 0;
		try
		{
			age = stoi(a);
		}
		catch (const std::exception&)
		{
			cout << "age is not correct.." << endl;
			return;
		}

		char g = strs[3][0];

		if (g != 'M' && g != 'F') {
			cout << "Error: Gender type should be (M) for male or (F) for female" << endl;
			return;
		}
		int gender = int(g);

		Teacher t(name, family, age, (GenderType)gender);
		PSCommandProvider::psInstance.AddTeacher(t);
		}));

	AddCommand(Command("show-teachers", [](string c) -> void {
		string str = PSCommandProvider::psInstance.DisplayTeachers();
		cout << "\n" << str << endl;
		}));

	AddCommand(Command("add-student", [](string c) -> void {
		//get variables
		int startIndex = 0;
		int endIndex = 0;
		vector<string> strs = substrsBetween(c, '(', ')', startIndex, 4);
		string name = strs[0];
		string family = strs[1];

		string a = strs[2];
		int age = 0;
		if (a == "") a = "0";
		try
		{
			age = stoi(a);
		}
		catch (const std::exception&)
		{
			PSCommandProvider::ShowErrore("age is not correct..");
			return;
		}


		char g = strs[3][0];
		if (g == 'm') g = 'M';
		if (g == 'f') g = 'F';
		if (g != 'M' && g != 'F') {
			PSCommandProvider::ShowErrore("Gender type should be (M) for male or (F) for female");
			return;
		}
		int gender = int(g);

		Student t(name, family, age, (GenderType)gender);
		PSCommandProvider::psInstance.AddStudent(t);
		}));

	AddCommand(Command("show-students", [](string c) -> void {
		string str = PSCommandProvider::psInstance.DisplayStudents();
		cout << "\n" << str << endl;
		}));

	AddCommand(Command("show-students-top", [](string c) -> void {
		int st = 0;
		int en = 0;
		string a = substrBetween(c, '(', ')', st, en);
		if (a == "") a = "1";
		int count = 0;
		try
		{
			count = stoi(a);
		}
		catch (const std::exception&)
		{
			PSCommandProvider::ShowErrore("Enter number for count...");
			return;
		}

		try
		{
			PrintTopStudents(count, PSCommandProvider::psInstance.Students);
		}
		catch (const std::exception&)
		{
			PSCommandProvider::ShowErrore("students count is :" + to_string(PSCommandProvider::psInstance.Students.size()));
			return;
		}

		}));

	AddCommand(Command("edit-student-score", [](string c) -> void {
		int st = 0;
		vector<string> strs = substrsBetween(c, '(', ')', st, 6);

		string name = strs[0];
		string family = strs[1];
		double scores[4];
		for (int i = 2; i < 6; i++)
		{
			string s = strs[i];
			if (strs[i] == "") strs[i] = "-1";
			try
			{
				scores[i - 2] = stod(s);
			}
			catch (const std::exception&)
			{
				PSCommandProvider::ShowErrore("incorrect inputs for double parameter");
			}
		}

		int index = PSCommandProvider::psInstance.FirstStudentIndex(name, family);
		if (index == -1) { PSCommandProvider::ShowErrore("Studet not found ..."); return; }

		PSCommandProvider::psInstance.EditStudent(index, scores[0], scores[1], scores[2], scores[3]);
		}));

	AddCommand(Command("seed-students", [](string c) -> void {
		int st = 0;
		int en = 0;
		string a = substrBetween(c, '(', ')', st, en);
		if (a == "") a = "1";
		int count = 0;
		try
		{
			count = stoi(a);
		}
		catch (const std::exception&)
		{
			PSCommandProvider::ShowErrore("Enter number for count...");
			return;
		}

		for (int i = 0; i < count; i++)
		{
			string name = "amirreza s" + to_string(i + 1);
			string family = "hosseini" + to_string(i + 1);
			int age = i + 20;
			GenderType gender = GenderType::male;
			Student t(name, family, age, gender);
			t.SetMidTerm(rand() % 6);
			t.SetFinalTerm(rand() % 11);
			t.SetExercise(rand() % 3);
			t.SetProject(rand() % 4);
			PSCommandProvider::psInstance.AddStudent(t);
		}
		}));

	AddCommand(Command("show-students-prov", [](string c) -> void {
		PrintProvisionalStudents(PSCommandProvider::psInstance.Students);
		}));

	AddCommand(Command("seed-teachers", [](string c) -> void {
		int st = 0;
		int en = 0;
		string a = substrBetween(c, '(', ')', st, en);
		if (a == "") a = "1";
		int count = 0;
		try
		{
			count = stol(a);
		}
		catch (const std::exception&)
		{
			PSCommandProvider::ShowErrore("Enter number for count...");
			return;
		}

		for (int i = 0; i < count; i++)
		{
			string name = "amirreza t" + to_string(i + 1);
			string family = "hosseini" + to_string(i + 1);
			int age = i + 25;
			GenderType gender = GenderType::male;
			Teacher t(name, family, age, gender);
			t.AddTime(rand() % 30);
			PSCommandProvider::psInstance.AddTeacher(t);
		}
		}));

	AddCommand(Command("edit-teacher-base-salary", [](string c) ->void {
		int st = 0;
		int en = 0;
		string a = substrBetween(c, '(', ')', st, en);
		if (a == "") a = "1";
		long salary = 0;
		try
		{
			salary = stoi(a);
		}
		catch (const std::exception&)
		{
			PSCommandProvider::ShowErrore("Enter number for salary...");
			return;
		}

		Teacher::ChangeBase(salary);
		}));

	AddCommand(Command("add-teacher-time", [](string c) ->void {
		int st = 0;
		vector<string> strs = substrsBetween(c, '(', ')', st, 3);
		string name = strs[0];
		string family = strs[1];
		string ti = strs[2];

		if (ti == "") ti = "1";
		int time = 0;
		try
		{
			time = stoi(ti);
		}
		catch (const std::exception&)
		{
			PSCommandProvider::ShowErrore("Enter number for time...");
			return;
		}

		int index = PSCommandProvider::psInstance.FirstTeacherIndex(name, family);
		if (index == -1) {
			cout << "teacher with name :" << name << " and family :" << family << " not found ..." << endl;
			return;
		}

		PSCommandProvider::psInstance.Teachers[index].AddTime(time);
		}));

	AddCommand(Command("backup-teachers", [](string c) -> void {
		int st = 0, en = 0;
		string filePath = substrBetween(c, '(', ')', st, en);
		if (filePath == "") filePath = "untitle.txt";
		FileManager psFileManager(filePath);

		vector<Teacher> teachers = PSCommandProvider::psInstance.Teachers;
		int size = teachers.size();
		string baseSalary;
		if (size > 0)
		{
			baseSalary = to_string(GetTeacherBaseSalary());
		}
		for (int i = 0; i < size; i++)
		{
			string bc = "add-teacher:";
			string name = teachers[i].Name;
			string family = teachers[i].Family;
			string age = to_string(teachers[i].Age);
			char gender = char(teachers[i].Gender);
			string time = to_string(GetTeacherTime(teachers[i]));

			try
			{
				bc += "(" + name + ")" + "(" + family + ")" + "(" + age + ")" + "(" + gender + ")";
				psFileManager.WriteLine(bc);

				bc = "add-teacher-time:(" + name + ")" + "(" + family + ")" + "(" + time + ")";
				psFileManager.WriteLine(bc);

				if (i == size - 1)
				{
					bc = "edit-teacher-base-salary:(" + baseSalary + ")";
					psFileManager.WriteLine(bc);
				}
				CommandProvider::ShowInfo("Successfully backed up ( " + name + " " + family + " ) in the file.");
			}
			catch (const std::exception&)
			{
				CommandProvider::ShowErrore("There was a problem in backing up ( " + name + " " + family + " ) in the file.");
			}
		}
		psFileManager.CloseAll();
		}));

	AddCommand(Command("backup-students", [](string c) -> void {
		int st = 0, en = 0;
		string filePath = substrBetween(c, '(', ')', st, en);
		if (filePath == "") filePath = "untitle.txt";
		FileManager psFileManager(filePath);

		vector<Student> students = PSCommandProvider::psInstance.Students;
		int size = students.size();
		for (int i = 0; i < size; i++)
		{
			string bc = "add-student:";
			string name = students[i].Name;
			string family = students[i].Family;
			string age = to_string(students[i].Age);
			char gender = char(students[i].Gender);

			string mid = to_string(GetStudentMidTerm(students[i]));
			string fin = to_string(GetStudentFinalTerm(students[i]));
			string exc = to_string(GetStudentExercise(students[i]));
			string prj = to_string(GetStudentProject(students[i]));

			try
			{
				bc += "(" + name + ")" + "(" + family + ")" + "(" + age + ")" + "(" + gender + ")";
				psFileManager.WriteLine(bc);

				bc = "edit-student-score:(" + name + ")(" + family + ")(" + mid + ")(" + fin + ")(" + exc + ")(" + prj + ")";
				psFileManager.WriteLine(bc);

				CommandProvider::ShowInfo("Successfully backed up ( " + name + " " + family + " ) in the file.");
			}
			catch (const std::exception&)
			{
				CommandProvider::ShowErrore("There was a problem in backing up ( " + name + " " + family + " ) in the file.");
			}
		}
		psFileManager.CloseAll();
		}));

	AddCommand(Command("backup-all", [](string c)->void {
		PSCommandProvider::psInstance.RunCommand("backup-teachers", c);
		PSCommandProvider::psInstance.RunCommand("backup-students", c);
		}));

	AddCommand(Command("execute-file", [](string c)-> void {
		int st = 0, en = 0;
		string filePath = substrBetween(c, '(', ')', st, en);
		if (filePath == "") filePath = "untitle.txt";
		FileManager psFileManager(filePath);

		string fc = psFileManager.ReadLine();
		while (fc != "")
		{
			try
			{
				PSCommandProvider::psInstance.RunCommand(fc);
				CommandProvider::ShowInfo("Successful execution of : " + fc);
			}
			catch (const std::exception&)
			{
				CommandProvider::ShowErrore("Unsuccessful execution :" + fc);
			}
			fc = psFileManager.ReadLine();
		}
		}));

	psInstance = *this;
	RunCommand("seed-students:(50)");
	RunCommand("seed-teachers:(50)");
}


//main.cpp
int main()
{
	PSCommandProvider PSCP;
	PSCP.RunProgramm();

	cout << "End";

	return 0;
}