#include <eepp/graphics/pixelperfect.hpp>

namespace EE { namespace Graphics {

bool PixelPerfectCollide( Texture * Tex1, const unsigned int& x1, const unsigned int& y1, Texture * Tex2, const unsigned int& x2, const unsigned int& y2, const Rectu& Tex1_SrcRECT, const Rectu& Tex2_SrcRECT ) {
	eeASSERT( NULL != Tex1 && NULL != Tex2 );

	unsigned int ax1, ay1, ax2, ay2, bx1, by1, bx2, by2;
	bool Collide = false;

	ax1 = x1;
	ay1 = y1;
	if (Tex1_SrcRECT.Right != 0 && Tex1_SrcRECT.Bottom != 0) {
		ax2 = ax1 + Tex1_SrcRECT.Right - Tex1_SrcRECT.Left - 1;
		ay2 = ay1 + Tex1_SrcRECT.Bottom - Tex1_SrcRECT.Top - 1;
	} else {
		ax2 = ax1 + Tex1->Width() - 1;
		ay2 = ay1 + Tex1->Height() - 1;
	}

	bx1 = x2;
	by1 = y2;
	if (Tex2_SrcRECT.Right != 0 && Tex2_SrcRECT.Bottom != 0) {
		bx2 = bx1 + Tex2_SrcRECT.Right - Tex2_SrcRECT.Left - 1;
		by2 = by1 + Tex2_SrcRECT.Bottom - Tex2_SrcRECT.Top - 1;
	} else {
		bx2 = bx1 + Tex2->Width() - 1;
		by2 = by1 + Tex2->Height() - 1;
	}

	if ( !(ax1 > bx2 || ax2 < bx1 || ay1 > by2 || ay2 < by1) ) {
		unsigned int inter_x0, inter_x1, inter_y0, inter_y1;
		
		Tex1->Lock();
		Tex2->Lock();
		
		inter_x0 = eemax(ax1,bx1);
		inter_x1 = eemin(ax2,bx2);
		inter_y0 = eemax(ay1,by1);
		inter_y1 = eemin(ay2,by2);
		ColorA C1, C2;

		for(unsigned int y = inter_y0 ; y <= inter_y1 ; y++) {
			for(unsigned int x = inter_x0 ; x <= inter_x1 ; x++) {
				if (Tex1_SrcRECT.Right != 0 && Tex1_SrcRECT.Bottom != 0)
					C1 = Tex1->GetPixel( x - x1 + Tex1_SrcRECT.Left, y - y1 + Tex1_SrcRECT.Top );
				else
					C1 = Tex1->GetPixel( x - x1, y - y1 );

				if (Tex2_SrcRECT.Right != 0 && Tex2_SrcRECT.Bottom != 0)
					C2 = Tex2->GetPixel( x - x2 + Tex2_SrcRECT.Left, y - y2 + Tex2_SrcRECT.Top );
				else
					C2 = Tex2->GetPixel( x - x2, y - y2 );

				if ( C1.A() > 0 && C2.A() > 0 ) {
					Collide = true;
					break;
				}
			}
		}
		
		Tex1->Unlock(true);
		Tex2->Unlock(true);
	}
	return Collide;
}

bool PixelPerfectCollide( Texture * Tex, const unsigned int& x1, const unsigned int& y1, const unsigned int& x2, const unsigned int& y2, const Rectu& Tex1_SrcRECT) {
	eeASSERT( NULL != Tex );

	unsigned int ax1, ay1, ax2, ay2;
	bool Collide = false;

	ax1 = x1;
	ay1 = y1;
	if (Tex1_SrcRECT.Right != 0 && Tex1_SrcRECT.Bottom != 0) {
		ax2 = ax1 + Tex1_SrcRECT.Right - Tex1_SrcRECT.Left - 1;
		ay2 = ay1 + Tex1_SrcRECT.Bottom - Tex1_SrcRECT.Top - 1;
	} else {
		ax2 = ax1 + Tex->Width() - 1;
		ay2 = ay1 + Tex->Height() - 1;
	}

	if ( !( ax1 >= x2 && ax2 <= x2 && ay1 >= y2 && ay2 <= y2 ) ) {
		ColorA C1;
		
		Tex->Lock();
		
		if (Tex1_SrcRECT.Right != 0 && Tex1_SrcRECT.Bottom != 0)
			C1 = Tex->GetPixel( x2 - ax1 + Tex1_SrcRECT.Left, y2 - ay1 + Tex1_SrcRECT.Top );
		else
			C1 = Tex->GetPixel( x2 - ax1, y2 - ay1 );

		if ( C1.A() > 0 )
			Collide = true;
		
		Tex->Unlock(true);
	}
	return Collide;
}

}}
