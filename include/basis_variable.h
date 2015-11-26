#ifndef _BASIS_VARIABLE_H_
#define _BASIS_VARIABLE_H_


#include "basis_define.h"


namespace basis {

class BSVarType
{
public:
	enum kVarType
	{
		kNil, // error
		kInt8,
		kUint8,
		kInt16,
		kUint16,
		kInt32,
		kUint32,
		kInt64,
		kUint64,
		kDouble,
		kString,
		kBlob,
		kVector,
		kList,
		kMap,
		kUserDef, 
	};

public:
	~BSVarType() {}
	BSVarType( kVarType tp ) : m_tp(tp) {}
	BSVarType( const BSVarType& value ) : m_tp(value.m_tp) { }
	BSVarType& operator=( const BSVarType& tp )
	{
		m_tp = tp.m_tp;
		return *this;
	}

	operator kVarType() { return m_tp; }
	operator kVarType() const { return m_tp; }
	bool operator==( const BSVarType& other )
	{
		return m_tp == other.m_tp;
	}

private:
	kVarType m_tp;
};

//////////////////////////////////////////////////////////////////////////
template<class T> class BSVarHolderImpl;

class BSVarHolder
{	
private:
	BSVarHolder& operator=(const BSVarHolder& r);

public:
	virtual ~BSVarHolder() {}

	virtual bool isString() { return false; }
	virtual bool isInterger() { return false; }
	virtual bool isDouble() { return false; }
	virtual bool isVector() { return false; }
	virtual bool isList() { return false; }
	virtual bool isMap() { return false; }

	virtual BSVarType type() { return BSVarType(BSVarType::kNil); }
	virtual BSVarHolder* clone() = 0;

protected:
	template<typename T>
	static BSVarHolder* clone_impl(const BSVarHolderImpl<T>& value);
};

template<typename T>
BSVarHolder* basis::BSVarHolder::clone_impl(const BSVarHolderImpl<T>& value)
{
	BSVarHolder* p = new(nothrow) BSVarHolderImpl<T>(value);
	return p;
}

//////////////////////////////////////////////////////////////////////////
template<typename T>
class BSVarHolderImpl : public BSVarHolder
{
public:
	BSVarHolderImpl(const T& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< T >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kUserDef);
	}

	const T & value() { return m_value; }

private:
	T m_value;	
};


//////////////////////////////////////////////////////////////////////////
template<>
class BSVarHolderImpl<uint8> : public BSVarHolder
{
private:
	uint8 m_value;

public:
	BSVarHolderImpl(const uint8& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< uint8 >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isInterger()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kUint8);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const uint8 & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class BSVarHolderImpl<int8> : public BSVarHolder
{
private:
	int8 m_value;

public:
	BSVarHolderImpl(const int8& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< int8 >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isInterger()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kInt8);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const int8 & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class BSVarHolderImpl<uint16> : public BSVarHolder
{
private:
	uint16 m_value;

public:
	BSVarHolderImpl(const uint16& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< uint16 >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isInterger()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kUint16);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const uint16 & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class BSVarHolderImpl<int16> : public BSVarHolder
{
private:
	int16 m_value;

public:
	BSVarHolderImpl(const int16& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< int16 >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isInterger()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kInt16);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const int16 & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class BSVarHolderImpl<uint32> : public BSVarHolder
{
private:
	uint32 m_value;

public:
	BSVarHolderImpl(const uint32& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< uint32 >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isInterger()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kUint32);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const uint32 & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class BSVarHolderImpl<int32> : public BSVarHolder
{
private:
	int32 m_value;

public:
	BSVarHolderImpl(const int32& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< int32 >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isInterger()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kInt32);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const int32 & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class BSVarHolderImpl<int64> : public BSVarHolder
{
private:
	int64 m_value;

public:
	BSVarHolderImpl(const int64& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< int64 >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isInterger()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kInt64);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const int64 & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class BSVarHolderImpl<uint64> : public BSVarHolder
{
private:
	uint64 m_value;

public:
	BSVarHolderImpl(const uint64& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< uint64 >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isInterger()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kUint64);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const uint64 & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class BSVarHolderImpl<string> : public BSVarHolder
{
private:
	string m_value;

public:
	BSVarHolderImpl(const string& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< string >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isString()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kString);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const string & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<typename T>
class BSVarHolderImpl< vector< T > > : public BSVarHolder
{
private:
	vector<T> m_value;

public:
	BSVarHolderImpl(const vector< T >& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< vector< T > >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isString()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kVector);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const vector< T > & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<typename T>
class BSVarHolderImpl< list< T > > : public BSVarHolder
{
private:
	list<T> m_value;

public:
	BSVarHolderImpl(const list< T >& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< list< T > >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isString()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kList);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const list< T > & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
template<typename K, typename V>
class BSVarHolderImpl< map< K, V > > : public BSVarHolder
{
private:
	map< K, V > m_value;

public:
	BSVarHolderImpl(const map< K, V >& value) : m_value(value) {}
	BSVarHolderImpl(const BSVarHolderImpl< map< K, V > >& r) : m_value(r.m_value) {}
	virtual ~BSVarHolderImpl() {}

public:
	virtual bool isString()
	{
		return true; 
	}

	virtual BSVarType type()
	{
		return BSVarType(BSVarType::kMap);
	}

	virtual BSVarHolder* clone()
	{
		return clone_impl(*this);
	}

	const map< K, V > & value() 
	{
		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////
class BSVar
{
public:
	template <typename T>
	BSVar(const T& value);

	BSVar() : m_holder(NULL) {}
	BSVar(const BSVar&);
	BSVar& operator=(const BSVar&);
	~BSVar();

public:
	bool isString() const ;
	bool isInterger() const ;
	bool isDouble() const ;
	bool isVector() const ;
	bool isList() const ;
	bool isMap() const ;

	BSVarType type() const;

private:
	BSVarHolder* m_holder;
};

template <typename T>
BSVar::BSVar( const T& value )
	: m_holder(new(nothrow) BSVarHolderImpl<T>(value))
{

}


};// namespace basis


#endif // _BASIS_VARIABLE_H_
