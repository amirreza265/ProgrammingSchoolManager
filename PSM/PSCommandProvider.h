#pragma once
#include"CommandProvider.h"
#include"PSManager.h"
#include"FileManager.h"

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
		cout << "show-teachers:" << endl;
		cout << "show-students:" << endl;
		cout << "show-students-prov: <= Show Provisional Students" << endl;
		cout << "show-students-top: (count=1)" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << "edit-student-score:(name)(family)(midTerm)(final)(exercise)(projects) \n (-1) for each element means dont change score" << endl;
		cout << "edit-teacher-base-salary:(salary)" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << "seed-students: (count=1)" << endl;
		cout << "seed-teachers: (count=1)" << endl;
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
}
