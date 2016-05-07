#ifndef __H_PUB_SAFE_MEM_OP__
#define __H_PUB_SAFE_MEM_OP__

#ifdef __cplusplus

#if !defined(_CRT_SECURE_NO_WARNINGS)
#	define _CRT_SECURE_NO_WARNINGS
#endif

namespace pub {
	inline char* strcpy(char* dst, size_t size, const char* src) {
		if (!dst || !size || !src)
			return NULL;

		char* p = dst;
		size_t count = size;

		while ((*dst++ = *src++) != 0 && --count > 0) {
		}

		if (count == 0)
			*(dst - 1) = 0;

		return p;
	}

	inline size_t strlen(const char *str, size_t maxsize) {
		size_t n;
		for (n = 0; n < maxsize && *str; n++, str++)
			;

		return n;
	}

	template < bool > struct StaticAssert { private: StaticAssert() {} };
	template <> struct StaticAssert< true > { StaticAssert() {} };

	struct InvalidLengthException : public std::exception { 
		explicit InvalidLengthException() {} 
	}; 
} // namespace pub

/// strcpy
template < size_t _Len > 
void strcpy_safe(char (&dst)[_Len], const char *src) {
	pub::strcpy(dst, _Len, src);
}

inline void strcpy_unsafe( char *dst, size_t _Len, const char *src ) {
	if (_Len > 0x10000000) {
		throw pub::InvalidLengthException();
	}
	pub::strcpy(dst, _Len, src);
}

/// strlen
template < size_t _Len > 
size_t strlen_safe(char (&str)[_Len]) {
	return (size_t)pub::strlen(str, _Len);
}

inline size_t strlen_safe(const char *str, int _Len) {
	return (size_t)pub::strlen(str, _Len);
}

inline size_t strlen_unsafe(const char *str) {
	return strlen( str );
}

/// sprintf
template < typename _Type > 
void sprintf_safe(_Type buffer, const char *format, ...) {
#if defined(PLATFORM_WINDOWS)
	pub::StaticAssert< false >();
#endif
}

template < size_t _Len > 
int sprintf_safe(char (&buffer)[_Len], const char *format, ...) {
	va_list argptr;
	va_start(argptr, format);
	int ret = vsnprintf(buffer, _Len, format, argptr);
	if ( ret == -1 || ret == (int)_Len ) {
		buffer[ _Len - 1 ] = 0;
		ret = _Len - 1;
	}
	return ret;
}

inline int sprintf_unsafe(char *buffer, size_t _Len, const char *format, ...) {
	if (_Len > 0x10000000) {
		throw pub::InvalidLengthException();
	}
	va_list argptr;
	va_start(argptr, format);
	int ret = vsnprintf(buffer, _Len, format, argptr);
	if (ret == -1 || ret == (int)_Len) {
		buffer[ _Len - 1 ] = 0;
		ret = ( int )( _Len - 1 );
	}
	return ret;
}

/// memcpy
template < typename _Type, typename _Size > 
void memcpy_safe(_Type dst, const void *src, _Size size) {
#if defined(PLATFORM_WINDOWS)
	pub::StaticAssert< false >();
#endif
}

template < typename _Type, size_t _Len > 
void memcpy_safe(_Type (&dst)[_Len], const void *src, size_t size) {
	if ( sizeof(_Type[_Len] ) < size) {
		throw pub::InvalidLengthException();
	}
	memcpy(dst, src, size);
}

inline void memcpy_unsafe( void *dst, size_t _Len, const void *src, size_t size ) {
	if ( _Len > 0x10000000 ) {
		throw pub::InvalidLengthException();
	}
	if ( _Len < size ) {
		throw pub::InvalidLengthException();
	}
	memcpy( dst, src, size );
}

template < size_t _Len > 
void memcpy0_safe( char (&dst)[_Len], const char *src, size_t size ) {
	if ( _Len < size + 1 ) {
		throw pub::InvalidLengthException();
	}
	memcpy( dst, src, size );
	dst[size] = 0;
}

inline void memcpy0_unsafe( char *dst, size_t _Len, const char *src, size_t size ) {
	if ( _Len > 0x10000000 ) {
		throw pub::InvalidLengthException();
	}
	if ( _Len < size + 1 ) {
		throw pub::InvalidLengthException();
	}
	memcpy( dst, src, size );
	dst[size] = 0;
}

//#define vsprintf_safe vsnprintf
//
//#define strcpy __choose_secure_version__
//#define strncpy __choose_secure_version__
//#define sprintf __choose_secure_version__
//#define vsprintf __choose_secure_version__
//#define memmove __choose_secure_version__
//#define memcpy __choose_secure_version__
//#define strlen __choose_secure_version__

#undef _CRT_SECURE_NO_WARNINGS
#endif // __cplusplus

#endif // __H_PUB_SAFE_MEM_OP__
