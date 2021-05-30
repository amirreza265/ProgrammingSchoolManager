#pragma once
#include <string>
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