#include "basis_variable.h"
#include "basis_checked_delete.h"

namespace basis 
{

BSVar::BSVar( const BSVar& var )
	: m_holder(var.m_holder->clone())
{
}

BSVar& BSVar::operator=( const BSVar& var )
{
	if (this != &var)
	{
		// 同类型的优化 ?????
		checked_delete(m_holder);
		if (var.m_holder)
		{
			m_holder = var.m_holder->clone();
		}
		else
		{
			m_holder = NULL;
		}	
	}
	return *this;
}

BSVar::~BSVar()
{
	checked_delete(m_holder);
}

bool BSVar::isInterger() const
{
	if (m_holder) 
	{
		return m_holder->isInterger();
	}
	return false;
}

bool BSVar::isDouble() const
{
	if (m_holder) 
	{
		return m_holder->isDouble();
	}
	return false;
}

bool BSVar::isString() const
{
	if (m_holder) 
	{
		return m_holder->isString();
	}
	return false;
}

bool BSVar::isVector() const
{
	if (m_holder) 
	{
		return m_holder->isVector();
	}
	return false;
}

bool BSVar::isList() const
{
	if (m_holder) 
	{
		return m_holder->isList();
	}
	return false;
}

bool BSVar::isMap() const
{
	if (m_holder) 
	{
		return m_holder->isMap();
	}
	return false;
}

BSVarType BSVar::type() const
{
	if (m_holder) 
	{
		return	m_holder->type();
	}
	return BSVarType(BSVarType::kNil);
}

}//namespace basis