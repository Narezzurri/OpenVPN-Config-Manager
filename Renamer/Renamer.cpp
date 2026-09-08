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
	string pattern = get_ini (rule, "Template", "pattern");
	string target = get_ini (rule, "Template", "target");
	if (pattern.empty())
	{
		fputs ("Miss or invalid pattern in the config file.\n", stderr);
		return 1;
	}
	if (target.empty())
	{
		fputs ("Miss or invalid target in the config file.\n", stderr);
		return 1;
	}
	map<string,int> mp;
	if (parse_error (string2re (rule, pattern, mp, pattern), "pattern", stderr))
		return 1;
	regex repattern ('^' + pattern + '$');
	vector<string> files;
	for (int i = 2; i < argc; i++)
		Search (argv[i], files);
	smatch match;
	vector<pss> ans;
	for (auto name : files)
	{
		auto [_, filename] = parse_filepath (name);
		if (regex_match (filename, match, repattern))
		{
			for (int i = 1; i <= 3; i++)
				cout << match[i] << endl;
			ans.emplace_back(name, "");
			if (parse_error (re2string (rule, target, match, mp, ans.back().second), "target", stderr))
			{
				ans.pop_back();
				return 1;
			}
		}
	}
	cout << ans.size() << " file(s) matched." << endl;
	for (auto [raw, nw] : ans)
		cout << quote (raw) << " -> " << quote (nw) << endl;
	cout << "Confirm to rename?(Y/N):";
	if (toupper (getchar ()) != 'Y')
		puts ("Canceled.");
	else
	{
		int cnt = 0;
		for (auto [raw, nw] : ans)
			cnt += !system (("move " + raw + ' ' + parse_filepath (raw).first + '\\' + nw).c_str());
		cout << cnt << " file(s) renamed." << endl;
	}
	return 0;
}
