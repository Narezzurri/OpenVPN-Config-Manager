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

#ifdef AUTH_TRACE
#define TRACE(...) do { cerr << "[Auth] " << __VA_ARGS__ << endl; } while (false)
#else
#define TRACE(...) ((void) 0)
#endif

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
#ifndef		SILENT
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
			if ([&] () -> bool
			{
				string arg = argv[i] + 1;
				if (arg == "c" || arg == "cred")
					return nxt + 1 < argc && (credential = argv[++nxt], true);
				else
				{
					cout << "Unrecognized argument : " << argv[i] << ".Skip." << endl;
					return 0;
				}
			}())
				i = nxt;
			else
				cout << "Miss or Invalid argument for argument " << i << " : " << argv[i] << ".Skip." << endl;
		}
	}
	TRACE ("arguments parsed");
	int cnt = 0;
	for (int i = 0; i < files.size(); i++)
	{
		string filename = files[i];
		TRACE ("file[" << i << "]=" << filename);
		cout << filename << " : ";
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
		TRACE ("read complete; bytes=" << cfg.size());
		while (cfg.find('\n') != string::npos)
		{
			int idx = cfg.find('\n');
			line.emplace_back(cfg.substr(0, idx + 1));
			cfg = cfg.substr(idx + 1);
		}
		line.emplace_back(cfg);
		TRACE ("split complete; lines=" << line.size());
		int lineIndex = 0;
		for (auto &s : line)
		{
			TRACE ("line[" << lineIndex++ << "] start; bytes=" << s.size());
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
			TRACE ("line scan start; end=" << end);
			while (pos < end && s.find("auth-user-pass", pos) != string::npos)
			{
				TRACE ("auth-user-pass found; pos=" << pos);
				int idx = s.find("auth-user-pass", pos) + 14;
				TRACE ("checking token boundary; index=" << idx << " size=" << s.size());
				bool followedByAlnum = idx != s.length() && isalnum (static_cast<unsigned char> (s[idx]));
				TRACE ("token boundary checked; alnum=" << followedByAlnum);
				if (followedByAlnum)
				{
					pos = idx;
					continue;
				}
				int nxt = s.find(credential, idx);
				TRACE ("credential search complete; result=" << nxt);
				if (!~nxt)
					nxt = s.length();
				int firstNonSpace = s.find_first_not_of(' ', nxt);
				TRACE ("suffix scan complete; result=" << firstNonSpace << " credential index=" << nxt);
				if (firstNonSpace != nxt)
				{
					TRACE ("inserting credential path; index=" << idx);
					s.insert(idx, ' ' + credential);
				}
				pos = idx + credential.length() + 1;
				end += credential.length() + 1;
			}
			s += comment;
			TRACE ("line complete; bytes=" << s.size());
		}
		TRACE ("write start");
		ofstream fileout (filename, ios::binary);
		if (!fileout.is_open())
		{
			puts ("Cannot write into file.");
			continue;
		}
		for (auto &s : line)
			fileout.write(s.data(), s.length());
		fileout.close();
		TRACE ("write complete");
		puts ("Done.");
		cnt++;
	}
	TRACE ("all files processed; count=" << cnt);
	cout << cnt << " files appended successfully." << endl;
	TRACE ("reopening console input");
	freopen ("CON", "r", stdin);
	TRACE ("console input reopened");
#ifndef		SILENT
	TRACE ("waiting for input");
	getchar ();
#endif
	TRACE ("returning from main");
	return 0;
}
