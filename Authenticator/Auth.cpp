/**
 * OpenVPN Config Manager - Authenticator utility
 * Copyright (c) 2026 Narezzurri - MIT License
*/
#include	<iostream>
#include	<fstream>
#include	<sstream>
#include	<vector>
#include	"lib.h"
using namespace std;

vector<string> files;
string content = "auth.txt";

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
	int cnt = 0;
	for (int i = 1; i < argc; i++)
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
			int pos = 0;
			int end = s.find_first_of('#');
			if (end == string::npos)
				end = s.length() - 1;
			while (pos < end && s.substr(0, end).find("auth-user-pass", pos) != string::npos)
			{
				int idx = s.substr(0, end).find("auth-user-pass", pos) + 14;
				s.insert(idx, ' ' + content);
				pos = idx + content.length() + 1;
				end += content.length() + 1;
			}
			fileout.write(s.data(), s.length());
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
