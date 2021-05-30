#pragma once
#include<vector>
#include <string>
#include "ConsoleManager.h"
using namespace std;

class Table
{
public:
	Table(vector<string> headers);
	void AddRow(vector<string> row);
	//void AddColumn(string columnHead);
	string CreateTable();
	void SetTableBorder(const char wall = '|', const char ceiling = '-', const char ceilinginter= 'o', const char intersection = 'o', const char rowCeiling = '.');
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
//
//inline void Table::AddColumn(string columnHead)
//{
//	_headers.push_back(columnHead);
//	_columnCount = _headers.size();
//	_maxColLen.push_back(columnHead.length());
//}

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
		vector<string> rowrec =_records[index];
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

inline string Table::GetCeiling(const char ceiling , char intersection)
{
	string ceil = "";
	for (int i = 0; i < _columnCount; i++)
	{
		ceil += string(_maxColLen[i], ceiling);
		ceil += (i < _columnCount - 1) ? intersection : '\n';
	}
	return ceil;
}

