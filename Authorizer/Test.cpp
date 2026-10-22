#include	<gtest/gtest.h>
#include	<fstream>
#include	<chrono>
#include	"lib.h"
using namespace std;
using namespace chrono;

class Feature : public testing::Test
{
protected:
	db minthr = 50;
	db maxthr = 1000;
	steady_clock::time_point birth;
	void SetUp (void) override
	{
		birth = steady_clock::now();
		ASSERT_EQ (0, system ("xcopy ..\\Templates Templates /I /Y"));
	}
	void TearDown (void) override
	{
		for (auto i : templates)
			EXPECT_EQ (0, system ("fc Templates\\" + i + ".ovpn Templates\\" + i + "-authed.ovpn"));
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
class Dryrun : public testing::Test
{
protected:
	db minthr = 0;
	db maxthr = 400;
	steady_clock::time_point birth;
	void SetUp (void) override
	{
		birth = steady_clock::now();
		ASSERT_EQ (0, system ("xcopy ..\\Templates Templates /I /Y"));
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
	EXPECT_EQ (0, system ("Auth			> stdout.Help.Help.1 && fc stdout.Help.Help.1 Help.txt /W"));
	EXPECT_EQ (0, system ("Auth --help	> stdout.Help.Help.2 && fc stdout.Help.Help.2 Help.txt /W"));
	EXPECT_EQ (0, system ("Auth /help	> stdout.Help.Help.3 && fc stdout.Help.Help.3 Help.txt /W"));
	EXPECT_EQ (0, system ("Auth /?		> stdout.Help.Help.4 && fc stdout.Help.Help.4 Help.txt /W"));
}

TEST_F (Feature, Basic)
{
	string cmd = "Auth ";
	for (auto i : templates)
		cmd += "Templates\\" + i + ".ovpn ";
	EXPECT_EQ (0, system (cmd));
}
TEST_F (Feature, Wildcard) { EXPECT_EQ (0, system ("Auth Templates\\*.ovpn")); }
TEST_F (Feature, Skip_Authed)
{
	for (auto i : templates)
		EXPECT_EQ (0, system ("copy Templates\\" + i + "-authed.ovpn Templates\\" + i + ".ovpn /Y"));
	EXPECT_EQ (0, system ("Auth Templates\\*.ovpn"));
}

TEST_F (Dryrun, Dryrun)
{
	EXPECT_EQ (0, system ("Auth Templates\\*.ovpn /dry-run > stdout.Dryrun.Dryrun"));
	vector<int> mark (templates.size());
	ifstream fin ("stdout.Dryrun.Dryrun");
	for (int i = 0; i < templates.size() * 2; i++)
	{
		vector<string> input;
		do
		{
			input.emplace_back();
			getline (fin, input.back());
		}
		while (input.back() != "");
		smatch match;
		regex rgx (R"(^Auth> .*Templates\\)" REGEX_FILENAME R"(\.ovpn$)");
		ASSERT_EQ (1, regex_match (input[0], match, rgx));
		int authed = 0;
		string filename = match[1];
		if (filename.length() == 15)
		{
			authed = 1;
			filename.resize(8);
		}
		else
			ASSERT_EQ (filename.length(), 8);
		int mtch = -1;
		for (int i = 0; i < templates.size(); i++) if (templates[i] == filename)
		{
			mtch = i;
			break;
		}
		ASSERT_NE (mtch, -1);
		if (input.size() == 4)
			EXPECT_EQ (input[1], "Appendage on Line 2 : auth-user-pass -> auth-user-pass auth.txt");
		else
			ASSERT_EQ (input.size(), 3);
		EXPECT_EQ (input[input.size() - 2], "No effective updates.");
		mark[mtch] |= 1 << authed;
	}
	for (auto i : mark)
		EXPECT_EQ (mark[i], 0x03);
	string s;
	do getline (fin, s);
	while (s.empty());
	EXPECT_EQ (s, "0 file(s) appended successfully.");
}
