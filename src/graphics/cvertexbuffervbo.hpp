#ifndef EE_GRAPHICSCVERTEXBUFFERVBO_HPP
#define EE_GRAPHICSCVERTEXBUFFERVBO_HPP

#include "cvertexbuffer.hpp"

namespace EE { namespace Graphics {

class cVertexBufferVBO : public cVertexBuffer {
	public:
		cVertexBufferVBO( const Uint32& VertexFlags = VERTEX_FLAGS_DEFAULT, EE_DRAW_MODE DrawType = DM_QUADS, const Int32& ReserveVertexSize = 0, const Int32& ReserveIndexSize = 0, EE_VBO_USAGE_TYPE UsageType = VBO_USAGE_TYPE_STATIC );

		virtual ~cVertexBufferVBO();

		void Bind();

		void Draw();

		bool Compile();
	protected:
		void SetVertexStates();

		bool mCompiled;

		Uint32 mElementHandle;

		Uint32 mArrayHandle[ VERTEX_FLAGS_COUNT ];
};

}}

#endif