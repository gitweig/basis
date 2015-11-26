#ifndef _BASIS_VAR_H_
#define _BASIS_VAR_H_

#include "basis_var_holder.h"

namespace basis {

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

public:
	template< typename T >
	bool convert(T& value) const;

	template< typename T >
	operator T () const;

	template< typename T >
	T & extract_def() const;

	template< typename T >
	bool extract(T& value) const;

private:
	BSVarHolder* m_holder;
};

template <typename T>
BSVar::BSVar( const T& value )
	: m_holder(new(nothrow) BSVarHolderImpl<T>(value))
{
}

template< typename T >
bool basis::BSVar::extract( T& value ) const
{
	ASSERT(m_holder);
	return BSVarHolder::extract(m_holder, value);
}

template<typename T>
T & basis::BSVar::extract_def() const
{
	ASSERT(m_holder);
	return BSVarHolder::extract_def<T>(m_holder);
}

template<typename T>
basis::BSVar::operator T() const
{
	return extract_def<T>();
}

template<typename T>
bool basis::BSVar::convert( T & value ) const
{
	if (m_holder == NULL) return false;
	return m_holder->convert(value);
}

};// namespace basis


#endif // _BASIS_VAR_H_
