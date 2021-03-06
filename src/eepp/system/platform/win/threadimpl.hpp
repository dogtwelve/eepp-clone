#ifndef EE_SYSTEMCTHREADIMPLWIN_HPP
#define EE_SYSTEMCTHREADIMPLWIN_HPP

#include <eepp/config.hpp>

#if EE_PLATFORM == EE_PLATFORM_WIN

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <process.h>

namespace EE { namespace System {
class Thread;

namespace Platform {

class ThreadImpl {
	public:
		static UintPtr GetCurrentThreadId();

		ThreadImpl( Thread * owner );

		~ThreadImpl();
		
		void Wait();
		
		void Terminate();

		UintPtr Id();
	protected:
		static unsigned int __stdcall EntryPoint(void* userData);

		HANDLE			mThread;
		unsigned int	mThreadId;
};

}}}

#endif

#endif
