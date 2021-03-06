#ifndef EE_GRAPHICSCBATCHRENDERER_H
#define EE_GRAPHICSCBATCHRENDERER_H

#include <eepp/graphics/base.hpp>
#include <eepp/math/polygon2.hpp>
#include <eepp/math/originpoint.hpp>

namespace EE { namespace Graphics {

struct eeTexCoord {
	Float u;
	Float v;
};

struct eeVertex {
	Vector2f pos;
	eeTexCoord tex;
	ColorA color;
};

class TextureFactory;
class Texture;

/** @brief A batch rendering class. */
class EE_API BatchRenderer {
	public:
		BatchRenderer();

		virtual ~BatchRenderer();

		/** Construct with a defined number of vertexs preallocated */
		BatchRenderer( const unsigned int& Prealloc );

		/** Allocate space for vertexs */
		void AllocVertexs( const unsigned int& size );

		/** Set the current texture to render on the batch ( if you change the texture and you have batched something, this will be renderer immediately ) */
		void SetTexture( const Texture * Tex );

		/** Set the predefined blending function to use on the batch */
		void SetBlendMode( const EE_BLEND_MODE& Blend );

		/** Set if every batch call have to be immediately rendered */
		void BatchForceRendering( const bool& force ) { mForceRendering = force; }

		/** Get if the rendering is force on every batch call */
		bool BatchForceRendering() const { return mForceRendering; }

		/** Force the batch rendering */
		void Draw();

		/** Force the batch rendering only if BatchForceRendering is enable */
		void DrawOpt();

		/** Set the rotation of the rendered vertex. */
		void BatchRotation( const Float& Rotation ) { mRotation = Rotation; }

		/** Get the rotation of the rendered vertex. */
		Float BatchRotation() const { return mRotation; }

		/** Set the scale of the rendered vertex. */
		void BatchScale( const Vector2f& Scale ) { mScale = Scale; }

		/** Set the scale of the rendered vertex. */
		void BatchScale( const Float& Scale ) { mScale = Vector2f( Scale, Scale ); }

		/** Get the scale of the rendered vertex. */
		Vector2f BatchScale() const { return mScale; }

		/** The batch position */
		void BatchPosition( const Vector2f Pos ) { mPosition = Pos; }

		/** @return The batch position */
		Vector2f BatchPosition() const { return mPosition; }

		/** This will set a center position for rotating and scaling the batched vertex. */
		void BatchCenter( const Vector2f Pos ) { mCenter = Pos; }

		/** @return The batch center position */
		Vector2f BatchCenter() const { return mCenter; }

		/** Add to the batch a quad ( this will change your batch rendering method to DM_QUADS, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchQuadEx( Float x, Float y, Float width, Float height, Float angle = 0.0f, Vector2f scale = Vector2f::One, OriginPoint originPoint = OriginPoint(OriginPoint::OriginCenter) );

		/** Add to the batch a quad ( this will change your batch rendering method to DM_QUADS, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchQuad( const Float& x, const Float& y, const Float& width, const Float& height, const Float& angle = 0.0f );

		/** Add to the batch a quad with the vertex freely seted ( this will change your batch rendering method to DM_QUADS, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchQuadFree( const Float& x0, const Float& y0, const Float& x1, const Float& y1, const Float& x2, const Float& y2, const Float& x3, const Float& y3 );

		/** Add to the batch a quad with the vertex freely seted ( this will change your batch rendering method to DM_QUADS, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchQuadFreeEx( const Float& x0, const Float& y0, const Float& x1, const Float& y1, const Float& x2, const Float& y2, const Float& x3, const Float& y3, const Float& Angle = 0.0f, const Float& Scale = 1.0f );

		/** This will set as the default batch rendering to GL_QUADS. WIll reset the texture subset rendering to the whole texture. Will reset the default color rendering to ColorA(255,255,255,255). */
		void QuadsBegin();

		/** Set the texture sector to be rendered */
		void QuadsSetSubset( const Float& tl_u, const Float& tl_v, const Float& br_u, const Float& br_v );

		/** Set the texture sector to be rendered but freely seted */
		void QuadsSetSubsetFree( const Float& x0, const Float& y0, const Float& x1, const Float& y1, const Float& x2, const Float& y2, const Float& x3, const Float& y3 );

		/** Set the quad color */
		void QuadsSetColor( const ColorA& Color );

		/** Set the quad color per vertex */
		void QuadsSetColorFree( const ColorA& Color0, const ColorA& Color1, const ColorA& Color2, const ColorA& Color3 );

		/** This will set as the default batch rendering to DM_POINTS. And will reset the point color to ColorA(255,255,255,255). */
		void PointsBegin();

		/** Set the point color */
		void PointSetColor( const ColorA& Color );

		/** Add to the batch a point ( this will change your batch rendering method to DM_POINTS, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchPoint( const Float& x, const Float& y );

		/** This will set as the default batch rendering to DM_LINES. And will reset the line color to ColorA(255,255,255,255). */
		void LinesBegin();

		/** Set the line color */
		void LinesSetColor( const ColorA& Color );

		/** Set the line color, per vertex */
		void LinesSetColorFree( const ColorA& Color0, const ColorA& Color1 );

		/** Add to the batch a line ( this will change your batch rendering method to DM_LINES, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchLine( const Float& x0, const Float& y0, const Float& x1, const Float& y1 );

		/** This will set as the default batch rendering to GL_LINE_LOOP. And will reset the line color to ColorA(255,255,255,255). */
		void LineLoopBegin();

		/** Set the line color */
		void LineLoopSetColor( const ColorA& Color );

		/** Set the line color, per vertex */
		void LineLoopSetColorFree( const ColorA& Color0, const ColorA& Color1 );

		/** Add to the batch a line ( this will change your batch rendering method to DM_LINE_LOOP, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchLineLoop( const Float& x0, const Float& y0, const Float& x1, const Float& y1 );

		/** Add to the batch a point to the line loop batch ( this will change your batch rendering method to DM_LINE_LOOP, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchLineLoop( const Float& x0, const Float& y0 );

		/** Add to the batch a line ( this will change your batch rendering method to DM_LINE_LOOP, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchLineLoop( const Vector2f& vector1, const Vector2f& vector2 );

		/** Add to the batch a point to the line loop batch ( this will change your batch rendering method to DM_LINE_LOOP, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchLineLoop( const Vector2f& vector1 );

		/** This will set as the default batch rendering to DM_LINE_STRIP. And will reset the line color to ColorA(255,255,255,255). */
		void LineStripBegin();

		/** Set the line color */
		void LineStripSetColor( const ColorA& Color );

		/** Set the line color, per vertex */
		void LineStripSetColorFree( const ColorA& Color0, const ColorA& Color1 );

		/** Add to the batch a line ( this will change your batch rendering method to DM_LINE_STRIP, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchLineStrip( const Float& x0, const Float& y0, const Float& x1, const Float& y1 );

		/** Add to the batch a point to the line strip batch ( this will change your batch rendering method to DM_LINE_STRIP, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchLineStrip( const Float& x0, const Float& y0 );

		/** Add to the batch a line ( this will change your batch rendering method to DM_LINE_STRIP, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchLineStrip( const Vector2f& vector1, const Vector2f& vector2 );

		/** Add to the batch a point to the line strip batch ( this will change your batch rendering method to DM_LINE_STRIP, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchLineStrip( const Vector2f& vector1 );

		/** This will set as the default batch rendering to DM_TRIANGLE_FAN. And will reset the line color to ColorA(255,255,255,255). */
		void TriangleFanBegin();

		/** Set the triangle fan color */
		void TriangleFanSetColor( const ColorA& Color );

		/** Set the triangle fan color, per vertex */
		void TriangleFanSetColorFree( const ColorA& Color0, const ColorA& Color1, const ColorA& Color2 );

		/** Set the texture sector to be rendered but freely seted */
		void TriangleFanSetSubset( const Float& x0, const Float& y0, const Float& x1, const Float& y1, const Float& x2, const Float& y2 );

		/** Add to the batch a triangle fan ( this will change your batch rendering method to DM_TRIANGLE_FAN, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchTriangleFan( const Float& x0, const Float& y0, const Float& x1, const Float& y1, const Float& x2, const Float& y2 );

		/** Add to the batch a triangle fan ( this will change your batch rendering method to DM_TRIANGLE_FAN, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchTriangleFan( const Float& x0, const Float& y0 );

		/** This will set as the default batch rendering to DM_TRIANGLES. And will reset the line color to ColorA(255,255,255,255). */
		void TrianglesBegin();

		/** Set the triangles color */
		void TrianglesSetColor( const ColorA& Color );

		/** Set the triangles color, per vertex */
		void TrianglesSetColorFree( const ColorA& Color0, const ColorA& Color1, const ColorA& Color2 );

		/** Set the texture sector to be rendered but freely seted */
		void TrianglesSetSubset( const Float& x0, const Float& y0, const Float& x1, const Float& y1, const Float& x2, const Float& y2 );

		/** Add to the batch a triangle ( this will change your batch rendering method to DM_TRIANGLES, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchTriangle( const Float& x0, const Float& y0, const Float& x1, const Float& y1, const Float& x2, const Float& y2 );

		/** Set the polygon color */
		void PolygonSetColor( const ColorA& Color );

		/** Add to the batch a polygon ( this will change your batch rendering method to DM_POLYGON, so if you were using another one will Draw all the batched vertexs first ) */
		void BatchPolygon( const Polygon2f& Polygon );

		/** Set the line width */
		void SetLineWidth( const Float& lineWidth );

		/** @return The current line width */
		Float GetLineWidth();

		/** Set the point size */
		void SetPointSize( const Float& pointSize );

		/** @return The current point size */
		Float GetPointSize();

		/** Batch a poligon adding one by one vector */
		void BatchPolygonByPoint( const Float& x, const Float& y );

		/** Batch a poligon adding one by one vector */
		void BatchPolygonByPoint( const Vector2f& Vector );

		/** Foce the blending mode change, ignoring if it's the same that before ( so you can change the blend mode and restore it without problems ) */
		void ForceBlendModeChange( const bool& Force );

		/** @return If the blending mode switch is forced */
		const bool& ForceBlendModeChange() const;
	protected:
		eeVertex *			mVertex;
		unsigned int				mVertexSize;
		eeVertex *			mTVertex;
		unsigned int				mNumVertex;

		const Texture *	mTexture;
		TextureFactory *	mTF;
		EE_BLEND_MODE		mBlend;

		eeTexCoord			mTexCoord[4];
		ColorA			mVerColor[4];

		EE_DRAW_MODE		mCurrentMode;

		Float				mRotation;
		Vector2f			mScale;
		Vector2f			mPosition;
		Vector2f			mCenter;

		bool				mForceRendering;
		bool				mForceBlendMode;

		void Flush();

		void Init();

		void AddVertexs( const unsigned int& num );

		void Rotate( const Vector2f& center, Vector2f* point, const Float& angle );

		void SetBlendMode( EE_DRAW_MODE Mode, const bool& Force );
};

}}

#endif
