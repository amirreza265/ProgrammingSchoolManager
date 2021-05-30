#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "ConsoleManager.h"
using namespace std;

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
		CommandProvider::ShowInfo("command was added : " + comand.Name );
	}
}

inline int CommandProvider::FindCommandIndex(string name)
{
	for (int i = 0; i < commands.size(); i++)
	{
		if (commands[i].Name.compare(name)==0)
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
	command = command.substr(command.find(':')+1);
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