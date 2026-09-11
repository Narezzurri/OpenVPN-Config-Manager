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

TEST (Bug, Bug202609090913) { EXPECT_EQ (0, system ("Checker Templates\\*.ovpn /t 2 /n 2")); }
