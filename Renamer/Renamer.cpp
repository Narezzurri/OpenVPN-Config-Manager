/**
 * OpenVPN Config Manager - Renamer utility
 * Copyright (c) 2026 Narezzurri - MIT License
*/
#include	"cpp/INIReader.h"
#include	"resource.h"
#include	<iostream>
#include	<regex>
#include	"lib.h"
using namespace std;

int main (int argc, const char* argv[])
{
	DisplayIcons ();
	if (HelpDetected (argc, argv))
	{
		cout << endl << string {
			#embed	"RenamerHelp.txt"
		} << endl;
#ifndef		DEBUG
		getchar ();
#endif
		return 0;
	}
	INIReader rule (argv[1]);
	if (rule.ParseError() < 0)
	{
		cerr << "Unable to load config file: " << argv[1] << endl;
		cerr << "Error message: " << rule.ParseErrorMessage() << endl;
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	vector<string> vec (argc);
	for (int i = 2; i < argc; i++)
		vec[i] = argv[i];
	vector<string> keys = rule.Keys("Replace");
	for (auto pattern : keys)
	{
		string target = rule.Get("Replace", pattern, "");
	}
	return 0;
}
