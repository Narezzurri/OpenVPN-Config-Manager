#include	<gtest/gtest.h>
#include	<climits>
#include	<fstream>
#include	<chrono>
#include	<regex>
#include	"lib.h"
using namespace std;
using namespace chrono;

class Feature : public testing::Test
{
protected:
	db minthr = 900;
	db maxthr = 2500;
	steady_clock::time_point birth;
	void SetUp (void) override
	{
		birth = steady_clock::now();
	}
	void TearDown (void) override
	{
		db dt = duration_cast<duration<db,milli> > (steady_clock::now() - birth).count();
		EXPECT_LE (minthr, dt);
		EXPECT_LE (dt, maxthr);
	}
	void SetThr (db min, db max)
	{
		minthr = min;
		maxthr = max;
	}
};

TEST (Help, Help)
{
	EXPECT_EQ (0, system ("Checker			> stdout && fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Checker --help	> stdout && fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Checker /help	> stdout && fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Checker /?		> stdout && fc stdout Help.txt /W"));
}

TEST_F (Feature, Basic)
{
	string cmd = "Checker ";
	for (auto i : templates)
		cmd += "..\\Templates\\" + i + ".ovpn ";
	cmd += "/w 0.8 /t 2 1 /n 2";
	EXPECT_EQ (0, system (cmd.c_str()));
}
TEST_F (Feature, Wildcard) { EXPECT_EQ (0, system ("Checker ..\\Templates\\*.ovpn /w 0.8 /t 2 1 /n 2")); }
TEST_F (Feature, PrintAddress)
{
	EXPECT_EQ (0, system ("Checker ..\\Templates\\*.ovpn /w 0.8 /t 2 1 /n 2 > stdout"));
	ifstream fin ("stdout");
	string input;
	vector<int> mark (templates.size());
	for (int i = 0; i < 8; i++) [&] ()
	{
		getline (fin, input);
		smatch match;
		ASSERT_EQ (1, regex_match (input, match, regex (R"(^.*Templates\\([1-4a-z.-]*)\.ovpn : (.*)$)")));
		string output = match[2];
		for (int i = 0; i < templates.size(); i++) if (match[1] == templates[i] || match[1] == templates[i] + "-authed")
		{
			if (mark[i])
				EXPECT_EQ (output, "None of new server address found.");
			else
			{
				mark[i] = 1;
				ASSERT_EQ (1, regex_match (output, match, regex (R"(^1 New address\(es\) found: ([a-z.]+) $)")));
				EXPECT_EQ (match[1], templates_address[i]);
			}
			return ;
		}
		EXPECT_EQ ("", ("Illegal filename output : " + output).c_str());
	} ();
	for (auto i : mark)
		EXPECT_EQ (i, 1);
	getline (fin, input); EXPECT_EQ (input, "4 server address(es) found.");
	getline (fin, input); EXPECT_EQ (input, "Launching connection with ja.wikipedia.org : Done.");
	getline (fin, input); EXPECT_EQ (input, "Launching connection with www.bbc.com : Done.");
	getline (fin, input); EXPECT_EQ (input, "Launching connection with www.cbc.ca : Done.");
	getline (fin, input); EXPECT_EQ (input, "Launching connection with www.github.com : Done.");
	getline (fin, input); EXPECT_EQ (input, "4 address(es) checked successfully.");
}

TEST (Bug, Bug202609090913) { EXPECT_EQ (0, system ("Checker ..\\Templates\\*.ovpn /t 2 /n 2")); }
