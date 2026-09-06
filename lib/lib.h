#include	<string>
using namespace std;
using db = double;

/* @brief Read a non-negative integer from [s] and store it in [ans] */
int get_int (const char *s, int &ans);
/* @brief Read a non-negative decimal fraction from [s] and store it in [ans] */
int get_float (const char *s, db &ans);
/* @brief Wrap a string with quotes */
string quote (string s);
/* @brief Extract a substring from [s] up to the last occurrence of [c] (included) */
string extract (string s, char c);
/* @brief Search for files matching the wildcard and store them in [ans] */
vector<string> Search (string filename, vector<string> &ans);
/* @brief Detect "--help" "/?" "/help" in the arguments */
int HelpDetected (int argc, const char* argv[]);
/* @brief Display the application icons */
void DisplayIcons (void);
