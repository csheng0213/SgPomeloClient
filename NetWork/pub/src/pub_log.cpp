
#include "pub.h"

static const int MAX_BUFFER_SIZE = 4096;
static char log_buffer[MAX_BUFFER_SIZE];
static char time_buffer[MAX_BUFFER_SIZE];

static bool_t log_console = True;
static std::string log_dir = "./";
static std::string log_info_file_name= "pub_info.log";
static std::string log_err_file_name = "pub_error.log";

static inline const char* get_time_string(struct tm &t) {
	sprintf_safe(time_buffer, "[%02d:%02d:%02d] ", t.tm_hour, t.tm_min, t.tm_sec);
	return time_buffer;
}

static inline const char* get_data_string(struct tm &t) {
	sprintf_safe(time_buffer, "[%d-%02d-%02d] ", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
	return time_buffer;
}

static bool_t log_to_file(const char *file_name, const char* txt) {
	if (!file_name)
		return False;

	time_t _t = time(NULL);
	struct tm* t = localtime(&_t);
	std::string date(get_data_string(*t));
	std::string fullPath = PUB_cat_path(log_dir, date + file_name);
	std::ofstream of(fullPath.c_str(), std::ios::app);
	if (!of.is_open())
		return False;

	of << get_time_string(*t) << txt << std::endl;
	return True;
}

static inline const char* get_log_type_string(enum LOG_TYPE type) {
	switch (type) {
		case LT_INFO:
			return "INFO";
		case LT_ERROR:
			return "ERROR";
		default:
			return "UNKNOWN";
	}
}

static inline const char* get_log_type_file_name(enum LOG_TYPE type) {
	switch (type) {
		case LT_INFO:
			return log_info_file_name.c_str();
		case LT_ERROR:
			return log_err_file_name.c_str();
		default:
			return NULL;
	}
}

bool_t PUB_log_file(const char *file_name, const char *format, ...) {
	va_list arg;
	va_start(arg, format);
	vsnprintf(log_buffer, MAX_BUFFER_SIZE, format, arg);
	va_end(arg);
	return log_to_file(file_name, log_buffer);
}

bool_t PUB_log(enum LOG_TYPE type, const char *format, ...) {
	va_list arg;
	va_start(arg, format);
	vsnprintf(log_buffer, MAX_BUFFER_SIZE, format, arg);
	va_end(arg);

	if (log_console)
		std::cout << "[" << get_log_type_string(type) << "] " << log_buffer << std::endl;
	else
		return log_to_file(get_log_type_file_name(type), log_buffer);
	return True;
}

void PUB_log_set_dir(const char *dir) {
	log_dir = dir;
}

void PUB_log_console_switch(bool_t enable) {
	log_console = enable;
}

void PUB_log_set_file_name(enum LOG_TYPE type, const char *file_name) {
	switch (type) {
		case LT_INFO:
			log_info_file_name = file_name;
			break;
		case LT_ERROR:
			log_err_file_name = file_name;
			break;
		default:
			break;;
	}
}

