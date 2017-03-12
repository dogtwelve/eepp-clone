#ifndef EE_UICUIGENERICGRID_HPP
#define EE_UICUIGENERICGRID_HPP

#include <eepp/ui/uicontrolanim.hpp>
#include <eepp/ui/uitablecell.hpp>
#include <eepp/ui/uiscrollbar.hpp>
#include <eepp/ui/uiitemcontainer.hpp>

namespace EE { namespace UI {

class EE_API UITable : public UIWidget {
	public:
		static UITable * New();

		UITable();

		~UITable();

		virtual Uint32 getType() const;

		virtual bool isType( const Uint32& type ) const;

		virtual void setTheme( UITheme * Theme );

		void add( UITableCell * Cell );

		void remove( UITableCell * Cell );

		void remove( std::vector<Uint32> ItemsIndex );

		void remove( Uint32 ItemIndex );

		UITable * setCollumnWidth( const Uint32& CollumnIndex, const Uint32& collumnWidth );

		const Uint32& getCollumnWidth( const Uint32& CollumnIndex ) const;

		Uint32 getCount() const;

		UITable * setCollumnsCount(const Uint32 & collumnsCount);

		const Uint32& getCollumnsCount() const;

		UITable * setRowHeight( const Uint32& height );

		const Uint32& getRowHeight() const;

		UITableCell * getCell( const Uint32& CellIndex ) const;

		void setVerticalScrollMode( const UI_SCROLLBAR_MODE& Mode );

		const UI_SCROLLBAR_MODE& getVerticalScrollMode();

		void setHorizontalScrollMode( const UI_SCROLLBAR_MODE& Mode );

		const UI_SCROLLBAR_MODE& getHorizontalScrollMode();

		Uint32 getCellPosition( const Uint32& CollumnIndex );

		UIScrollBar * getVerticalScrollBar() const;

		UIScrollBar * getHorizontalScrollBar() const;

		Uint32 getItemIndex( UITableCell * Item );

		UITableCell * getItemSelected();

		Uint32 getItemSelectedIndex() const;

		Uint32 onMessage( const UIMessage * Msg );

		UIItemContainer<UITable> * getContainer() const;

		virtual void update();

		bool isTouchDragEnabled() const;

		void setTouchDragEnabled( const bool& enable );

		bool isTouchDragging() const;

		void setTouchDragging( const bool& dragging );

		bool getSmoothScroll() const;

		void setSmoothScroll(bool smoothScroll);

		Float getTouchDragDeceleration() const;

		void setTouchDragDeceleration(const Float & touchDragDeceleration);

		Recti getContainerPadding() const;

		void setContainerPadding( const Recti & containerPadding);
	protected:
		friend class UIItemContainer<UITable>;
		friend class UITableCell;

		Recti						mContainerPadding;
		bool						mSmoothScroll;
		UIItemContainer<UITable> *	mContainer;
		UIScrollBar *				mVScrollBar;
		UIScrollBar *				mHScrollBar;
		UI_SCROLLBAR_MODE			mVScrollMode;
		UI_SCROLLBAR_MODE			mHScrollMode;
		std::vector<UITableCell*>	mItems;
		Uint32						mCollumnsCount;
		Uint32						mRowHeight;
		std::vector<Uint32>			mCollumnsWidth;
		std::vector<Uint32>			mCollumnsPos;
		Uint32						mTotalWidth;
		Uint32						mTotalHeight;
		Uint32						mLastPos;
		Uint32						mVisibleFirst;
		Uint32						mVisibleLast;
		Int32						mHScrollInit;
		Int32						mItemsNotVisible;
		Int32						mSelected;

		Vector2i					mTouchDragPoint;
		Float						mTouchDragAcceleration;
		Float						mTouchDragDeceleration;

		bool						mCollWidthAssigned;

		void updateCells();

		void updateCollumnsPos();

		void autoPadding();

		virtual void onSizeChange();

		virtual void onAlphaChange();

		void containerResize();

		void onScrollValueChange( const UIEvent * Event );

		void setDefaultCollumnsWidth();

		void updateScroll( bool FromScrollChange = false );

		void updateSize();

		virtual Uint32 onSelected();

		void updateVScroll();

		void updateHScroll();

		void setHScrollStep();
};

}}

#endif
