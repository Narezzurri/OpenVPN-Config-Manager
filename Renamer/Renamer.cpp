/**
 * OpenVPN Config Manager - Renamer utility
 * Copyright (c) 2026 Narezzurri - MIT License
*/
#include	"INIReader.h"
#include	"resource.h"
#include	<iostream>
#include	<regex>
#include	"lib.h"
using namespace std;
using pss = pair<string,string>;

int main (int argc, const char* argv[])
{
	system ("chcp 65001 > nul");
	DisplayIcons ();
	if (HelpDetected (argc, argv))
	{
		cout << endl << string {
			#embed	"Help.txt"
		} << endl;
#ifndef		DEBUG
		getchar ();
#endif
		return 0;
	}
	vector<string> files;
	for (int i = 1; i < argc; i++)
		Search (argv[i], files);
	int config = -1;
	for (int i = 0; i < files.size(); i++) if (extract_suffix (files[i], '.') == ".ini")
	{
		config = i;
		break;
	}
	if (!~config)
	{
		puts ("No config assigned.");
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	INIReader rule (files[config]);
	if (rule.ParseError() < 0)
	{
		cout << "Fail to load config file: " << files[config] << endl;
		cout << "Error message: " << rule.ParseErrorMessage() << endl;
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	string pattern = get_ini (rule, "Template", "pattern");
	string target = get_ini (rule, "Template", "target");
	if (pattern.empty())
	{
		puts ("Miss or invalid pattern in the config file.");
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	if (target.empty())
	{
		puts ("Miss or invalid target in the config file.");
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	map<string,int> mp;
	if (parse_error (string2re (rule, pattern, mp, pattern), "pattern", stdout))
	{
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	regex repattern ('^' + pattern + '$');
	smatch match;
	vector<pss> ans;
	for (int i = 0; i < files.size(); i++) if (i != config)
	{
		string filename = parse_filepath (files[i]).second;
		if (regex_match (filename, match, repattern))
		{
			ans.emplace_back(files[i], "");
			if (parse_error (re2string (rule, target, match, mp, ans.back().second), "target", stdout))
			{
				ans.pop_back();
#ifndef		DEBUG
				getchar ();
#endif
				return 1;
			}
		}
	}
	cout << ans.size() << " file(s) matched." << endl;
	for (auto [raw, nw] : ans)
		cout << quote (raw) << " -> " << quote (nw) << endl;
	cout << "Confirm to rename?(Y/N):";
#ifdef		DEBUG
	cout << endl;
#else
	if (toupper (getchar ()) != 'Y')
		puts ("Canceled.");
	else
#endif
	{
		vector<pss> succeed;
		for (auto [raw, nw] : ans) if (!system (("move " + raw + ' ' + parse_filepath (raw).first + '\\' + nw).c_str()))
			succeed.emplace_back(raw, nw);
		cout << succeed.size() << " file(s) renamed." << endl;
		// int error = 0;
		// for (auto [raw, nw] : succeed) if (error |= system (("move " + parse_filepath (raw).first + '\\' + nw + ' ' + raw + " > nul").c_str()))
		// 	cout << "Roll back failed: " << quote (raw) << " -> " << quote (nw) << endl;
		// return error;
	}
#ifndef		DEBUG
	getchar ();
	getchar ();
#endif
	return 0;
}
