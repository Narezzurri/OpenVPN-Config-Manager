#include	<gtest/gtest.h>
#include	"lib.h"
using namespace std;

class Feature : public testing::Test
{
protected:
	void SetUp (void) override
	{
		ASSERT_EQ (0, system ("xcopy ..\\Templates Templates /I /Y"));
	}
	void TearDown (void) override
	{
		for (auto i : templates)
			EXPECT_EQ (0, system (("fc Templates\\" + i + ".ovpn Templates\\" + i + "-authed.ovpn").c_str()));
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
	EXPECT_EQ (0, system (cmd.c_str()));
}
TEST_F (Feature, Wildcard) { EXPECT_EQ (0, system ("Auth Templates\\*.ovpn")); }
TEST_F (Feature, Skip_Authed)
{
	EXPECT_EQ (0, system ("copy Templates\\us-1.udp-authed.ovpn Templates\\us-1.udp.ovpn /Y"));
	EXPECT_EQ (0, system ("copy Templates\\uk-2.udp-authed.ovpn Templates\\uk-2.udp.ovpn /Y"));
	EXPECT_EQ (0, system ("copy Templates\\ca-3.tcp-authed.ovpn Templates\\ca-3.tcp.ovpn /Y"));
	EXPECT_EQ (0, system ("copy Templates\\jp-4.tcp-authed.ovpn Templates\\jp-4.tcp.ovpn /Y"));
	EXPECT_EQ (0, system ("Auth Templates\\*.ovpn"));
}
