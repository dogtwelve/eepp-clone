#include <eepp/gaming/mapeditor/mapobjectproperties.hpp>
#include <eepp/gaming/mapeditor/mapeditor.hpp>

namespace EE { namespace Gaming { namespace Private {

MapObjectProperties::MapObjectProperties( GameObjectObject * Obj ) :
	mUITheme( NULL ),
	mUIWindow( NULL ),
	mGenGrid( NULL ),
	mObj( Obj )
{
	if ( NULL == mObj ) {
		eeDelete( this );
		return;
	}

	mUITheme		= UIThemeManager::instance()->getDefaultTheme();

	if ( NULL == mUITheme )
		return;

	mUIWindow	= UIWindow::New();
	mUIWindow->setSizeWithDecoration( 500, 500 )->setWinFlags( UI_WIN_DEFAULT_FLAGS | UI_WIN_MODAL )->setMinWindowSize( 500, 500 );

	mUIWindow->addEventListener( UIEvent::EventOnWindowClose, cb::Make1( this, &MapObjectProperties::onWindowClose ) );
	mUIWindow->setTitle( "Object Properties" );

	Int32 InitialY		= 16;
	Int32 DistFromTitle	= 18;

	UITextBox * Txt = mUITheme->createTextBox( "Object name:", mUIWindow->getContainer(), Sizei(), Vector2i( 50, InitialY ), UI_CONTROL_DEFAULT_FLAGS | UI_DRAW_SHADOW | UI_AUTO_SIZE );
	mUIInput = mUITheme->createTextInput( mUIWindow->getContainer(), Sizei( 120, 22 ), Vector2i( Txt->getPosition().x + DistFromTitle, Txt->getPosition().y + DistFromTitle ), UI_CONTROL_DEFAULT_FLAGS | UI_CLIP_ENABLE | UI_AUTO_PADDING | UI_AUTO_SIZE, true, 64 );
	mUIInput->setText( mObj->getName() );
	mUIInput->addEventListener( UIEvent::EventOnPressEnter, cb::Make1( this, &MapObjectProperties::onOKClick ) );

	UITextBox * Txt2 = mUITheme->createTextBox( "Object type:", mUIWindow->getContainer(), Sizei(), Vector2i( 50+192, InitialY ), UI_CONTROL_DEFAULT_FLAGS | UI_DRAW_SHADOW | UI_AUTO_SIZE );
	mUIInput2 = mUITheme->createTextInput( mUIWindow->getContainer(), Sizei( 120, 22 ), Vector2i( Txt2->getPosition().x + DistFromTitle, Txt2->getPosition().y + DistFromTitle ), UI_CONTROL_DEFAULT_FLAGS | UI_CLIP_ENABLE | UI_AUTO_PADDING | UI_AUTO_SIZE, true, 64 );
	mUIInput2->setText( mObj->getTypeName() );
	mUIInput2->addEventListener( UIEvent::EventOnPressEnter, cb::Make1( this, &MapObjectProperties::onOKClick ) );

	Uint32 TxtBoxFlags = UI_CONTROL_DEFAULT_FLAGS | UI_DRAW_SHADOW | UI_HALIGN_CENTER | UI_VALIGN_CENTER;
	mUITheme->createTextBox( "Property Name", mUIWindow->getContainer(), Sizei(192, 24), Vector2i( 50, mUIInput->getPosition().y + mUIInput->getSize().getHeight() + 12 ), TxtBoxFlags );
	UITextBox * TxtBox = mUITheme->createTextBox( "Property Value", mUIWindow->getContainer(), Sizei(192, 24), Vector2i( 50+192, mUIInput->getPosition().y + mUIInput->getSize().getHeight() + 12 ), TxtBoxFlags );

	UIPushButton * OKButton = UIPushButton::New();
	OKButton->setParent(  mUIWindow->getContainer() )->setSize( 80, 0 );
	OKButton->setIcon( mUITheme->getIconByName( "ok" ) );
	OKButton->setPosition( mUIWindow->getContainer()->getSize().getWidth() - OKButton->getSize().getWidth() - 4, mUIWindow->getContainer()->getSize().getHeight() - OKButton->getSize().getHeight() - 4 );
	OKButton->addEventListener( UIEvent::EventMouseClick, cb::Make1( this, &MapObjectProperties::onOKClick ) );
	OKButton->setText( "OK" );
	OKButton->setAnchors( UI_ANCHOR_RIGHT | UI_ANCHOR_BOTTOM );

	UIPushButton * CancelButton = UIPushButton::New();
	CancelButton->setParent( mUIWindow->getContainer() )->setSize( OKButton->getSize() )->setPosition( OKButton->getPosition().x - OKButton->getSize().getWidth() - 4, OKButton->getPosition().y );
	CancelButton->setIcon( mUITheme->getIconByName( "cancel" ) );
	CancelButton->addEventListener( UIEvent::EventMouseClick, cb::Make1( this, &MapObjectProperties::onCancelClick ) );
	CancelButton->setText( "Cancel" );
	CancelButton->setAnchors( UI_ANCHOR_RIGHT | UI_ANCHOR_BOTTOM );

	mGenGrid = UIGenericGrid::New();
	mGenGrid->setParent( mUIWindow->getContainer() );
	mGenGrid->setSize( 400, 340 )->setPosition( 50, TxtBox->getPosition().y + TxtBox->getSize().getHeight() );
	mGenGrid->setRowHeight( 24 )->setCollumnsCount( 5 );
	mGenGrid->setAnchors( UI_ANCHOR_LEFT | UI_ANCHOR_TOP | UI_ANCHOR_RIGHT | UI_ANCHOR_BOTTOM );
	mGenGrid->setCollumnWidth( 0, 10 );
	mGenGrid->setCollumnWidth( 1, 175 );
	mGenGrid->setCollumnWidth( 2, 10 );
	mGenGrid->setCollumnWidth( 3, 175 );
	mGenGrid->setCollumnWidth( 4, 10 );

	Vector2i Pos( mGenGrid->getPosition().x + mGenGrid->getSize().getWidth() + 10, mGenGrid->getPosition().y );

	UIPushButton * AddButton = UIPushButton::New();
	AddButton->setParent( mUIWindow->getContainer() )->setSize( 24, 0 )->setPosition( Pos );
	AddButton->setIcon( mUITheme->getIconByName( "add" ) );
	AddButton->setAnchors( UI_ANCHOR_RIGHT | UI_ANCHOR_TOP );
	AddButton->addEventListener( UIEvent::EventMouseClick, cb::Make1( this, &MapObjectProperties::onAddCellClick ) );

	if ( NULL == AddButton->getIcon()->getSubTexture() )
		AddButton->setText( "+" );

	Pos.y += AddButton->getSize().getHeight() + 5;

	UIPushButton * RemoveButton = UIPushButton::New();
	RemoveButton->setParent( mUIWindow->getContainer() )->setSize( 24, 0 )->setPosition( Pos );
	RemoveButton->setIcon( mUITheme->getIconByName( "remove" ) );
	RemoveButton->setAnchors( UI_ANCHOR_RIGHT | UI_ANCHOR_TOP );
	RemoveButton->addEventListener( UIEvent::EventMouseClick, cb::Make1( this, &MapObjectProperties::onRemoveCellClick ) );

	if ( NULL == RemoveButton->getIcon()->getSubTexture() )
		RemoveButton->setText( "-" );

	createGridElems();

	mUIWindow->center();
	mUIWindow->show();
}

MapObjectProperties::~MapObjectProperties() {
}

void MapObjectProperties::saveProperties() {
	mObj->clearProperties();

	for ( Uint32 i = 0; i < mGenGrid->getCount(); i++ ) {
		UIGridCell * Cell = mGenGrid->getCell( i );

		UITextInput * Input = reinterpret_cast<UITextInput*>( Cell->getCell( 1 ) );
		UITextInput * Input2 = reinterpret_cast<UITextInput*>( Cell->getCell( 3 ) );

		if ( NULL != Cell && Input->getText().size() && Input2->getText().size() ) {
			mObj->addProperty(	Input->getText(), Input2->getText() );
		}
	}
}

void MapObjectProperties::loadProperties() {
	GameObjectObject::PropertiesMap& Proper = mObj->getProperties();

	for ( GameObjectObject::PropertiesMap::iterator it = Proper.begin(); it != Proper.end(); it++ ) {
		UIGridCell * Cell = createCell();

		UITextInput * Input = reinterpret_cast<UITextInput*>( Cell->getCell( 1 ) );
		UITextInput * Input2 = reinterpret_cast<UITextInput*>( Cell->getCell( 3 ) );

		Input->setText( it->first );
		Input2->setText( it->second );

		mGenGrid->add( Cell );
	}
}

void MapObjectProperties::onOKClick( const UIEvent * Event ) {
	saveProperties();

	mObj->setName( mUIInput->getText().toUtf8() );
	mObj->setTypeName( mUIInput2->getText().toUtf8() );

	mUIWindow->closeWindow();
}

void MapObjectProperties::onCancelClick( const UIEvent * Event ) {
	mUIWindow->closeWindow();
}

void MapObjectProperties::onWindowClose( const UIEvent * Event ) {
	eeDelete( this );
}

void MapObjectProperties::onAddCellClick( const UIEvent * Event ) {
	mGenGrid->add( createCell() );

	Uint32 Index = mGenGrid->getItemSelectedIndex();

	if ( eeINDEX_NOT_FOUND == Index ) {
		mGenGrid->getCell( 0 )->select();
	}
}

void MapObjectProperties::onRemoveCellClick( const UIEvent * Event ) {
	Uint32 Index = mGenGrid->getItemSelectedIndex();

	if ( eeINDEX_NOT_FOUND != Index ) {
		mGenGrid->remove( Index );

		if ( Index < mGenGrid->getCount() ) {
			mGenGrid->getCell( Index )->select();
		} else {
			if ( mGenGrid->getCount() ) {
				if ( Index > 0 )
					mGenGrid->getCell( Index - 1 )->select();
			}
		}
	}
}

void MapObjectProperties::createGridElems() {
	loadProperties();

	if ( 0 == mGenGrid->getCount() ) {
		onAddCellClick( NULL );
	} else {
		mGenGrid->getCell( 0 )->select();
	}
}

UIGridCell * MapObjectProperties::createCell() {
	UIGridCell * Cell = UIGridCell::New();
	UITextInput * TxtInput = UITextInput::New();
	UITextInput * TxtInput2 = UITextInput::New();

	Cell->setParent( mGenGrid->getContainer() );
	TxtInput->setMaxLength( LAYER_NAME_SIZE );
	TxtInput2->setMaxLength( LAYER_NAME_SIZE );

	Cell->setCell( 0, UIComplexControl::New() );
	Cell->setCell( 1, TxtInput );
	Cell->setCell( 2, UIComplexControl::New() );
	Cell->setCell( 3, TxtInput2 );
	Cell->setCell( 4, UIComplexControl::New() );

	return Cell;
}

}}}

