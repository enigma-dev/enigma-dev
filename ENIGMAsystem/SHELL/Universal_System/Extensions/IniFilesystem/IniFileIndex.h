// 
// Copyright (C) 2014 Seth N. Hetu
// 
// This file is a part of the ENIGMA Development Environment.
// 
// ENIGMA is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, version 3 of the license or any later version.
// 
// This application and its source code is distributed AS-IS, WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with this code. If not, see <http://www.gnu.org/licenses/>
//

#include <map>
#include <string>
#include <vector>
#include <istream>

namespace enigma {

struct IniValue;
struct IniFileSection;

class IniFileIndex {
public:
	IniFileIndex() : commentChar(';') {}

	void load(std::istream& input, char commentChar);
	void clear();

	bool keyExists(const std::string& section, const std::string& key);
	bool sectionExists(const std::string& section);

	void write(const std::string& section, const std::string& key, const std::string& value);
	void write(const std::string& section, const std::string& key, float value);

	std::string read(const std::string& section, const std::string& key, std::string def) const;
	float read(const std::string& section, const std::string& key, float def) const;

	void delKey(const std::string& section, const std::string& key);
	void delSection(const std::string& section);

	bool saveToFile(const std::string& fname) const; //Returns false if the file could not be opened in write mode.
	std::string toString() const;

private:
	//The authoritative storage of Sections, by name.
	std::map<std::string, IniFileSection> sections;

	//List of Section names in order, as they were loaded from the file.
	std::vector<std::string> secNameOrder;

	//Any comments after the last property.
	std::string postComment;

	//The character we are treating as the comment identifier. ';' by default, but '#' is also common.
	char commentChar;
};


struct IniFileSection {
	//All lines preceeding this section (but after the previous section or value). Typically comments.
	std::string prefix;

	//The authoritative storage of key,value properties.
	std::map<std::string, IniValue> props;

	//List of property keys in order, as they were loaded from the file.
	std::vector<std::string> propKeyOrder;
};

struct IniValue {
	std::string value; //The value (trimmed), loaded from the file.
	std::string prefix; //All lines (usually comments) before this line (but after the previous key/section).
	std::string postfix; //Any comment after the value, but on the same line.
};


}


