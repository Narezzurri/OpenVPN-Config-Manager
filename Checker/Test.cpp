#include	<gtest/gtest.h>
#ifndef		OVPNS
#define		OVPNS		""
#endif
using namespace std;

TEST (Help, Help)
{
	EXPECT_EQ (0, system ("Checker			> stdout && fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Checker --help	> stdout && fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Checker /help	> stdout && fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Checker /?		> stdout && fc stdout Help.txt /W"));
}

TEST (Feature, Basic) { EXPECT_EQ (0, system ("cd Templates && ..\\Checker " OVPNS " /w 0.8 /t 2 1 /n 2")); }
TEST (Feature, Wildcard) { EXPECT_EQ (0, system ("Checker Templates\\*.ovpn /w 0.8 /t 2 1 /n 2")); }
TEST (Feature, PrintAddress)
{
	EXPECT_EQ (0, system ("Checker Templates\\*.ovpn /w 0.8 /t 2 1 /n 2 > stdout"));
	freopen ("stdout", "r", stdin);
	string input;
	for (int i = 0; i < 8; i++)
		getline (cin, input);
	getline (cin, input); EXPECT_EQ (input, "4 server address(es) found.");
	getline (cin, input); EXPECT_EQ (input, "Launching connection with ja.wikipedia.org");
	getline (cin, input); EXPECT_EQ (input, "Done.");
	getline (cin, input); EXPECT_EQ (input, "Launching connection with www.bbc.com");
	getline (cin, input); EXPECT_EQ (input, "Done.");
	getline (cin, input); EXPECT_EQ (input, "Launching connection with www.cbc.ca");
	getline (cin, input); EXPECT_EQ (input, "Done.");
	getline (cin, input); EXPECT_EQ (input, "Launching connection with www.github.com");
	getline (cin, input); EXPECT_EQ (input, "Done.");
	getline (cin, input); EXPECT_EQ (input, "4 address(es) checked successfully.");
	freopen ("CON", "r", stdin);
}

TEST (Bug, Bug202609090913) { EXPECT_EQ (0, system ("Checker Templates\\*.ovpn /t 2 /n 2")); }
