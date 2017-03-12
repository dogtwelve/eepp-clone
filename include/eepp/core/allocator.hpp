#ifndef EE_ALLOCATOR_HPP
#define EE_ALLOCATOR_HPP

#include <eepp/core/memorymanager.hpp>
#include <cstddef>

namespace EE {

template<typename T>
class Allocator {
	public:
		typedef T				value_type;
		typedef T *				pointer;
		typedef const T *		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef ptrdiff_t		difference_type;
		typedef size_t			size_type;

		Allocator() {
		}

		Allocator( const Allocator& ) {
		}

		virtual ~Allocator() {
		}

		T * allocate( size_t cnt, typename std::allocator<void>::const_pointer ptr = 0 ) {
			(void)ptr;
			return ( T * ) eeMalloc( cnt * sizeof( T ) );
		}

		void deallocate( T * ptr, size_type ) {
			eeFree( ptr );
		}

		void construct( T * ptr, const T&e ) {
			eeNewInPlace( ( (void*)ptr ), T, ( e ) );
		}

		void destroy( T * ptr ) {
			#ifdef EE_MEMORY_MANAGER
			EE::MemoryManager::RemovePointer( ptr );
			#endif

			ptr->~T();
		}

		size_t max_size() const {
			return size_t( 0xFFFFFFFF );
		}

		pointer address( reference x ) const {
			return &x;
		}

		const_pointer address( const_reference x ) const {
			return &x;
		}

		Allocator<T>&  operator=(const Allocator&) { return *this; }

		template <class U>
		struct rebind {
			typedef Allocator<U> other;
		};

		template <class U>
		Allocator( const Allocator<U>& ) {}

		template <class U>
		Allocator& operator=(const Allocator<U>&) { return *this; }
	protected:

};

}

#endif
