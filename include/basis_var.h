#ifndef _BASIS_VAR_H_
#define _BASIS_VAR_H_

#include "basis_var_holder.h"
#include "basis_smart_ptr.h"

namespace basis {

	class BSVar
	{
	public:
		template <typename T>
		BSVar(const T& value);

		BSVar() : m_holder() {}
		BSVar(const BSVar&);
		BSVar& operator=(const BSVar&);
	
		~BSVar() {}

	public:
		bool isString() const ;
		bool isInterger() const ;
		bool isDouble() const ;
		bool isVector() const ;
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
		BSVarHolder* content() const { return m_holder.get(); }

	private:
		typedef basis::shared_ptr<BSVarHolder> VarHolderPtr;

		VarHolderPtr m_holder;
	};

	template <typename T>
	BSVar::BSVar( const T& value )
		: m_holder(new(nothrow) BSVarHolderImpl<T>(value))
	{
	}

	template< typename T >
	bool BSVar::extract( T& value ) const
	{
		ASSERT(content());
		return BSVarHolder::extract(content(), value);
	}

	template<typename T>
	T & BSVar::extract_def() const
	{
		ASSERT(content());
		return BSVarHolder::extract_def<T>(content());
	}

	template<typename T>
	BSVar::operator T() const
	{
		return extract_def<T>();
	}

	template<typename T>
	bool BSVar::convert( T & value ) const
	{
		if (content() == NULL) return false;
		return m_holder->convert(value);
	}

};// namespace basis


#endif // _BASIS_VAR_H_
