#pragma once
#include<fstream>
#include <string>
using namespace std;

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
	oFile = new ofstream(fileName,ios::app);
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
