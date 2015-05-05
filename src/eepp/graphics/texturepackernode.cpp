#include <eepp/graphics/texturepackernode.hpp>

namespace EE { namespace Graphics { namespace Private {

TexturePackerNode::TexturePackerNode( Int32 x, Int32 y, Int32 width, Int32 height ) {
	mX 		= x;
	mY 		= y;
	mWidth 	= width;
	mHeight = height;
	mNext 	= NULL;
}

TexturePackerNode * TexturePackerNode::GetNext() const {
	return mNext;
}

bool TexturePackerNode::Fits( Int32 width, Int32 height, Int32 &edgeCount, const bool& AllowFlipping ) const {
	bool ret = false;

	edgeCount = 0;

	if ( width == mWidth  || height == mHeight || width == mHeight || height == mWidth ) {
		if ( width == mWidth ) {
			edgeCount++;
			if ( height == mHeight ) edgeCount++;
		}
		else if ( AllowFlipping && width == mHeight ) {
			edgeCount++;
			if ( height == mWidth ) edgeCount++;
		}
		else if ( AllowFlipping && height == mWidth ) {
			edgeCount++;
		}
		else if ( height == mHeight ) {
			edgeCount++;
		}
	}

	if ( width <= mWidth && height <= mHeight ) {
		ret = true;
	} else if ( AllowFlipping && height <= mWidth && width <= mHeight ) {
		ret = true;
	}

	return ret;
}

void TexturePackerNode::GetRect( Recti &r ) const {
	r = Recti( mX, mY, mX + mWidth - 1, mY + mHeight - 1 );
}

void TexturePackerNode::Validate( TexturePackerNode * n ) {
	Recti r1;
	Recti r2;
	GetRect( r1 );
	n->GetRect( r2 );
	eeASSERT( !r1.Intersect(r2) );
}

bool TexturePackerNode::Merge( const TexturePackerNode& n ) {
	bool ret = false;

	Recti r1;
	Recti r2;

	GetRect( r1 );
	n.GetRect( r2 );

	r1.Right++;
	r1.Bottom++;
	r2.Right++;
	r2.Bottom++;

	if ( r1.Left == r2.Left && r1.Right == r2.Right && r1.Top == r2.Bottom ) // if we share the top edge then..
	{
		mY 		= n.Y();
		mHeight += n.Height();
		ret 	= true;
	}
	else if ( r1.Left == r2.Left && r1.Right == r2.Right && r1.Bottom == r2.Top ) // if we share the bottom edge
	{
		mHeight += n.Height();
		ret 	= true;
	}
	else if ( r1.Top == r2.Top && r1.Bottom == r2.Top && r1.Left == r2.Right ) // if we share the left edge
	{
		mX 		= n.X();
		mWidth += n.Width();
		ret 	= true;
	}
	else if ( r1.Top == r2.Top && r1.Bottom == r2.Top && r1.Right == r2.Left ) // if we share the left edge
	{
		mWidth	+= n.Width();
		ret 	= true;
	}

	return ret;
}

}}}
