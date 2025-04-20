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

#include "UNIXiniindex.h"

#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <fstream>

namespace {

std::string ltrim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\r");
	if (start != std::string::npos) {
		return str.substr(start);
	}
	return str;
}

std::string rtrim(const std::string& str) {
	size_t last = str.find_last_not_of(" \t\r");
	if (last != std::string::npos) {
		return str.substr(0, last+1);
	}
	return str;
}

std::string trim(const std::string& str) {
	return rtrim(ltrim(str));
}

std::string optional_quote(const std::string& str, const char commentChar) {
	if (!str.empty() && (str[0]==' ' || str[0]=='\t' || str[str.size()-1]==' ' || str[str.size()-1]=='\t' || str.find(commentChar)!=std::string::npos)) {
		return std::string("\"") + str + "\"";
	}
	return str;
}

} //En un-named namespace


void enigma::IniFileIndex::load(std::istream& input, char commentChar)
{
	//Save the comment char
	this->commentChar = commentChar;

	//Track the current section.
	std::string secName;

	//Track the current "comment" string.
	std::stringstream comment;

	//Read all properties.
	std::string line;
	std::string origLine;
	while(std::getline(input, origLine)) {
		//Trim front and back whitespace, but save the original line.
		line = trim(origLine);

		//Now, dispatch based on the current line type. Invalid lines are pruned and ignored.
		if (line.empty() || line[0]==commentChar) {
			//It's a comment; append it to the current comment string.
			comment <<origLine <<"\n";
		} else if (line[0]=='[') {
			//Find the closing bracket.
			size_t br = line.find(']', 1);
			if (br != std::string::npos) {
				//It's a section; give it a section name. 
				secName = line.substr(1, br-1);

				//Make sure there's no double-open-bracket.
				if (secName.find('[')==std::string::npos) {
					//No duplicates
					if (sections.count(secName)==0) {
						sections[secName].prefix = comment.str();
						secNameOrder.push_back(secName);
					}
				}
			}
			//Either way, clear the comment string (so the user doesn't think a section was read when it actually wasn't).
			comment.str("");
		} else {
			//Comments and empty lines were already taken care of, so all lines should be of the form {K = "V" ;C} with optional comment and quotes.
			size_t eq = line.find('=');
			if (eq != std::string::npos) {
				std::string key = trim(line.substr(0,eq));
				std::string value;

				//The value is complicated by the fact that there might be a quoted string (with escaped quotes). So we have to scan it.
				std::string rhs = trim(line.substr(eq+1));
				if (!rhs.empty() && rhs[0]=='"') {
					//Quoted string.
					size_t qt = rhs.find('"', 1);
					if (qt==std::string::npos) {
						rhs = ""; //Only occurs if they forgot to close the quote. Kill the value.
					} else {
						value = rhs.substr(1,qt-1); //DON'T trim; that's why we have the quote.
						rhs = trim(rhs.substr(qt+1));
					}
				} else {
					//Non-quoted string.
					size_t ct = rhs.find(commentChar, 1);
					if (ct==std::string::npos) {
						value = rhs;
						rhs = "";
					} else {
						value = trim(rhs.substr(0,ct));
						rhs = trim(rhs.substr(ct)); //Keep the comment character.
					}
				}

				//rhs must be consumed (or there's a comment)
				if (rhs.empty() || rhs[0]==commentChar) {
					//No empty strings.
					if (!value.empty()) {
						//No duplicates
						if (sections[secName].props.count(key)==0) {
							//Save it.
							sections[secName].props[key].prefix = comment.str();
							sections[secName].props[key].value = value;
							sections[secName].propKeyOrder.push_back(key);
						}
					}
				}

				//Either way, reset the comment string (again, to avoid false positives).
				comment.str("");

				//Now deal with comments.
				if (rhs[0]==commentChar) {
					sections[secName].props[key].postfix = trim(rhs);
				}
			}
		}
	}

	//Save any leftover comments.
	postComment = comment.str();
}


void enigma::IniFileIndex::clear() 
{
	sections.clear();
	secNameOrder.clear();
	postComment.clear();
}

bool enigma::IniFileIndex::keyExists(const std::string& section, const std::string& key)
{
	std::map<std::string, IniFileSection>::const_iterator secIt = sections.find(section);
	if (secIt==sections.end()) { return false; }
	std::map<std::string, IniValue>::const_iterator propIt = secIt->second.props.find(key);
	if (propIt==secIt->second.props.end()) { return false; }
	return true;
}

bool enigma::IniFileIndex::sectionExists(const std::string& section)
{
	std::map<std::string, IniFileSection>::const_iterator secIt = sections.find(section);
	if (secIt==sections.end()) { return false; }
	return true;
}

std::string enigma::IniFileIndex::read(const std::string& section, const std::string& key, std::string def) const
{
	std::map<std::string, IniFileSection>::const_iterator secIt = sections.find(section);
	if (secIt==sections.end()) { return def; }
	std::map<std::string, IniValue>::const_iterator propIt = secIt->second.props.find(key);
	if (propIt==secIt->second.props.end()) { return def; }
	return propIt->second.value;
}

float enigma::IniFileIndex::read(const std::string& section, const std::string& key, float def) const
{
	std::map<std::string, IniFileSection>::const_iterator secIt = sections.find(section);
	if (secIt==sections.end()) { return def; }
	std::map<std::string, IniValue>::const_iterator propIt = secIt->second.props.find(key);
	if (propIt==secIt->second.props.end()) { return def; }

	//TODO: Double-check GM; I think it returns 0 for invalid number strings (which is what atof does).
	return (float)atof(propIt->second.value.c_str());
}

void enigma::IniFileIndex::write(const std::string& section, const std::string& key, const std::string& value)
{
	//Keep our ordering up-to-date.
	std::map<std::string, IniFileSection>::const_iterator secIt = sections.find(section);
	if (secIt==sections.end()) {
		secNameOrder.push_back(section);
	}
	std::map<std::string, IniValue>::const_iterator propIt = sections[section].props.find(key);
	if (propIt==sections[section].props.end()) {
		sections[section].propKeyOrder.push_back(key);
	}
	sections[section].props[key].value = value;
}

void enigma::IniFileIndex::write(const std::string& section, const std::string& key, float value)
{
	//Keep our ordering up-to-date.
	std::map<std::string, IniFileSection>::const_iterator secIt = sections.find(section);
	if (secIt==sections.end()) {
		secNameOrder.push_back(section);
	}
	std::map<std::string, IniValue>::const_iterator propIt = sections[section].props.find(key);
	if (propIt==sections[section].props.end()) {
		sections[section].propKeyOrder.push_back(key);
	}

	//Save the float as its string representation.
	std::ostringstream valF;
	valF<<value;
	sections[section].props[key].value = valF.str();
}

void enigma::IniFileIndex::delKey(const std::string& section, const std::string& key)
{
	//This silently fails if the section doesn't exist.
	std::map<std::string, IniFileSection>::iterator secIt = sections.find(section);
	if (secIt==sections.end()) {
		return;
	}

	//..or if the key doesn't exist.
	std::map<std::string, IniValue>::iterator propIt=secIt->second.props.find(key);
	if (propIt==secIt->second.props.end()) {
		return;
	}

	//Keep our ordering correct.
	std::vector<std::string>::iterator remIt = std::find(secIt->second.propKeyOrder.begin(), secIt->second.propKeyOrder.end(), key);
	if (remIt != secIt->second.propKeyOrder.end()) {
		secIt->second.propKeyOrder.erase(remIt);
	}

	//Remove the element.
	secIt->second.props.erase(propIt);

	//Remove the section if empty
	if (secIt->second.props.empty()) {
		delSection(section);
	}
}


void enigma::IniFileIndex::delSection(const std::string& section)
{
	//This silently fails if the section doesn't exist.
	std::map<std::string, IniFileSection>::iterator secIt = sections.find(section);
	if (secIt==sections.end()) {
		return;
	}

	//Keep our ordering correct.
	std::vector<std::string>::iterator remIt = std::find(secNameOrder.begin(), secNameOrder.end(), section);
	if (remIt != secNameOrder.end()) {
		secNameOrder.erase(remIt);
	}

	//Remove the element
	sections.erase(secIt);
}


bool enigma::IniFileIndex::saveToFile(const std::string& fname) const
{
	//Don't print empty files unless they already exist.
	if (sections.empty() && postComment.empty()) {
		std::ifstream file(fname.c_str());
		if (!file.good()) {
			return true;
		}
	}

	std::ofstream out(fname.c_str());
	if (!out.good()) {
		return false;
	}
	
	out <<this->toString();
	return true;
}

std::string enigma::IniFileIndex::toString() const
{
	std::stringstream res;
	for (std::vector<std::string>::const_iterator secIt=secNameOrder.begin(); secIt!=secNameOrder.end(); secIt++) {
		const IniFileSection& sec = sections.find(*secIt)->second;
		res <<sec.prefix;
		res <<"[" <<(*secIt) <<"]\n";
		for (std::vector<std::string>::const_iterator keyIt=sec.propKeyOrder.begin(); keyIt!=sec.propKeyOrder.end(); keyIt++) {
			const IniValue& val = sec.props.find(*keyIt)->second;
			res <<val.prefix;
			res <<*keyIt <<" = " <<optional_quote(val.value, commentChar) <<(val.postfix.empty()?"":std::string(" ")+val.postfix) <<"\n";
		}
	}
	res <<postComment;
	return res.str();
}




