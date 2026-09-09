#define		CONVERSION_SUCCEEDED		0x00
#define		INVALID_VARIABLE_NAME		0x01
#define		MISSING_VARIABLE_BRACKETS	0x02
#define		DUPLICATE_VARIABLE_NAME		0x03
#define		INVALID_VARIABLE_TYPE		0x04
#include	"INIReader.h"
#include	<string>
#include	<regex>
using namespace std;
using db = double;
using pss = pair<string,string>;

/** @brief Read a non-negative integer from [s] and store it in [ans] */
int get_int (string s, int &ans);
/** @brief Read a non-negative decimal fraction from [s] and store it in [ans] */
int get_float (string s, db &ans);
/** @brief Wrap a string with quotes */
string quote (string s);
/** @brief Extract a substring from [s] up to the last occurrence of [c] (included) */
string extract_suffix (string s, char c);
/** @brief Separate filepath into path and filename */
pss parse_filepath (string filepath);
/** @brief Search for files matching the wildcard and store them in [ans] */
void Search (string filename, vector<string> &ans);
/** @brief Detect "--help" "/?" "/help" in the arguments */
int HelpDetected (int argc, const char* argv[]);
/** @brief Display the application icons */
void DisplayIcons (void);
/** @brief Extract key and value from angle brackets */
string get_ini (const INIReader &rule, string section, string key, string default_value = "");
/** @brief Convert string into a regular expression */
int string2re (const INIReader& rule, string s, map<string,int> &mp, string& ans);
/** @brief Convert regular expression into common string */
int re2string (const INIReader& rule, string s, const smatch& match, map<string,int> mp, string& ans);
/** @brief Parse the error code and output details in [err] 
 *  @return 1 if error occurs */
int parse_error (int error_code, string name, FILE* err);
