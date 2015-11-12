#ifndef _BASIS_SMART_PTR_H_
#define _BASIS_SMART_PTR_H_

#include "basis_atomic.h"

namespace basis {

void checked_delete(void* p)
{

}

class sp_counted_base
{
private:
	BSAtomic32 m_use_count;		// #shared
	BSAtomic32 m_weak_count;	// #weak + (#shared != 0)	

	sp_counted_base(sp_counted_base const & );
	sp_counted_base & operator = (sp_counted_base const & );

protected:
	virtual ~sp_counted_base() {}

public:

	sp_counted_base(): m_use_count(1), m_weak_count(1)
	{

	}

	// dispose() is called when m_use_count drops to zero, to release
	// the resources managed by *this.
	virtual void dispose() = 0;

	// destroy() is called when m_weak_count drops to zero.
	virtual void destroy()
	{
		delete this;
	}

	void add_ref_copy()
	{
		++m_use_count;
	}

	bool add_ref_lock() // true on success
	{
		return m_use_count.test_zero_inc() != 0;
	}

	void release()
	{
		if (--m_use_count == 0)
		{
			dispose();
			weak_release();
		}
	}

	void weak_add_ref()
	{
		++m_weak_count;
	}

	void weak_release()
	{
		if (--m_weak_count == 0 )
		{
			destroy();
		}
	}

	uint32 use_count()
	{
		return (uint32)m_use_count.get_value();
	}
};

template<class X> class sp_counted_impl_p: public sp_counted_base
{
private:

	X * m_px;

	sp_counted_impl_p( sp_counted_impl_p const & );
	sp_counted_impl_p & operator= ( sp_counted_impl_p const & );

	typedef sp_counted_impl_p<X> this_type;

public:
	explicit sp_counted_impl_p(X * px): m_px(px)
	{
	}

	virtual void dispose()
	{
		checked_delete(m_px);
	}
	
	void * operator new(std::size_t)
	{
		return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
	}

	void operator delete(void * p)
	{
		std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
	}
};

template<class P, class D>
class sp_counted_impl_pd: public sp_counted_base
{
private:
	P m_ptr; 
	D m_del; 

	sp_counted_impl_pd (sp_counted_impl_pd const &);
	sp_counted_impl_pd & operator= (sp_counted_impl_pd const &);

	typedef sp_counted_impl_pd<P, D> this_type;

public:
	sp_counted_impl_pd(P p, D& d): m_ptr(p), m_del(d)
	{
	}

	sp_counted_impl_pd(P p): ptr(p), del()
	{
	}

	virtual void dispose()
	{
		m_del(m_ptr);
	}

	void * operator new(std::size_t)
	{
		return std::allocator<this_type>().allocate(1, static_cast<this_type *>(0));
	}

	void operator delete(void * p)
	{
		std::allocator<this_type>().deallocate(static_cast<this_type *>(p), 1);
	}
};

template<class P, class D, class A> 
class sp_counted_impl_pda: public sp_counted_base
{
private:
	P m_p;
	D m_d;
	A m_a;

	sp_counted_impl_pda(sp_counted_impl_pda const &);
	sp_counted_impl_pda & operator= (sp_counted_impl_pda const &);

	typedef sp_counted_impl_pda<P, D, A> this_type;

public:
	sp_counted_impl_pda(P p, D& d, A a): m_p(p), m_d(d), m_a(a)
	{
	}

	sp_counted_impl_pda(P p, A a): m_p(p), m_d(a), m_a(a)
	{
	}

	virtual void dispose()
	{
		m_d(m_p);
	}

	virtual void destroy()
	{
		typedef typename std::allocator_traits<A>::template rebind_alloc< this_type > A2;
		A2 a2(m_a);
		std::allocator_traits<A2>::destroy(a2, this);
		a2.deallocate(this, 1);
	}
};







} // namespace basis


#endif // _BASIS_SMART_PTR_H_