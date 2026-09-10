#include	<gtest/gtest.h>
#ifndef		OVPNS
#define		OVPNS		""
#endif
using namespace std;

class Feature : public testing::Test
{
public:
	void SetUp (void) override
	{
		system ("xcopy ..\\..\\Templates Templates /I /Y");
	}
	void TearDown (void) override
	{
		EXPECT_EQ (0, system ("fc Templates\\us-1.udp.ovpn Templates\\us-1.udp-authed.ovpn"));
		EXPECT_EQ (0, system ("fc Templates\\uk-2.udp.ovpn Templates\\uk-2.udp-authed.ovpn"));
		EXPECT_EQ (0, system ("fc Templates\\ca-3.tcp.ovpn Templates\\ca-3.tcp-authed.ovpn"));
		EXPECT_EQ (0, system ("fc Templates\\jp-4.tcp.ovpn Templates\\jp-4.tcp-authed.ovpn"));
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

TEST_F (Feature, Basic) { EXPECT_EQ (0, system ("cd Templates && ..\\Auth " OVPNS)); }
TEST_F (Feature, Wildcard) { EXPECT_EQ (0, system ("Auth Templates\\*.ovpn")); }
TEST_F (Feature, Skip_Authed)
{
	EXPECT_EQ (0, system ("copy Templates\\us-1.udp-authed.ovpn Templates\\us-1.udp.ovpn /Y"));
	EXPECT_EQ (0, system ("copy Templates\\uk-2.udp-authed.ovpn Templates\\uk-2.udp.ovpn /Y"));
	EXPECT_EQ (0, system ("copy Templates\\ca-3.tcp-authed.ovpn Templates\\ca-3.tcp.ovpn /Y"));
	EXPECT_EQ (0, system ("copy Templates\\jp-4.tcp-authed.ovpn Templates\\jp-4.tcp.ovpn /Y"));
	EXPECT_EQ (0, system ("Auth Templates\\*.ovpn"));
}
