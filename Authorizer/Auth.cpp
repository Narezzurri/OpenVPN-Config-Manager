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

vector<string> files;
string credential = "auth.txt";

int main (int argc, const char* argv[])
{
	DisplayIcons ();
	if (HelpDetected (argc, argv))
	{
		cout << string {
			#embed	"Help.txt"
		} << endl;
#ifndef		DEBUG
		getchar ();
#endif
		return 0;
	}
	vector<int> mark (argc);
	for (int i = 1; i < argc; i++) if (argv[i][0] == '/') [&] ()
	{
		mark[i] = 1;
		int nxt = i;
		if (i <= argc - 1 && [&] () -> bool
		{
			string arg = argv[i] + 1;
			if (arg == "c" || arg == "credit")
			{
				credential = argv[++nxt];
				return 1;
			}
			else
			{
				cerr << "Unrecognized argument : " << argv[i] << ".Skip." << endl;
				return 0;
			}
		}())
		{
			for (int j = nxt; j > i; j--)
				mark[j] = 1;
		}
		else
			cerr << "Miss or Invalid argument for argument " << i << " : " << argv[i] << ".Skip." << endl;
	} ();
	int cnt = 0;
	for (int i = 1; i < argc; i++) if (!mark[i])
		Search (argv[i], files);
	for (int i = 0; i < files.size(); i++)
	{
		string filename = files[i];
		cerr << filename << " : ";
		if (filename.substr(filename.find_last_of('.')) != ".ovpn")
		{
			fputs ("Unsupported file type.\n", stderr);
			continue;
		}
		ifstream filein (filename, ios::binary);
		if (!filein.is_open())
		{
			fputs ("Cannot open file.\n", stderr);
			continue;
		}
		stringstream ss;
		ss << filein.rdbuf();
		filein.close();
		ofstream fileout (filename, ios::binary);
		if (!fileout.is_open())
		{
			fputs ("Cannot write into file.\n", stderr);
			continue;
		}
		vector<string> line;
		string cfg = ss.str();
		while (cfg.find('\n') != string::npos)
		{
			int idx = cfg.find('\n');
			line.emplace_back(cfg.substr(0, idx + 1));
			cfg = cfg.substr(idx + 1);
		}
		line.emplace_back(cfg);
		for (auto s : line)
		{
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
				if (int pos = s.find_first_not_of(' ', nxt); pos == string::npos || pos < nxt)
					s.insert(idx, ' ' + credential);
				pos = idx + credential.length() + 1;
				end += credential.length() + 1;
			}
			fileout.write(s.data(), s.length());
			fileout.write(comment.data(), comment.length());
			fileout.flush();
		}
		fileout.close();
		fputs ("Done.\n", stderr);
		cnt++;
	}
	cerr << cnt << " files appended successfully." << endl;
	freopen ("CON", "r", stdin);
#ifndef		DEBUG
	getchar ();
#endif
	return 0;
}
