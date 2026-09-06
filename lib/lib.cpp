#include	"resource.h"
#include	<windows.h>
#include	<sstream>
#include	<vector>
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

string extract (string s, char c)
{
	if (int idx = s.find_last_of (c); idx == string::npos)
		return "";
	else
		return s.substr (idx);
}

vector<string> Search (string filename, vector<string> &ans)
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
	return ans;
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
