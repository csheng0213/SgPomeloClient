#ifndef _PUB_INI_H_
#define _PUB_INI_H_

#ifdef __cplusplus

namespace pub {
	class CINIParser {
	public:
		PUB_API CINIParser();
		PUB_API ~CINIParser();

		PUB_API bool parse(const char* file_name);
		PUB_API bool parsemem(const void *mem, size_t len);
		PUB_API bool parsestr(const char *str);

		PUB_API bool get(const char* section, const char* key, int32_t* v);
		PUB_API bool get(const char* section, const char* key, float* v);
		PUB_API bool get(const char* section, const char* key, std::string* v);

		PUB_API void set(const char *section, const char *key, int32_t v);
		PUB_API void set(const char *section, const char *key, float v);
		PUB_API void set(const char *section, const char *key, const char *v);

		PUB_API void savestr(std::string &str);

		void print();

	private:
		bool parse_line(std::string& line, std::string& section);

	private:
		typedef std::map<std::string, std::string> MAP_ITEM;
		typedef std::map<std::string, MAP_ITEM> MAP_SECTION;
		MAP_SECTION _mapSection;
	};
}

#endif // __cplusplus

#endif /* _PUB_INI_H_ */
