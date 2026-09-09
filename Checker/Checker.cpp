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

int T = -1;
int a, b, c, d;
int ping_cnt = 0;
db sec_per_wait = 1;
db ping_timeout = 1;
vector<string> files;

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
	vector<int> mark (argc);
	for (int i = 1; i < argc; i++) if (argv[i][0] == '/') [&] ()
	{
		mark[i] = 1;
		int nxt = i;
		if (i <= argc - 1 && [&] () -> bool
		{
			string arg = argv[i] + 1;
			if (arg == "t" || arg == "time")
			{
				int succeed = get_int (argv[++nxt], T);
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
				cerr << "Unrecognized argument : " << argv[i] << ".Skip" << endl;
				return 0;
			}
		}())
		{
			for (int j = nxt; j > i; j--)
				mark[j] = 1;
		}
		else
			cerr << "Miss or Invalid argument for argument " << i << " : " << argv[i] << ".Skip" << endl;
	} ();
	int cnt = 0;
	for (int i = 1; i < argc; i++) if (!mark[i])
		Search (argv[i], files);
	for (int i = 0; i < files.size(); i++)
	{
		if (T > 0 && cnt && !(cnt % T))
			Sleep (sec_per_wait * 1000);
		string filename = files[i];
		cerr << filename << " : ";
		if (extract_suffix (filename, '.') != ".ovpn")
			fputs ("Unsupported file type.\n", stderr);
		else
		{
			cin.clear();
			freopen (filename.c_str(), "r", stdin);
			string addr;
			cin >> addr;
			while (cin >> addr && addr != "remote");
			if (addr != "remote")
				fputs ("Server ip address not found.\n", stderr);
			else
			{
				cin >> addr;				// Server address
				string ping = "ping " + addr + " -w " + to_string ((int) (ping_timeout * 1e3));	// Complete ping command
				if (ping_cnt)
					ping += " -n " + to_string (ping_cnt);
				else
					ping += " -t";
				if (string s = extract_suffix (filename, '\\'); !s.empty())
					filename = s.substr(1);
				string cmd = "start " + quote (filename) + " cmd /c " + quote (ping);
				if (!~T)
					cmd = "start /wait" + cmd.substr(5);
				system (cmd.c_str());
				cnt++;
				fputs ("Done.\n", stderr);
			}
		}
	}
	cerr << cnt << " files checked successfully." << endl;
	freopen ("CON", "r", stdin);
#ifndef		DEBUG
	getchar ();
#endif
	return 0;
}
