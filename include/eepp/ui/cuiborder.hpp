#ifndef EE_UICUIBORDER_HPP
#define EE_UICUIBORDER_HPP

#include <eepp/ui/base.hpp>

namespace EE { namespace UI {

class EE_API cUIBorder {
	public:
		cUIBorder();
		cUIBorder( const cUIBorder& border );

		const ColorA& Color() const;
		void Color( const ColorA& Col );

		const unsigned int& Width() const;
		void Width( const unsigned int& width );
	protected:
		ColorA		mColor;
		unsigned int			mWidth;
};

}}

#endif
