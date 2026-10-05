/**
 * OpenVPN Config Manager - Authorizer utility
 * Copyright (c) 2026 Narezzurri - MIT License
*/
#include	<iostream>
#include	<fstream>
#include	<sstream>
#include	<vector>
#include	"lib.h"
using namespace std;

int dryrun = 0;
vector<string> files;
string credential = "auth.txt";

int main (int argc, const char* argv[])
{
	system ("chcp 65001 > nul");
	DisplayIcons ();
	if (HelpDetected (argc, argv))
	{
		cout << string {
			#embed	"Help.txt"
		};
#ifndef		DEBUG
		getchar ();
#endif
		return 0;
	}
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] != '/')
			Search (argv[i], files);
		else
		{
			int nxt = i;
			if (i <= argc - 1 && [&] () -> bool
			{
				string arg = argv[i] + 1;
				if (arg == "c" || arg == "cred")
				{
					credential = argv[++nxt];
					return 1;
				}
				else if (arg == "dry-run")
				{
					dryrun = 1;
					return 1;
				}
				else
				{
					cout << "Unrecognized argument : " << argv[i] << ".Skip." << endl;
					return 1;
				}
			}())
				i = nxt;
			else
				cout << "Miss or Invalid argument for argument " << i << " : " << argv[i] << ".Skip." << endl;
		}
	}
	int cnt = 0;
	for (int i = 0; i < files.size(); i++)
	{
		if (i)
			puts ("");
		string filename = files[i];
		cout << "Auth> " << filename << endl;
		if (filename.substr(filename.find_last_of('.')) != ".ovpn")
		{
			puts ("Unsupported file type.");
			continue;
		}
		ifstream filein (filename, ios::binary);
		if (!filein.is_open())
		{
			puts ("Cannot open file.");
			continue;
		}
		stringstream ss;
		ss << filein.rdbuf();
		filein.close();
		vector<string> line;
		string cfg = ss.str();
		while (cfg.find('\n') != string::npos)
		{
			int idx = cfg.find('\n');
			line.emplace_back(cfg.substr(0, idx + 1));
			cfg = cfg.substr(idx + 1);
		}
		line.emplace_back(cfg);
		int file_updated = 0;
		for (int i = 0; i < line.size(); i++)
		{
			string &s = line[i];
			int sharp = s.find_first_of('#');
			string comment;
			if (sharp != string::npos)
			{
				comment = s.substr(sharp);
				s = s.substr(0, sharp);
			}
			int pos = 0;
			int end = s.find_first_of('#');
			if (end == string::npos)
				end = s.length() - 1;
			while (pos < end && s.find("auth-user-pass", pos) != string::npos)
			{
				int idx = s.find("auth-user-pass", pos) + 14;
				if (idx != s.length() && isalnum (s[idx]))
				{
					pos = idx;
					continue;
				}
				int nxt = s.find(credential, idx);
				if (!~nxt)
					nxt = s.length();
				if (string t = s; s.find_first_not_of(' ', nxt) != nxt)
				{
					t.insert(idx, ' ' + credential);
					cout << "Appendage on Line " << i + 1 << " : " << remove_end_newline (s) << " -> " << remove_end_newline (t) << endl;
					if (!dryrun)
					{
						cout << "Confirm to append?(Y/n)";
#ifdef		DEBUG
						cout << endl;
#else
						if (choice (cin))
#endif
						{
							s = t;
							file_updated = 1;
						}
					}
				}
				pos = idx + credential.length() + 1;
				end += credential.length() + 1;
			}
			s += comment;
 		}
		if (!file_updated)
		{
			puts ("No effective updates.");
			continue;
		}
		cout << "Assign the output file:(Reserved to skip)";
		string output_filename;
#ifndef		DEBUG
		getline (cin, output_filename);
		if (output_filename.empty())
#endif
			output_filename = filename;
#ifndef		DEBUG
		do
		{
#endif
			ofstream fileout (output_filename, ios::binary);
			if (!fileout.is_open())
				puts ("Cannot write into file.");
			else
			{
				for (auto s : line)
					fileout.write(s.data(), s.length());
				fileout.close();
				cout << "Written into file: " << output_filename << endl;
				cnt++;
#ifndef		DEBUG
				break;
#endif
			}
#ifndef		DEBUG
			cout << "Assign the output file:(Reserved to skip)";
			getline (cin, output_filename);
		}
		while (!output_filename.empty());
#endif
	}
	cout << endl << cnt << " file(s) appended successfully." << endl;
#ifndef		DEBUG
	getchar ();
#endif
	return 0;
}
