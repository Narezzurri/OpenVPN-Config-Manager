#include	<gtest/gtest.h>
#include	<filesystem>
#ifndef		OVPNS
#define		OVPNS				""
#endif
#ifndef		OVPNS_AUTHED
#define		OVPNS_AUTHED		""
#endif
using namespace std;
using namespace filesystem;

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
		EXPECT_FALSE (exists ("Templates\\us-1.udp.ovpn"));
		EXPECT_FALSE (exists ("Templates\\uk-2.udp.ovpn"));
		EXPECT_FALSE (exists ("Templates\\ca-3.tcp.ovpn"));
		EXPECT_FALSE (exists ("Templates\\jp-4.tcp.ovpn"));
		EXPECT_FALSE (exists ("Templates\\us-1.udp-authed.ovpn"));
		EXPECT_FALSE (exists ("Templates\\uk-2.udp-authed.ovpn"));
		EXPECT_FALSE (exists ("Templates\\ca-3.tcp-authed.ovpn"));
		EXPECT_FALSE (exists ("Templates\\jp-4.tcp-authed.ovpn"));
		// New files
		EXPECT_TRUE (exists ("Templates\\UnitedStates#1-UDP.ovpn"));
		EXPECT_TRUE (exists ("Templates\\UnitedKingdom#2-UDP.ovpn"));
		EXPECT_TRUE (exists ("Templates\\Canada#3-TCP.ovpn"));
		EXPECT_TRUE (exists ("Templates\\Japan#4-TCP.ovpn"));
		EXPECT_TRUE (exists ("Templates\\UnitedStates#1-udp-authed.ovpn"));
		EXPECT_TRUE (exists ("Templates\\UnitedKingdom#2-udp-authed.ovpn"));
		EXPECT_TRUE (exists ("Templates\\Canada#3-tcp-authed.ovpn"));
		EXPECT_TRUE (exists ("Templates\\Japan#4-tcp-authed.ovpn"));
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
