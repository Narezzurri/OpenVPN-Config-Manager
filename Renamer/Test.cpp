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
public:
	void SetUp (void) override
	{
		system ("rmdir Templates /s /q");
		system ("xcopy ..\\..\\Templates Templates /I");
		system ("copy Template.ini Templates /Y");
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
	EXPECT_EQ (0, system ("Renamer > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Renamer --help > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Renamer /help > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Renamer /? > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
}

TEST_F (Feature, Basic) { EXPECT_EQ (0, system ("cd Templates && ..\\Renamer " OVPNS " " OVPNS_AUTHED " Template.ini")); }
TEST_F (Feature, Wildcard) { EXPECT_EQ (0, system ("Renamer Templates\\*")); }
