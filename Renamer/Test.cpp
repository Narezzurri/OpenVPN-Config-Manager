#include	<gtest/gtest.h>
#include	<filesystem>
#include	"lib.h"
using namespace std;
using namespace filesystem;

vector<string> new_templates = 
{
	"UnitedStates#1-UDP.ovpn", "UnitedStates#1-udp-authed.ovpn",
	"UnitedKingdom#2-UDP.ovpn", "UnitedKingdom#2-udp-authed.ovpn",
	"Canada#3-TCP.ovpn", "Canada#3-tcp-authed.ovpn",
	"Japan#4-TCP.ovpn", "Japan#4-tcp-authed.ovpn"
};

class Feature : public testing::Test
{
protected:
	void SetUp (void) override
	{
		system ("rmdir Templates /s /q");
		system ("xcopy ..\\Templates Templates /I");
	}
	void TearDown (void) override
	{
		// Old files
		for (auto i : templates)
		{
			EXPECT_FALSE (exists ("Templates\\" + i + ".ovpn"));
			EXPECT_FALSE (exists ("Templates\\" + i + "-authed.ovpn"));
		}
		// New files
		for (auto i : new_templates)
			EXPECT_TRUE (exists ("Templates\\" + i));
	}
};

TEST (Help, Help)
{
	EXPECT_EQ (0, system ("Renamer			> stdout.Help.Help.1 && fc stdout.Help.Help.1 Help.txt /W"));
	EXPECT_EQ (0, system ("Renamer --help	> stdout.Help.Help.2 && fc stdout.Help.Help.2 Help.txt /W"));
	EXPECT_EQ (0, system ("Renamer /help	> stdout.Help.Help.3 && fc stdout.Help.Help.3 Help.txt /W"));
	EXPECT_EQ (0, system ("Renamer /?		> stdout.Help.Help.4 && fc stdout.Help.Help.4 Help.txt /W"));
}

TEST_F (Feature, Basic)
{
	string cmd = "Renamer ";
	for (auto i : templates)
	{
		cmd += "Templates\\" + i + ".ovpn ";
		cmd += "Templates\\" + i + "-authed" + ".ovpn ";
	}
	cmd += "Templates\\Template.ini";
	EXPECT_EQ (0, system (cmd.c_str()));
}
TEST_F (Feature, Wildcard) { EXPECT_EQ (0, system ("Renamer Templates\\*")); }
