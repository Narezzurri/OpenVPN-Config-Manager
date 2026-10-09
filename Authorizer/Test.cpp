#include	<gtest/gtest.h>
#include	<fstream>
#include	"lib.h"
using namespace std;

class Feature : public testing::Test
{
public:
	void SetUp (void) override
	{
		ASSERT_EQ (0, system ("xcopy ..\\..\\Templates Templates /I /Y"));
	}
	void TearDown (void) override
	{
		for (auto i : templates)
			EXPECT_EQ (0, system (("fc Templates\\" + i + ".ovpn Templates\\" + i + "-authed.ovpn").c_str()));
	}
};
class Dryrun : public testing::Test
{
public:
	void SetUp (void) override
	{
		ASSERT_EQ (0, system ("xcopy ..\\..\\Templates Templates /I /Y"));
	}
};

TEST (Help, Help)
{
	EXPECT_EQ (0, system ("Auth > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Auth --help > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Auth /help > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Auth /? > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
}

TEST_F (Feature, Basic)
{
	string cmd = "Auth ";
	for (auto i : templates)
		cmd += "Templates\\" + i + ".ovpn ";
	EXPECT_EQ (0, system (cmd.c_str()));
}
TEST_F (Feature, Wildcard) { EXPECT_EQ (0, system ("Auth Templates\\*.ovpn")); }
TEST_F (Feature, Skip_Authed)
{
	for (auto i : templates)
		EXPECT_EQ (0, system (("copy Templates\\" + i + "-authed.ovpn Templates\\" + i + ".ovpn /Y").c_str()));
	EXPECT_EQ (0, system ("Auth Templates\\*.ovpn"));
}

TEST_F (Dryrun, Dryrun)
{
	EXPECT_EQ (0, system ("Auth Templates\\*.ovpn /dry-run > stdout"));
	vector<int> mark (templates.size());
	ifstream fin ("stdout");
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
		regex rgx (R"(^Auth> ([^<]+)Templates\\([^<]+)\.ovpn$)");
		ASSERT_EQ (1, regex_match (input[0], match, rgx));
		int authed = 0;
		string filename = match[2];
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
