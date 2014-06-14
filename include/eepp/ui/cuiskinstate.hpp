#ifndef EE_UICUISKINSTATE_HPP
#define EE_UICUISKINSTATE_HPP

#include <eepp/ui/base.hpp>

namespace EE { namespace UI {

class cUISkin;

class EE_API cUISkinState {
	public:
		enum UISkinStates {
			StateNormal = 0,
			StateFocus,
			StateSelected,
			StateMouseEnter,
			StateMouseExit,
			StateMouseDown,
			StateCount
		};

		cUISkinState( cUISkin * Skin );

		~cUISkinState();

		const Uint32& GetState() const;

		void SetState( const Uint32& State );

		cUISkin * GetSkin() const;

		void Draw( const Float& X, const Float& Y, const Float& Width, const Float& Height, const Uint32& Alpha );

		bool StateExists( const Uint32& State );

		const Uint32& GetPrevState() const;
	protected:
		friend class cUIControl;

		cUISkin * 	mSkin;
		Uint32 		mCurState;
		Uint32		mLastState;

		void StateBack( const Uint32& State );

		void SetPrevState();

		void SetStateTypeSimple( const Uint32& State );

		void SetStateTypeDefault( const Uint32& State );
};

}}

#endif

