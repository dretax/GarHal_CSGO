#ifndef IncConfig
#define IncConfig

#include <string>
#include <map>
#include <list>
using namespace std;

/*
   Config

   Parse structured config files

   Config files contains lines with name-value assignements in the form "<name> = <value>".
   Trailing and leading whitespace is stripped. Parsed config entries are stored in
   a symbol map.

   Lines beginning with '#' are a comment and ignored.

   Config files may be structured (to arbitrary depth). To start a new config sub group
   (or sub section) use a line in the form of "<name> = (".
   Subsequent entries are stured in the sub group, until a line containing ")" is found.

   Values may reuse already defined names as a variable which gets expanded during
   the parsing process. Names for expansion are searched from the current sub group
   upwards. Finally the process environment is searched, so also environment
   variables may be used as expansion symbols in the config file.

   Errors and warnings are handled by emitting logging messages (see log.h/log.cpp)
   or by calling exit() for severe errors. Depending on project needs this may be replaced
   by exeptions, error return codes, ...
 */

class Config {
public:
	/* Parse config file 'configFile'. If the process environment
	 * is provided, environment variables can be used as expansion symbols.
	 */
	Config(string configFile, char** envp = 0);

	~Config();

	// get string config entry
	string pString(string name);

	/* get boolean config entry
	 * A value of Yes/yes/YES/true/True/TRUE leads to true,
	 * all other values leads to false.
	 */
	bool pBool(string name);

	// get double config entry; value is parsed using atof()
	double pDouble(string name);

	// get int config entry; value is parsed using atoi()
	int pInt(string name);

	int pHex(string name);

	// get the symbol map (e.g. for iterating over all symbols)
	inline map<string, string>& getSymbols() {
		return symbols;
	}

	// get config sub group
	inline Config* group(string name) {
		return groups[name];
	}

	// get config sub group map (e.g. for iterating over all groups)
	inline map<string, Config*>& getGroups() {
		return groups;
	}

private:
	// private constructor for sub groups
	Config(string name, string parentDebugInfo);

	// helper functions for parsing
	void add(string name, string value);
	void split(string in, string& left, string& right, char c);
	void trim(string& s);
	void symbolExpand(string& s);
	void symbolExpand(map<string, string>& symbols, string& s);
	void envSymbolExpand(string& s);

	// config group symbol map
	map<string, string> symbols;

	// environment symbol map
	map<string, string> envSymbols;

	// config sub group map
	map<string, Config*> groups;

	// stack of config groups for parsing (only used in top config element)
	list<Config*> groupStack;

	// debug info used for logging messages
	string debugInfo;
};

#endif