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
		fputs ("No config assigned.\n", stderr);
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	INIReader rule (files[config]);
	if (rule.ParseError() < 0)
	{
		cerr << "Fail to load config file: " << files[config] << endl;
		cerr << "Error message: " << rule.ParseErrorMessage() << endl;
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	string pattern = get_ini (rule, "Template", "pattern");
	string target = get_ini (rule, "Template", "target");
	if (pattern.empty())
	{
		fputs ("Miss or invalid pattern in the config file.\n", stderr);
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	if (target.empty())
	{
		fputs ("Miss or invalid target in the config file.\n", stderr);
#ifndef		DEBUG
		getchar ();
#endif
		return 1;
	}
	map<string,int> mp;
	if (parse_error (string2re (rule, pattern, mp, pattern), "pattern", stderr))
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
			if (parse_error (re2string (rule, target, match, mp, ans.back().second), "target", stderr))
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
#ifndef		DEBUG
	if (toupper (getchar ()) != 'Y')
		puts ("Canceled.");
	else
#endif
	{
		vector<pss> succeed;
		for (auto [raw, nw] : ans) if (!system (("move " + raw + ' ' + parse_filepath (raw).first + '\\' + nw).c_str()))
			succeed.emplace_back(raw, nw);
		cout << succeed.size() << " file(s) renamed." << endl;
#ifdef		DEBUG
		int error = 0;
		for (auto [raw, nw] : succeed) if (error |= system (("move " + parse_filepath (raw).first + '\\' + nw + ' ' + raw + " > nul").c_str()))
			cerr << "Roll back failed: " << quote (raw) << " -> " << quote (nw) << endl;
		return error;
#endif
	}
	getchar ();
	return 0;
}
