#ifndef _BASIS_SMART_PTR_H_
#define _BASIS_SMART_PTR_H_

#include "basis_atomic.h"
#include "basis_checked_delete.h"

namespace basis {
	
namespace detail {

	class weak_count;
	class shared_count;
	
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

	template<class X> 
	class sp_counted_impl_p: public sp_counted_base
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
			return std::allocator<this_type>().allocate(1, static_cast<this_type*>(0));
		}

		void operator delete(void * p)
		{
			std::allocator<this_type>().deallocate(static_cast<this_type *>(p), 1);
		}
	};

	template<class P, class D>
	class sp_counted_impl_pd: public sp_counted_base
	{
	private:
		P* m_ptr; 
		D m_del; 

		sp_counted_impl_pd (sp_counted_impl_pd const &);
		sp_counted_impl_pd & operator= (sp_counted_impl_pd const &);

		typedef sp_counted_impl_pd<P, D> this_type;

	public:
		sp_counted_impl_pd(P* p, D& d): m_ptr(p), m_del(d)
		{
		}

		sp_counted_impl_pd(P* p): m_ptr(p), m_del()
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

	class shared_count
	{
	private:
		sp_counted_base * m_pi;
		friend class weak_count;

	public:
		shared_count(): m_pi(0)
		{
		}

		template<class Y>
		explicit shared_count(Y * p): m_pi(0)
		{
			m_pi = new sp_counted_impl_p<Y>(p);
			if (m_pi == 0) checked_delete(p);
			ASSERT(m_pi);
		}

		template<class Y, class D>
		explicit shared_count(Y * p, D& d): m_pi(0)
		{
			m_pi = new sp_counted_impl_pd<Y, D>(p, d);
			if (m_pi == 0) d(p);
			ASSERT(m_pi);
		}

		explicit shared_count(shared_count const & r) : m_pi(r.m_pi)
		{
			if (m_pi != 0) m_pi->add_ref_copy();
		}

		explicit inline shared_count(weak_count const & r);

		~shared_count()
		{
			if (m_pi) m_pi->release();
			m_pi = 0;
		}

		shared_count & operator= (shared_count const & r)
		{
			sp_counted_base* tmp = r.m_pi;
			if (tmp != m_pi)
			{
				if (tmp != 0) tmp->add_ref_copy();
				if (m_pi != 0) m_pi->release();
				m_pi = tmp;
			}
			return *this;
		}

		void swap(shared_count& r)
		{
			std::swap(m_pi, r.m_pi);
		}

		long use_count() const
		{
			return m_pi != 0 ? m_pi->use_count() : 0;
		}

		bool unique() const
		{
			return use_count() == 1;
		}

		bool empty() const
		{
			return m_pi == 0;
		}

		friend inline bool operator==(shared_count const & a, shared_count const & b)
		{
			return a.m_pi == b.m_pi;
		}

		friend inline bool operator<(shared_count const & a, shared_count const & b)
		{
			return std::less<sp_counted_base *>()( a.m_pi, b.m_pi );
		}
	};

	class weak_count
	{
	private:
		sp_counted_base* m_pi;
		friend class shared_count;

	public:
		weak_count() : m_pi( 0 )
		{
		}

		weak_count(weak_count const & r) : m_pi( r.m_pi )
		{
			if (m_pi) m_pi->weak_add_ref();
		}

		weak_count(shared_count const & r) : m_pi( r.m_pi )
		{
			if (m_pi) m_pi->weak_add_ref();
		}

		~weak_count()
		{
			if (m_pi != 0) m_pi->weak_release();
			m_pi = 0;
		}

		weak_count & operator= (weak_count const & r)
		{
			sp_counted_base* tmp = r.m_pi;
			if (tmp != m_pi)
			{
				if (tmp) tmp->weak_add_ref();
				if (m_pi) m_pi->weak_release();
				m_pi = tmp;
			}
			return *this;
		}

		weak_count & operator= (shared_count const & r)
		{
			sp_counted_base* tmp = r.m_pi;
			if (tmp != m_pi)
			{
				if (tmp) tmp->weak_add_ref();
				if (m_pi) m_pi->weak_release();
				m_pi = tmp;
			}
			return *this;
		}

		void swap(weak_count & r)
		{
			std::swap(m_pi, r.m_pi);
		}

		long use_count() const 
		{
			return m_pi != 0 ? m_pi->use_count() : 0;
		}

		bool empty() const
		{
			return m_pi == 0;
		}

		friend inline bool operator==(weak_count const & a, weak_count const & b)
		{
			return a.m_pi == b.m_pi;
		}

		friend inline bool operator<(weak_count const & a, weak_count const & b)
		{
			return std::less<sp_counted_base *>()(a.m_pi, b.m_pi);
		}
	};

	inline shared_count::shared_count(weak_count const & r) : m_pi(r.m_pi)
	{
		if (m_pi == 0 || !m_pi->add_ref_lock())
		{
			m_pi = NULL;
		}
	}

}; // namespace detail

template<typename T>
class scoped_ptr
{
private:
	T* m_p;

	typedef scoped_ptr<T> this_type;

	scoped_ptr(scoped_ptr const &);
	scoped_ptr & operator=( scoped_ptr const &);

	void operator==( scoped_ptr const& ) const;
	void operator!=( scoped_ptr const& ) const;

public:
	typedef T element_type;

	explicit scoped_ptr(T* p = 0) : m_p( p )
	{
	}

	~scoped_ptr()
	{
		basis::checked_delete(m_p);
	}

	void reset(T * p = 0)
	{
		ASSERT( p == 0 || p != m_p ); // catch self-reset errors
		this_type(m_p).swap(*this);
	}

	T & operator*() const
	{
		ASSERT( m_p != 0 );
		return *m_p;
	}

	T * operator->() const
	{
		ASSERT( m_p != 0 );
		return m_p;
	}

	T * get() const
	{
		return m_p;
	}

	void swap(scoped_ptr & r)
	{
		std::swap(m_p, r.m_p);
	}
};

template<class T> inline void swap(scoped_ptr<T> & a, scoped_ptr<T> & b)
{
	a.swap(b);
}

// get_pointer(p) is a generic way to say p.get()
template<class T>
inline T * get_pointer(scoped_ptr<T> const & p)
{
	return p.get();
}

template<class T> class shared_ptr;
template<class T>
class weak_ptr
{
private:
	typedef weak_ptr<T> this_type;
	typedef T element_type;

	template<class Y> friend class weak_ptr;
	template<class Y> friend class shared_ptr;


	element_type * m_px;        // contained pointer
	detail::weak_count m_pn;	// reference counter

public:
	weak_ptr() : m_px(0), m_pn()
	{
	}

	weak_ptr( weak_ptr const & r ) : m_px( r.m_px ), m_pn( r.m_pn )
	{
	}

	weak_ptr & operator=( weak_ptr const & r )
	{
		this_type(r).swap(*this);
		return *this;
	}

	template<class Y>
	weak_ptr & operator=( weak_ptr<Y> const & r )
	{
		m_px = r.lock().get();
		m_pn = r.m_pn;
		return *this;
	}

	template<class Y>
	weak_ptr & operator=( shared_ptr<Y> const & r )
	{
		m_px = r.m_px;
		m_pn = r.m_pn;
		return *this;
	}

	shared_ptr<T> lock() const
	{
		return shared_ptr<T>( *this );
	}

	long use_count() const
	{
		return m_pn.use_count();
	}

	bool expired() const
	{
		return m_pn.use_count() == 0;
	}

	bool _empty() const // extension, not in std::weak_ptr
	{
		return m_pn.empty();
	}

	void reset()
	{
		this_type().swap(*this);
	}

	void swap(this_type & other)
	{
		std::swap(m_px, other.m_px);
		m_pn.swap(other.m_pn);
	}

	template<class Y>
	bool owner_before( weak_ptr<Y> const & rhs ) const
	{
		return m_pn < rhs.m_pn;
	}

	template<class Y>
	bool owner_before( shared_ptr<Y> const & rhs ) const 
	{
		return m_pn < rhs.m_pn;
	}

};  // weak_ptr

template<class T, class U> 
inline bool operator<(weak_ptr<T> const & a, weak_ptr<U> const & b)
{
	return a.owner_before( b );
}

template<class T> void swap(weak_ptr<T> & a, weak_ptr<T> & b)
{
	a.swap(b);
}

template<class T>
class shared_ptr
{
private:
	typedef T element_type;
	typedef T* element_access;
	typedef shared_ptr<T> this_type;

	template<class Y> friend class shared_ptr;
	template<class Y> friend class weak_ptr;

	element_type * m_px;          // contained pointer
	detail::shared_count m_pn;    // reference counter

public:
	shared_ptr() : m_px( 0 ), m_pn()
	{
	}

	template<class Y>
	explicit shared_ptr( Y * p ): m_px( p ), m_pn( p ) // Y must be complete
	{
	}

	template<class Y, class D> 
	shared_ptr( Y * p, D d ): m_px( p ), m_pn( p, d )
	{
	}

	shared_ptr( shared_ptr const & r ) : m_px( r.m_px ), m_pn( r.m_pn )
	{
	}

	template<class Y>
	explicit shared_ptr( weak_ptr<Y> const & r ): m_pn( r.m_pn ), m_px( r.m_px)
	{
		if (m_pn.empty())
		{
			m_px = NULL;
		}
	}

	// aliasing
	template< class Y >
	shared_ptr( shared_ptr<Y> const & r, element_type * p ) : m_px( p ), m_pn( r.m_pn )
	{
	}

	// assignment
	shared_ptr & operator=( shared_ptr const & r )
	{
		this_type(r).swap(*this);
		return *this;
	}

	template<class Y>
	shared_ptr & operator=(shared_ptr<Y> const & r)
	{
		this_type(r).swap(*this);
		return *this;
	}

	void reset()
	{
		this_type().swap(*this);
	}

	template<class Y> 
	void reset( Y * p ) // Y must be complete
	{
		ASSERT( p == 0 || p != m_px ); // catch self-reset errors
		this_type( p ).swap( *this );
	}

	template<class Y, class D>
	void reset( Y * p, D d )
	{
		this_type( p, d ).swap( *this );
	}

	template<class Y> 
	void reset( shared_ptr<Y> const & r, element_type * p )
	{
		this_type( r, p ).swap( *this );
	}

	element_type & operator* () const
	{
		ASSERT( m_px != 0 );
		return *m_px;
	}

	element_access operator-> () const 
	{
		ASSERT( m_px != 0 );
		return m_px;
	}

	element_type * get() const
	{
		return m_px;
	}

	bool unique() const
	{
		return m_pn.unique();
	}

	long use_count() const
	{
		return m_pn.use_count();
	}

	void swap( shared_ptr & other )
	{
		std::swap(m_px, other.m_px);
		m_pn.swap(other.m_pn);
	}

	template<class Y>
	bool owner_before( shared_ptr<Y> const & rhs ) const
	{
		return m_pn < rhs.m_pn;
	}

	template<class Y> 
	bool owner_before( weak_ptr<Y> const & rhs ) const
	{
		return m_pn < rhs.m_pn;
	}
};  // shared_ptr

template<class T, class U>
inline bool operator==(shared_ptr<T> const & a, shared_ptr<U> const & b)
{
	return a.get() == b.get();
}

template<class T, class U>
inline bool operator!=(shared_ptr<T> const & a, shared_ptr<U> const & b)
{
	return a.get() != b.get();
}

template<class T, class U>
inline bool operator<(shared_ptr<T> const & a, shared_ptr<U> const & b)
{
	return a.owner_before( b );
}

template<class T>
inline void swap(shared_ptr<T> & a, shared_ptr<T> & b)
{
	a.swap(b);
}

template<class T, class U>
shared_ptr<T> static_pointer_cast( shared_ptr<U> const & r )
{
	(void) static_cast< T* >( static_cast< U* >( 0 ) );

	typedef typename shared_ptr<T>::element_type E;

	E * p = static_cast< E* >( r.get() );
	return shared_ptr<T>( r, p );
}

template<class T, class U>
shared_ptr<T> const_pointer_cast( shared_ptr<U> const & r )
{
	(void) const_cast< T* >( static_cast< U* >( 0 ) );

	typedef typename shared_ptr<T>::element_type E;

	E * p = const_cast< E* >( r.get() );
	return shared_ptr<T>( r, p );
}

template<class T, class U>
shared_ptr<T> dynamic_pointer_cast( shared_ptr<U> const & r )
{
	(void) dynamic_cast< T* >( static_cast< U* >( 0 ) );

	typedef typename shared_ptr<T>::element_type E;

	E * p = dynamic_cast< E* >( r.get() );
	return p? shared_ptr<T>( r, p ): shared_ptr<T>();
}

template<class T, class U> 
shared_ptr<T> reinterpret_pointer_cast( shared_ptr<U> const & r )
{
	(void) reinterpret_cast< T* >( static_cast< U* >( 0 ) );

	typedef typename shared_ptr<T>::element_type E;

	E * p = reinterpret_cast< E* >( r.get() );
	return shared_ptr<T>( r, p );
}

} // namespace basis


#endif // _BASIS_SMART_PTR_H_