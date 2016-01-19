#include "basis_var.h"
#include "basis_checked_delete.h"

namespace basis 
{

BSVar::BSVar( const BSVar& var )
{	
	if (var.isInterger() || var.isDouble())
	{
		m_holder = VarHolderPtr(var.m_holder->clone());
	}
	else if (var.content())
	{
		m_holder = var.m_holder;
	}
}

BSVar& BSVar::operator=( const BSVar& var )
{
	if (this != &var)
	{
		if (var.isInterger() || var.isDouble())
		{
			m_holder = VarHolderPtr(var.m_holder->clone());
		}
		else if (var.content())
		{
			m_holder = var.m_holder;
		}		
	}
	return *this;
}

bool BSVar::isInterger() const
{
	if (content()) 
	{
		return m_holder->isInterger();
	}
	return false;
}

bool BSVar::isDouble() const
{
	if (content()) 
	{
		return m_holder->isDouble();
	}
	return false;
}

bool BSVar::isString() const
{
	if (content()) 
	{
		return m_holder->isString();
	}
	return false;
}

bool BSVar::isVector() const
{
	if (content()) 
	{
		return m_holder->isVector();
	}
	return false;
}

bool BSVar::isMap() const
{
	if (content()) 
	{
		return m_holder->isMap();
	}
	return false;
}

BSVarType BSVar::type() const
{
	if (content()) 
	{
		return	m_holder->type();
	}
	return BSVarType(BSVarType::kNil);
}

//////////////////////////////////////////////////////////////////////////
bool BSVarHolder::convert( int8 & v )
{
	v = 0;
	return false;
}

bool BSVarHolder::convert( uint8 & v )
{
	v = 0;
	return false;
}

bool BSVarHolder::convert( int16 & v )
{
	v = 0;
	return false;
}

bool BSVarHolder::convert( uint16 & v )
{
	v = 0;
	return false;
}

bool BSVarHolder::convert( int32 & v )
{
	v = 0;
	return false;
}

bool BSVarHolder::convert( uint32 & v )
{
	v = 0;
	return false;
}

bool BSVarHolder::convert( uint64 & v )
{
	v = 0;
	return false;
}

bool BSVarHolder::convert( int64 & v )
{
	v = 0;
	return false;
}

bool BSVarHolder::convert( string & v )
{
	v.clear();
	return false;
}

//////////////////////////////////////////////////////////////////////////

}//namespace basis