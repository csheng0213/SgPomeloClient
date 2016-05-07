#include "pub.h"
#include "pub_ini.h"
#include <cstring>

#define INI_LINE_MAX 1024

static void strip(std::string& s) {
	if (s.empty())
		return;

	std::string::size_type i = 0;
	while (i < s.size()) {
		if (isspace(s[i])) {
			++i;
			continue;
		}
		else {
			break;
		}
	}

	std::string::size_type j = s.size();
	while (j > 0) {
		if (isspace(s[j - 1])) {
			--j;
			continue;
		}
		else {
			break;
		}
	}
	
	if (j <= i)
		s.clear();
	else
		s = s.substr(i, j - i);
}

static void strip_comment(std::string& s) {
	std::string::size_type p = s.find("##");
	if (p == std::string::npos)
		return;

	s = s.substr(0, p);
}

namespace pub {
	CINIParser::CINIParser() {
		
	}

	CINIParser::~CINIParser() {
		
	}

	bool CINIParser::parse_line(std::string& line, std::string& section) {
		strip_comment(line);
		strip(line);

		if (line.empty())
			return true;

		if (line[0] == '[') {
			std::string::size_type p = line.find(']');
			if (p == std::string::npos)
				return false;

			section = line.substr(1, p - 1);
			return true;
		}
		else {
			if (section.empty())
				return false;

			std::string::size_type p = line.find('=');
			if (p == std::string::npos)
				return false;

			std::string k = line.substr(0, p);
			std::string v = line.substr(p + 1);

			strip(k);
			strip(v);

			if (k.empty() || v.empty())
				return false;

			MAP_ITEM& map_item = _mapSection[section];
			map_item[k] = v;
			return true;
		}
	}

	bool CINIParser::parse(const char* file_name) {
		_mapSection.clear();
		
		std::ifstream fs(file_name);
		if (!fs.is_open())
			return false;
		
		std::string section;
		char line[INI_LINE_MAX];
		while (!fs.eof()) {
			fs.getline(line, sizeof(line));
			
			std::string sline(line);
			if (!parse_line(sline, section))
				return false;
		}

		return true;
	}

	bool CINIParser::parsemem(const void *mem, size_t len) {
		if (mem == NULL)
			return false;

		_mapSection.clear();

        auto str = std::string((const char *)mem, len);
		std::stringstream ss(str);

		std::string section;
		char line[INI_LINE_MAX];
		while (!ss.eof()) {
			ss.getline(line, sizeof(line));

			std::string sline(line);
			if (!parse_line(sline, section))
				return false;
		}

		return true;
	}

	bool CINIParser::parsestr(const char *str) {
		if (str == NULL)
			return false;

		_mapSection.clear();

		std::stringstream ss(std::string((const char *)str));

		std::string section;
		char line[INI_LINE_MAX];
		while (!ss.eof()) {
			ss.getline(line, sizeof(line));

			std::string sline(line);
			if (!parse_line(sline, section))
				return false;
		}

		return true;
	}

	bool CINIParser::get(const char* section, const char* key, int32_t* v) {
		PUB_ASSERT(v);
		std::string s;
		if (!get(section, key, &s))
			return false;

		char* endptr = NULL;
		errno = 0;
		long val = strtol(s.c_str(), &endptr, 10);
		if (errno != 0)
			return false;

		*v = (int32_t)val;
		return true;
	}

	bool CINIParser::get(const char* section, const char* key, float* v) {
		PUB_ASSERT(v);
		std::string s;
		if (!get(section, key, &s))
			return false;

		errno = 0;
		return true;
	}

	bool CINIParser::get(const char* section, const char* key, std::string* v) {
		PUB_ASSERT(v);
		MAP_ITEM& map_item = _mapSection[std::string(section)];
		MAP_ITEM::iterator it = map_item.find(std::string(key));
		if (it == map_item.end())
			return false;

		*v = it->second;
		return true;
	}

	void CINIParser::set(const char *section, const char *key, int32_t v) {
		char temp[32] = { 0 };
		sprintf_safe(temp, "%d", v);
		_mapSection[std::string(section)][std::string(key)] = std::string(temp);
	}

	void CINIParser::set(const char *section, const char *key, float v) {
		char temp[32] = { 0 };
		sprintf_safe(temp, "%f", v);
		_mapSection[std::string(section)][std::string(key)] = std::string(temp);
	}

	void CINIParser::set(const char *section, const char *key, const char *v) {
		_mapSection[std::string(section)][std::string(key)] = std::string(v);
	}

	void CINIParser::savestr(std::string &str)
	{
		str.clear();
		MAP_SECTION::iterator it_sec = _mapSection.begin();
		while (it_sec != _mapSection.end()) {
			str.append("[").append(it_sec->first).append("]\n");
			MAP_ITEM &map_item = it_sec->second;
			MAP_ITEM::iterator it_item = map_item.begin();
			while (it_item != map_item.end()) {
				str.append(it_item->first).append("=").append(it_item->second).append("\n");
				++it_item;
			}
			++it_sec;
		}
	}

	void CINIParser::print() {
		MAP_SECTION::iterator it_sec = _mapSection.begin();
		while (it_sec != _mapSection.end()) {
			PUB_log(LT_INFO, "[%s]", it_sec->first.c_str());
			MAP_ITEM& map_item = it_sec->second;
			MAP_ITEM::iterator it_item = map_item.begin();
			while (it_item != map_item.end()) {
				PUB_log(LT_INFO, "%s = %s", it_item->first.c_str(), it_item->second.c_str());
				++it_item;
			}
			
			++it_sec;
		}
	}
}
