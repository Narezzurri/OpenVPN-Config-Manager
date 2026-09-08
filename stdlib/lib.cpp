#include	"INIReader.h"
#include	"resource.h"
#include	<windows.h>
#include	<iostream>
#include	<sstream>
#include	<format>
#include	<string>
#include	<vector>
#include	<regex>
#include	"lib.h"
using namespace std;

int get_int (const char *s, int &ans)
{
	for (int i = 0; s[i]; i++) if (!isdigit (s[i]))
		return 1;
	return stringstream (s) >> ans && ans >= 0;
}

int get_float (const char *s, db &ans)
{
	for (int i = 0; s[i]; i++) if (!isdigit (s[i]) && s[i] != '.')
		return 1;
	return stringstream (s) >> ans && ans >= 0;
}

string quote (string s)
{
	return '"' + s + '"';
}

string extract_suffix (string s, char c)
{
	if (int idx = s.find_last_of (c); idx == string::npos)
		return "";
	else
		return s.substr (idx);
}

pss parse_filepath (string filepath)
{
	string filename = extract_suffix (filepath, '\\');
	if (filename.empty())
		return pss (".", filepath);
	else
		return pss (filepath.substr(0, filepath.length() - filename.length()), filename.substr(1));
}

void Search (string filename, vector<string> &ans)
{
	string dir = "";
	if (int idx = filename.find_last_of('\\'); idx != string::npos)
		dir = filename.substr(0, idx);
	WIN32_FIND_DATAA file;
	HANDLE handle = FindFirstFileA (filename.c_str(), &file);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				ans.emplace_back(string (MAX_PATH, '\0'));
				if (dir.empty())
					GetFullPathNameA (file.cFileName, MAX_PATH, ans.back().data(), NULL);
				else
					GetFullPathNameA ((dir + '\\' + file.cFileName).c_str(), MAX_PATH, ans.back().data(), NULL);
				if (int idx = ans.back().find_first_of('\0'); idx != string::npos)
					ans.back().resize(idx);
			}
		}
		while (FindNextFileA (handle, &file));
	}
	FindClose (handle);
	return ;
}

int HelpDetected (int argc, const char* argv[])
{
	if (argc <= 1)
		return 1;
	for (int i = 1; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "--help" || arg == "/?" || arg == "/help")
			return 1;
	}
	return 0;
}

void DisplayIcons (void)
{
	HWND hwnd = GetConsoleWindow ();
	if (hwnd)
	{
		HICON hIcon = LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE (LIGHTICON));
		if (hIcon)
			SendMessage (hwnd, WM_SETICON, ICON_SMALL, (LPARAM) hIcon);
		hIcon = LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE (DARKICON));
		if (hIcon)
			SendMessage (hwnd, WM_SETICON, ICON_BIG, (LPARAM) hIcon);
	}
	return ;
}

string get_ini (const INIReader &rule, string section, string key, string default_value)
{
	string ans = rule.Get(section, '<' + key + '>', default_value);
	if (ans.empty() || ans[0] != '<' || ans.back() != '>')
		return default_value;
	else
		return ans.substr(1, ans.length() - 2);
}

int string2re (const INIReader& rule, string s, map<string,int> &mp, string& ans)
{
	mp.clear();
	ans.clear();
	int cnt = 0;
	int invar = 0;
	string varname;
	for (auto i : s)
	{
		if (i == '<')
		{
			if (invar)
				return INVALID_VARIABLE_NAME;
			else
			{
				invar = 1;
				varname.clear();
			}
		}
		else if (i == '>')
		{
			if (!invar)
				return MISSING_VARIABLE_BRACKETS;
			else
			{
				invar = 0;
				if (varname.empty())
					return INVALID_VARIABLE_NAME;
				else if (mp.count(varname))
					return DUPLICATE_VARIABLE_NAME;
				else
				{
					string type = get_ini (rule, "Map." + varname, "");
					if (type == "string")
						ans += "([^<]+)";
					else if (type == "keep")
						ans += "([^<]+)";
					else if (type == "alpha")
						ans += "([A-Za-z]+)";
					else if (type == "int")
						ans += "(\\d+)";
					else if (type == "float")
						ans += "(\\d+\\.\\d+)";
					else
						ans += "([^<]+)";
					mp[varname] = ++cnt;
				}
			}
		}
		else if (invar)
			varname += i;
		else
		{
			if (!isalnum (i) && i == '_')
				ans += '\\';
			ans += i;
		}
	}
	return 0;
}

int re2string (const INIReader& rule, string s, const smatch& match, map<string,int> mp, string& ans)
{
	ans.clear();
	int cnt = 0;
	int invar = 0;
	string varname;
	for (auto i : s)
	{
		if (i == '<')
		{
			if (invar)
				return INVALID_VARIABLE_NAME;
			else
			{
				invar = 1;
				varname.clear();
			}
		}
		else if (i == '>')
		{
			if (!invar)
				return MISSING_VARIABLE_BRACKETS;
			else
			{
				invar = 0;
				if (varname.empty() || !mp.count(varname))
					return INVALID_VARIABLE_NAME;
				else
				{
					if (string type = get_ini (rule, "Map." + varname, ""); type.empty() || type == "keep")
						ans += match[mp[varname]];
					else
						ans += get_ini (rule, "Map." + varname, match[mp[varname]]);
				}
			}
		}
		else if (invar)
			varname += i;
		else
		{
			if (!isalnum (i) && i == '_')
				ans += '\\';
			ans += i;
		}
	}
	return 0;
}

int parse_error (int error_code, string name, FILE* err)
{
	switch (error_code)
	{
		case 0:
			return 0;
		case INVALID_VARIABLE_NAME:
			fputs (("Invalid variable name in " + name + ".\n").c_str(), err);
			break;
		case MISSING_VARIABLE_BRACKETS:
			fputs (("Missing variable brackets in " + name + ".\n").c_str(), err);
			break;
		case DUPLICATE_VARIABLE_NAME:
			fputs (("Duplicate variable name in " + name + ".\n").c_str(), err);
			break;
		default:
			cerr << ("Unknown error occured when converting " + name + ".Code: ").c_str() << hex << error_code << endl;
			break;
	}
	return 1;
}
