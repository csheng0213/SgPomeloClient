#ifndef __SLIM_XML_H__
#define __SLIM_XML_H__

#undef UNICODE

#include "pub.h"

#include <string>
#include <list>
#include <istream>

namespace xml
{

enum Encode
{
	MULTI_BYTE = 0,
	UTF_8,
	UTF_8_NO_MARK,
	UTF_16,
	UTF_16_BIG_ENDIAN,

#ifdef UNICODE
	DefaultEncode = UTF_8
#else
	DefaultEncode = MULTI_BYTE
#endif
};

#ifdef UNICODE
	typedef wchar_t Char;
	#define T(str) L##str
	#define StrToI _wtoi
	#define StrToF _wtof
	//#define Sprintf swprintf_s
	#define Sprintf swprintf
	#define Strlen wcslen
	#define Strcmp wcscmp
	#define Strncmp wcsncmp
	#define Memchr wmemchr
#else
	typedef char Char;
	#define T(str) str
	#define StrToI atoi
	#define StrToF atof
	//#define Sprintf sprintf_s
	#define Sprintf sprintf_safe
	#define Strlen strlen
	#define Strcmp strcmp
	#define Strncmp strncmp
	#define Memchr memchr
#endif

typedef std::basic_string<Char> String;
class XmlAttribute;
class XmlNode;
typedef std::list<XmlAttribute*>::const_iterator AttributeIterator;
typedef std::list<XmlNode*>::const_iterator NodeIterator;

enum NodeType
{
	DOCUMENT = 0,
	ELEMENT,
	COMMENT,
	DECLARATION
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class XmlBase
{
public:
	const Char* getName() const;
	void setName( const Char* name );
	void assignName( const Char* name, size_t length );

	const Char*	getString() const;
	bool		getBool() const;
	int			getInt() const;
	float		getFloat() const;
	double		getDouble() const;

	void setString( const Char* value );
	void setString( const String& value );
	void assignString( const Char* value, size_t length );
	void setBool( bool value );
	void setInt( int value );
	void setFloat( float value );
	void setDouble( double value );

protected:
	String	m_name;
	String	m_value;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class XmlAttribute : public XmlBase
{
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class XmlNode : public XmlBase
{
public:
	XmlNode( NodeType type, XmlNode* parent );
	~XmlNode();

public:
	NodeType getType() const;

	bool isEmpty() const;

	XmlNode* getParent() const;

	bool hasChild() const;

	size_t getChildrenCount() const;
	XmlNode* getFirstChild( NodeIterator& iter ) const;
	XmlNode* getNextChild( NodeIterator& iter ) const;
	XmlNode* getChild( NodeIterator iter ) const;

	XmlNode* findChild( const Char* name ) const;
	XmlNode* findFirstChild( const Char* name, NodeIterator& iter ) const;
	XmlNode* findNextChild( const Char* name, NodeIterator& iter ) const;

	void clearChild();

	XmlNode* addChild( const Char* name = NULL, NodeType = ELEMENT );


	bool hasAttribute() const;

	XmlAttribute* findAttribute( const Char* name ) const;

	XmlAttribute* getFirstAttribute( AttributeIterator& iter ) const;
	XmlAttribute* getNextAttribute( AttributeIterator& iter ) const;

	void clearAttribute();

	XmlAttribute* addAttribute( const Char* name = NULL, const Char* value = NULL );

protected:
	void writeNode( String& output, int depth ) const;

	void writeChildNodes( String& output, int depth ) const;

private:
	NodeType					m_type;
	std::list<XmlAttribute*>	m_attributes;
	XmlNode*					m_parent;
	std::list<XmlNode*>			m_children;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class XmlDocument : public XmlNode
{
public:
	XmlDocument();

	bool loadFromFile( const Char* filename );
	bool loadFromStream( std::istream& input );
	bool loadFromMemory( const char* buffer, size_t size );

	bool save( const Char* filename, Encode encode = DefaultEncode ) const;

private:
	bool parse( const Char* input, size_t size );

	bool findLabel( const Char* &begin, size_t size, const Char* &label, size_t &labelSize );

	void parseLabel( XmlNode* node, const Char* label, size_t labelSize );
};

///////////////////////////////////////////////////////////////////////////////////////////////////
inline const Char* XmlBase::getName() const
{
	return m_name.c_str();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline void XmlBase::setName( const Char* name )
{
	m_name = name;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline void XmlBase::assignName( const Char* name, size_t length )
{
	m_name.assign( name, length );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline const Char* XmlBase::getString() const
{
	return m_value.c_str();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline bool XmlBase::getBool() const
{
	return ( m_value == T("true") || m_value == T("TRUE") );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline int XmlBase::getInt() const
{
	return StrToI( m_value.c_str() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline float XmlBase::getFloat() const
{
	return static_cast<float>( StrToF( m_value.c_str() ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline double XmlBase::getDouble() const
{
	return StrToF( m_value.c_str() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline void XmlBase::setString( const Char* value )
{
	m_value = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline void XmlBase::setString( const String& value )
{
	m_value = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline void XmlBase::assignString( const Char* value, size_t length )
{
	m_value.assign( value, length );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline void XmlBase::setBool( bool value )
{
	m_value = value ? T("true") : T("false");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline void XmlBase::setInt( int value )
{
	Char sz[128];
	Memchr(sz, 0, 128);
	//Sprintf( sz, 128, T("%d"), value );
	Sprintf( sz, T("%d"), value );
	m_value = sz;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline void XmlBase::setFloat( float value )
{
	Char sz[128];
	Memchr(sz, 0, 128);
	//Sprintf( sz, 128, T("%g"), value );
	Sprintf( sz, T("%g"), value );
	m_value = sz;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline void XmlBase::setDouble( double value )
{
	Char sz[128];
	Memchr(sz, 0, 128);
	//Sprintf( sz, 128, T("%g"), value );
	Sprintf( sz, T("%g"), value );
	m_value = sz;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline NodeType XmlNode::getType() const
{
	return m_type;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline bool XmlNode::isEmpty() const
{
	return ( !hasChild() && m_value.empty() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline bool XmlNode::hasChild() const
{
	return !m_children.empty();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline XmlNode* XmlNode::getParent() const
{
	return m_parent;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline size_t XmlNode::getChildrenCount() const
{
	return m_children.size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline XmlNode* XmlNode::getFirstChild( NodeIterator& iter ) const
{
	iter = m_children.begin();
	if ( iter != m_children.end() )
	{
		return *iter;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline XmlNode* XmlNode::getNextChild( NodeIterator& iter ) const
{
	if ( iter != m_children.end() )
	{
		++iter;
		if ( iter != m_children.end() )
		{
			return *iter;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline XmlNode* XmlNode::getChild( NodeIterator iter ) const
{
	if ( iter != m_children.end() )
	{
		return *iter;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline bool XmlNode::hasAttribute() const
{
	return !m_attributes.empty();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline XmlAttribute* XmlNode::getFirstAttribute( AttributeIterator& iter ) const
{
	iter = m_attributes.begin();
	if ( iter != m_attributes.end() )
	{
		return *iter;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline XmlAttribute* XmlNode::getNextAttribute( AttributeIterator& iter ) const
{
	if ( iter != m_attributes.end() )
	{
		++iter;
		if ( iter != m_attributes.end() )
		{
			return *iter;
		}
	}
	return NULL;
}

size_t utf8toutf16( const char* u8, size_t size, wchar_t* u16, size_t outBufferSize );
size_t utf16toutf8( const wchar_t* u16, size_t size, char* u8, size_t outBufferSize );
bool detectUtf8( const char* str, size_t size );

}

#define UNICODE

#endif
