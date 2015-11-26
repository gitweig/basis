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

private:
	BSVarHolder* m_holder;
};

template <typename T>
BSVar::BSVar( const T& value )
	: m_holder(new(nothrow) BSVarHolderImpl<T>(value))
{

}

};// namespace basis


#endif // _BASIS_VAR_H_
