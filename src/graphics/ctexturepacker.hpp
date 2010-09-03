#ifndef EE_GRAPHICSCTEXTUREATLAS
#define EE_GRAPHICSCTEXTUREATLAS

/*!
**
** Copyright (c) 2009 by John W. Ratcliff mailto:jratcliffscarab@gmail.com
**
** The MIT license:
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is furnished
** to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.

** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
** WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/*! NOTE by Martin Lucas Golini
**	This implementation is based on the John W. Ratcliff texture atlas implementation.
** Implementation differ from the original, but i use the base texture atlas algorithm.
*/

#include "base.hpp"
#include "packerhelper.hpp"
#include "ctexturepackernode.hpp"
#include "ctexturepackertex.hpp"

namespace EE { namespace Graphics {

using namespace Private;

class EE_API cTexturePacker {
	public:
		cTexturePacker();

		cTexturePacker( const Uint32& MaxWidth, const Uint32& MaxHeight, const bool& ForcePowOfTwo = true, const Uint32& PixelBorder = 0, const bool& AllowFlipping = false );

		~cTexturePacker();

		bool AddTexture( const std::string& TexturePath );

		bool AddTexturesPath( std::string TexturesPath );

		Int32 PackTextures();

		void Save( const std::string& Filepath, const EE_SAVE_TYPE& Format = EE_SAVE_TYPE_DDS, const bool& SaveExtensions = false );

		void Close();

		/** First of all you need to set at least the max dimensions of the texture atlas. */
		void SetOptions( const Uint32& MaxWidth, const Uint32& MaxHeight, const bool& ForcePowOfTwo = true, const Uint32& PixelBorder = 0, const bool& AllowFlipping = false );

		inline const Int32& Width() const { return mWidth; }

		inline const Int32& Height() const { return mHeight; }
	protected:
		enum PackStrategy {
			PackBig,
			PackTiny,
			PackFail
		};

		std::list<cTexturePackerTex>	mTextures;

    	Int32 							mLongestEdge;
    	Int32							mTotalArea;
    	cTexturePackerNode * 			mFreeList;
    	Int32							mWidth;
    	Int32							mHeight;
		bool							mPacked;
		bool							mAllowFlipping;
		cTexturePacker * 				mChild;
		Int32							mStrategy;
		Int32							mCount;
		cTexturePacker * 				mParent;
		std::string						mFilepath;
		Int32							mPlacedCount;
		bool							mForcePowOfTwo;
		Int32							mPixelBorder;
		bool							mSaveExtensions;
		EE_SAVE_TYPE						mFormat;

		cTexturePacker * 				GetChild() const;

		cTexturePacker * 				GetParent() const;

		std::list<cTexturePackerTex> *	GetTexturePackPtr();

		const std::string&				GetFilepath() const;

		void							ChildSave( const EE_SAVE_TYPE& Format );

		void							SaveShapes();

    	void 							NewFree( Int32 x, Int32 y, Int32 width, Int32 height );

    	bool 							MergeNodes();

    	void 							Validate();

    	cTexturePackerTex * 			GetLonguestEdge();

    	cTexturePackerTex * 			GetShortestEdge();

    	Int32							GetChildCount();

    	const Int32&					GetWidth() const;

    	const Int32&					GetHeight() const;

    	const Int32&					GetPlacedCount() const;

    	sTextureHdr						CreateTextureHdr( cTexturePacker * Packer );

    	void							CreateShapesHdr( cTexturePacker * Packer, std::vector<sShapeHdr>& Shapes, std::fstream * fs );

    	cTexturePackerNode *			GetBestFit( cTexturePackerTex * t, cTexturePackerNode ** prevBestFit, Int32 * EdgeCount );

    	void							InsertTexture( cTexturePackerTex * t, cTexturePackerNode * bestFit, Int32 edgeCount, cTexturePackerNode * previousBestFit );

    	void							AddBorderToTextures( const Int32& BorderSize );

    	void							CreateChild();
};

}}

#endif
