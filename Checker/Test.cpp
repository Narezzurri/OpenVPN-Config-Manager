#include	<gtest/gtest.h>
#include	<fstream>
#include	"lib.h"
using namespace std;

TEST (Help, Help)
{
	EXPECT_EQ (0, system ("Checker			> stdout.Help.Help.1 && fc stdout.Help.Help.1 Help.txt /W"));
	EXPECT_EQ (0, system ("Checker --help	> stdout.Help.Help.2 && fc stdout.Help.Help.2 Help.txt /W"));
	EXPECT_EQ (0, system ("Checker /help	> stdout.Help.Help.3 && fc stdout.Help.Help.3 Help.txt /W"));
	EXPECT_EQ (0, system ("Checker /?		> stdout.Help.Help.4 && fc stdout.Help.Help.4 Help.txt /W"));
}

TEST (Feature, Basic)
{
	string cmd = "Checker ";
	for (auto i : templates)
		cmd += "..\\Templates\\" + i + ".ovpn ";
	cmd += "/w 0.8 /t 2 1 /n 2";
	EXPECT_EQ (0, system (cmd.c_str()));
}
TEST (Feature, Wildcard) { EXPECT_EQ (0, system ("Checker ..\\Templates\\*.ovpn /w 0.8 /t 2 1 /n 2")); }
TEST (Feature, PrintAddress)
{
	EXPECT_EQ (0, system ("Checker ..\\Templates\\*.ovpn /w 0.8 /t 2 1 /n 2 > stdout.Feature.PrintAddress"));
	ifstream fin ("stdout.Feature.PrintAddress");
	string input;
	for (int i = 0; i < 8; i++)
		getline (fin, input);
	getline (fin, input); EXPECT_EQ (input, "4 server address(es) found.");
	getline (fin, input); EXPECT_EQ (input, "Launching connection with ja.wikipedia.org : Done.");
	getline (fin, input); EXPECT_EQ (input, "Launching connection with www.bbc.com : Done.");
	getline (fin, input); EXPECT_EQ (input, "Launching connection with www.cbc.ca : Done.");
	getline (fin, input); EXPECT_EQ (input, "Launching connection with www.github.com : Done.");
	getline (fin, input); EXPECT_EQ (input, "4 address(es) checked successfully.");
}

TEST (Bug, Bug202609090913) { EXPECT_EQ (0, system ("Checker ..\\Templates\\*.ovpn /t 2 /n 2")); }
