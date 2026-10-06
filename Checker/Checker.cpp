/**
 * OpenVPN Config Manager - Checker utility
 * Copyright (c) 2026 Narezzurri - MIT License
*/
#include	"resource.h"
#include	<windows.h>
#include	<iostream>
#include	<sstream>
#include	<vector>
#include	"lib.h"
using namespace std;
using db = double;

int period = -1;
int ping_cnt = 0;
db sec_per_wait = 1;
db ping_timeout = 1;
vector<string> files;

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
	int err = 0;
	for (int i = 1; i < argc; i++) [&] ()
	{
		if (argv[i][0] != '/')
			Search (argv[i], files);
		else
		{
			int nxt = i;
			if (i <= argc - 1 && [&] () -> bool
			{
				string arg = argv[i] + 1;
				if (arg == "t" || arg == "time")
				{
					int succeed = get_int (argv[++nxt], period);
					if (nxt == argc - 1 || !get_float (argv[++nxt], sec_per_wait))
						nxt--;
					return succeed;
				}
				else if (arg == "w" || arg == "wait")
					return get_float (argv[++nxt], ping_timeout);
				else if (arg == "n" || arg == "number")
					return get_int (argv[++nxt], ping_cnt);
				else
				{
					cout << "Unrecognized argument : " << argv[i] << ".Skip" << endl;
					err = 1;
					return 1;
				}
			}())
				i = nxt;
			else
			{
				cout << "Miss or Invalid argument for argument " << i << " : " << argv[i] << ".Skip" << endl;
				err = 1;
			}
		}
	} ();
	int cnt = 0;
	set<string> address;
	for (auto filename : files)
	{
		if (period > 0 && cnt && !(cnt % period))
			Sleep (sec_per_wait * 1000);
		cout << filename << " : ";
		if (extract_suffix (filename, '.') != ".ovpn")
			puts ("Unsupported file type.");
		else
		{
			cin.clear();
			string s;
			set<string> new_address;
			freopen (filename.c_str(), "r", stdin);
			while (cin >> s) if (s == "remote" && cin >> s && !address.count(s))
				new_address.emplace(s);
			if (new_address.empty())
				puts ("None of new server address found.");
			else
			{
				cout << new_address.size() << " New address(es) found: ";
				for (auto addr : new_address)
				{
					cout << addr << ' ';
					address.emplace(addr);
				}
				cout << endl;
			}
		}
	}
	if (address.empty())
		puts ("No server address found.");
	else
	{
		cout << address.size() << " server address(es) found." << endl;
		for (auto addr : address)
		{
			cout << "Launching connection with " << addr << endl;
			string ping = "ping " + addr + " -w " + to_string ((int) (ping_timeout * 1e3));	// Complete ping command
			if (ping_cnt)
				ping += " -n " + to_string (ping_cnt);
			else
				ping += " -t";
			string cmd = "start " + quote (addr) + " cmd /c " + quote (ping);
			if (!~period)
				cmd = "start /wait" + cmd.substr(5);
			system (cmd.c_str());
			cnt++;
			puts ("Done.");
		}
	}
	cout << cnt << " address(es) checked successfully." << endl;
	freopen ("CON", "r", stdin);
#ifndef		DEBUG
	getchar ();
#endif
	return err;
}
