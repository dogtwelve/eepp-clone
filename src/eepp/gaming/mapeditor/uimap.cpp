#include <eepp/gaming/mapeditor/uimap.hpp>
#include <eepp/gaming/mapeditor/mapobjectproperties.hpp>
#include <eepp/gaming/gameobjectobject.hpp>
#include <eepp/gaming/mapobjectlayer.hpp>
#include <eepp/ui/uimanager.hpp>
#include <eepp/ui/uipopupmenu.hpp>

namespace EE { namespace Gaming { namespace Private {

UIMap::UIMap( const UIComplexControl::CreateParams& Params, UITheme * Theme, TileMap * Map ) :
	UIComplexControl( Params ),
	mMap( Map ),
	mCurLayer( NULL ),
	mEditingMode( 0 ),
	mEditingObjMode( SELECT_OBJECTS ),
	mAddLight( NULL ),
	mSelLight( NULL ),
	mClampToTile(true),
	mObjRECTEditing(false),
	mObjPolyEditing(false),
	mObjDragging( false ),
	mSelObj( NULL ),
	mTheme( Theme ),
	mSelPointIndex( eeINDEX_NOT_FOUND ),
	mSelPoint( false ),
	mTileBox( NULL )
{
	if ( NULL == Map ) {
		mMap = eeNew( TileMap, () );
	}

	mMap->BackColor( ColorA( 100, 100, 100, 100 ) );
	mMap->GridLinesColor( ColorA( 150, 150, 150, 150 ) );

	mMap->SetDrawCallback( cb::Make0( this, &UIMap::MapDraw ) );

	mDragButton = EE_BUTTON_MMASK;
	DragEnable( true );

	UpdateScreenPos();
}

UIMap::~UIMap() {
	eeSAFE_DELETE( mMap );
}

Uint32 UIMap::OnDrag( const Vector2i& Pos ) {

	if (	( EDITING_OBJECT == mEditingMode && NULL != mSelObj ) ||
			( EDITING_LIGHT == mEditingMode && NULL != mSelLight ) ) {
		mDragPoint = Pos;
		return 0;
	}

	Vector2i nPos( -( mDragPoint - Pos ) );
	Vector2f nPosf( nPos.x, nPos.y );

	mMap->Move( nPosf );

	mDragPoint = Pos;

	if ( mUpdateScrollCb.IsSet() ) {
		mUpdateScrollCb();
	}

	return 0;
}

void UIMap::ReplaceMap( TileMap * newMap ) {
	eeSAFE_DELETE( mMap );
	mMap = newMap;
	UpdateScreenPos();
}

TileMap * UIMap::Map() const {
	return mMap;
}

void UIMap::Draw() {
	UIComplexControl::Draw();

	if ( NULL != mMap ) {
		mMap->Draw();
	}
}

void UIMap::UpdateScreenPos() {
	UIComplexControl::UpdateScreenPos();

	if ( NULL != mMap ) {
		mMap->Position( mScreenPos );
	}
}

void UIMap::Update() {
	UIComplexControl::Update();

	if ( NULL != mMap ) {
		mMap->Update();

		if ( mEnabled && mVisible && IsMouseOver() ) {
			Uint32 Flags 			= UIManager::instance()->GetInput()->ClickTrigger();

			if ( EDITING_LIGHT == mEditingMode ) {
				if ( NULL != mSelLight ) {
					if ( Flags & EE_BUTTONS_WUWD ) {
						if ( Flags & EE_BUTTON_WUMASK ) {
							mSelLight->Radius( mSelLight->Radius() + 10 );
						} else if ( Flags & EE_BUTTON_WDMASK ) {
							mSelLight->Radius( mSelLight->Radius() - 10 );
						}

						if ( mLightRadiusChangeCb.IsSet() )
							mLightRadiusChangeCb( mSelLight );
					} else if ( Flags & EE_BUTTON_RMASK ) {
						if ( mSelLight == mAddLight ) {
							mMap->GetLightManager()->RemoveLight( mAddLight );

							eeSAFE_DELETE( mAddLight );

							mSelLight = NULL;
						} else if ( NULL != mSelLight ) {
							if ( mSelLight->GetAABB().Contains( mMap->GetMouseMapPosf() ) ) {
								mMap->GetLightManager()->RemoveLight( mSelLight );

								eeSAFE_DELETE( mSelLight );
							}
						}
					} else if ( Flags & EE_BUTTON_LMASK ) {
						if ( mSelLight == mAddLight ) {
							mAddLight = NULL;
						} else {
							TryToSelectLight();
						}
					}

					Flags = UIManager::instance()->GetInput()->PressTrigger();

					if ( Flags & EE_BUTTON_MMASK ) {
						mSelLight->Position( mMap->GetMouseMapPosf() );
					}
				} else {
					if ( Flags & EE_BUTTON_LMASK ) {
						TryToSelectLight();
					}
				}
			} else if ( EDITING_OBJECT == mEditingMode ) {
				ManageObject( Flags );
			}
		}
	}
}

Vector2f UIMap::GetMouseMapPos() {
	Vector2f mp( mMap->GetMouseMapPosf() );

	if ( mClampToTile ) {
		Vector2i mpc( mMap->GetTileCoords( mMap->GetMouseTilePos() + 1 ) );
		mp = Vector2f( mpc.x, mpc.y );
	}

	return mp;
}

void UIMap::SelectPolyObj() {
	if ( NULL != mCurLayer && mCurLayer->Type() == MAP_LAYER_OBJECT ) {
		MapObjectLayer * tLayer = reinterpret_cast<MapObjectLayer*>( mCurLayer );

		GameObject * tObj = tLayer->GetObjectOver( mMap->GetMouseMapPos(), MapObjectLayer::SEARCH_POLY );

		if ( NULL != tObj ) {
			if ( NULL != mSelObj ) {
				mSelObj->Selected( false );
			}

			mSelObj = reinterpret_cast<GameObjectObject*>( tObj );
			mSelObj->Selected( true );
		} else {
			if ( NULL != mSelObj ) {
				mSelObj->Selected( false );
				mSelObj = NULL;
			}
		}
	} else {
		if ( mAlertCb.IsSet() ) {
			mAlertCb( "No layer found", "An Object Layer must be selected first." )->SetFocus();
		}
	}
}

void UIMap::SelectPolyPoint() {
	if ( NULL != mCurLayer && mCurLayer->Type() == MAP_LAYER_OBJECT && NULL != mSelObj ) {
		if ( mSelObj->PointInside( mMap->GetMouseMapPosf() ) ) {
			mSelPointIndex = mSelObj->GetPolygon().ClosestPoint( mMap->GetMouseMapPosf() );
			SetPointRect( mSelObj->GetPolygon().GetAt( mSelPointIndex ) );
		}
	}
}

void UIMap::DragPoly( Uint32 Flags, Uint32 PFlags ) {
	if ( ( PFlags & EE_BUTTON_MMASK ) && NULL != mSelObj ) {
		if ( mSelObj->PointInside( mMap->GetMouseMapPosf() ) ) {
			if ( !mObjDragging ) {
				mObjDragging = true;
				mObjDragDist = GetMouseMapPos() - mSelObj->Pos();
			}
		}

		mSelObj->Pos( GetMouseMapPos() - mObjDragDist );

		if ( EDIT_POLYGONS == mEditingObjMode ) {
			SelectPolyPoint();
		}
	} else if ( Flags & EE_BUTTON_MMASK ) {
		if ( mObjDragging ) {
			mObjDragging = false;
			mObjDragDist = Vector2f(0,0);
		}
	}
}

void UIMap::ManageObject( Uint32 Flags ) {
	Uint32 PFlags	= UIManager::instance()->GetInput()->PressTrigger();
	Uint32 LPFlags	= UIManager::instance()->GetInput()->LastPressTrigger();

	switch ( mEditingObjMode )
	{
		case INSERT_OBJECT:
		{
			if ( PFlags & EE_BUTTON_LMASK ) {
				Vector2f mp( GetMouseMapPos() );

				if ( !mObjRECTEditing ) {
					mObjRECTEditing = true;
					mObjRECT		= Rectf( mp, Sizef(0,0) );
				} else {
					if ( mObjRECT.Pos().x < mp.x && mObjRECT.Pos().y < mp.y ) {
						mObjRECT		= Rectf( mObjRECT.Pos(), Sizef( mp - mObjRECT.Pos() ) );
					}
				}
			}

			if ( Flags & EE_BUTTON_LMASK ){
				if ( mObjRECTEditing ) {
					mAddObjectCallback( GAMEOBJECT_TYPE_OBJECT, Polygon2f( mObjRECT ) );
					mObjRECTEditing = false;
				}
			}

			break;
		}
		case INSERT_POLYLINE:
		case INSERT_POLYGON:
		{
			if ( Flags & EE_BUTTON_LMASK ) {
				mObjPoly.PushBack( GetMouseMapPos() );
			} else if ( Flags & EE_BUTTON_RMASK ) {
				mAddObjectCallback( ( INSERT_POLYGON == mEditingObjMode ) ? GAMEOBJECT_TYPE_POLYGON : GAMEOBJECT_TYPE_POLYLINE, mObjPoly );

				mObjPoly.Clear();
			}

			break;
		}
		case SELECT_OBJECTS:
		{
			if ( ( Flags & EE_BUTTON_LMASK ) ) {
				SelectPolyObj();
			} else {
				DragPoly( Flags, PFlags );
			}

			break;
		}
		case EDIT_POLYGONS:
		{
			if ( ( Flags & EE_BUTTON_LMASK ) ) {
				if ( !mSelPoint ) {
					SelectPolyObj();
					SelectPolyPoint();
				} else {
					mSelPoint = false;
				}
			} else if ( !( LPFlags & EE_BUTTON_LMASK  ) && ( PFlags & EE_BUTTON_LMASK ) ) {
				if ( NULL != mSelObj && eeINDEX_NOT_FOUND != mSelPointIndex && mSelPointRect.Contains( mMap->GetMouseMapPosf() ) ) {
					mSelPoint = true;
				}
			} else if ( ( PFlags & EE_BUTTON_LMASK ) ) {
				if ( mSelPoint && NULL != mSelObj && eeINDEX_NOT_FOUND != mSelPointIndex ) {
					mSelObj->SetPolygonPoint( mSelPointIndex, GetMouseMapPos() );
					SetPointRect( GetMouseMapPos() );
				}
			} else {
				DragPoly( Flags, PFlags );
			}

			break;
		}
		default:
		{
		}
	}
}

void UIMap::SetPointRect( Vector2f p ) {
	mSelPointRect = Rectf( Vector2f( p.x - 10, p.y - 10 ), Sizef( 20, 20 ) );
}

void UIMap::TryToSelectLight() {
	MapLight * tLight = mSelLight;
	mSelLight = mMap->GetLightManager()->GetLightOver( mMap->GetMouseMapPosf(), mSelLight );

	if ( NULL != mSelLight && mSelLight != tLight ) {
		if ( mLightSelCb.IsSet() )
			mLightSelCb( mSelLight );
	}
}

void UIMap::OnSizeChange() {
	if ( NULL != mMap ) {
		mMap->Position( mScreenPos );
		mMap->ViewSize( mSize );
	}

	UIComplexControl::OnSizeChange();
}

Uint32 UIMap::OnMouseMove( const Vector2i& Pos, const Uint32 Flags ) {
	if ( NULL != mMap ) {
		if ( EDITING_LIGHT == mEditingMode && NULL != mAddLight ) {
			mAddLight->Position( mMap->GetMouseMapPosf() );
		}
	}

	if ( NULL != mTileBox ) {
		Vector2i mp( mMap->GetMouseTilePos() );

		if ( mLastMouseTilePos != mp ) {
			mLastMouseTilePos = mp;
			mTileBox->Text( String::ToStr( mp.x ) + "," + String::ToStr( mp.y ) );
		}
	}

	return UIComplexControl::OnMouseMove( Pos, Flags );
}

void UIMap::AddLight( MapLight * Light ) {
	if ( NULL != mMap->GetLightManager() ) {
		if ( NULL != mAddLight ) {
			mMap->GetLightManager()->RemoveLight( mAddLight );

			eeSAFE_DELETE( mAddLight );
		}

		mAddLight = Light;

		mSelLight = Light;

		mMap->GetLightManager()->AddLight( Light );

		if ( mLightSelCb.IsSet() )
			mLightSelCb( mSelLight );
	}
}

void UIMap::MapDraw() {
	if ( EDITING_LIGHT == mEditingMode ) {
		if ( NULL != mSelLight ) {
			mP.SetColor( ColorA( 255, 0, 0, (Uint8)mAlpha ) );

			Vector2f Pos( mSelLight->GetAABB().Left, mSelLight->GetAABB().Top );
			eeAABB AB( mSelLight->GetAABB() );

			mP.FillMode( DRAW_LINE );
			mP.DrawRectangle( Rectf( Pos, AB.Size() ) );
		}
	} else if ( EDITING_OBJECT == mEditingMode ) {
		switch ( mEditingObjMode ) {
			case INSERT_OBJECT:
			{
				if ( mObjRECTEditing ) {
					mP.FillMode( DRAW_FILL );
					mP.SetColor( ColorA( 100, 100, 100, 20 ) );
					mP.DrawRectangle( mObjRECT );

					mP.FillMode( DRAW_LINE );
					mP.SetColor( ColorA( 255, 0, 0, 200 ) );
					mP.DrawRectangle( mObjRECT );
				}

				break;
			}
			case INSERT_POLYGON:
			{
				mP.FillMode( DRAW_FILL );
				mP.SetColor( ColorA( 50, 50, 50, 50 ) );
				mP.DrawPolygon( mObjPoly );

				mP.FillMode( DRAW_LINE );
				mP.SetColor( ColorA( 255, 0, 0, 200 ) );
				mP.DrawPolygon( mObjPoly );

				Polygon2f polyN( mObjPoly );
				polyN.PushBack( GetMouseMapPos() );

				mP.FillMode( DRAW_FILL );
				mP.SetColor( ColorA( 100, 100, 100, 100 ) );
				mP.DrawPolygon( polyN );

				mP.FillMode( DRAW_LINE );
				mP.SetColor( ColorA( 255, 255, 0, 200 ) );
				mP.DrawPolygon( polyN );

				break;
			}
			case INSERT_POLYLINE:
			{
				mP.FillMode( DRAW_LINE );
				mP.SetColor( ColorA( 255, 0, 0, 200 ) );
				mP.DrawPolygon( mObjPoly );

				Polygon2f polyN( mObjPoly );
				polyN.PushBack( GetMouseMapPos() );

				mP.FillMode( DRAW_LINE );
				mP.SetColor( ColorA( 255, 255, 0, 200 ) );
				mP.DrawPolygon( polyN );

				break;
			}
			case EDIT_POLYGONS:
			{
				if ( NULL != mSelObj && eeINDEX_NOT_FOUND != mSelPointIndex ) {
					mP.SetColor( ColorA( 255, 255, 100, 100 ) );

					mP.FillMode( DRAW_FILL );
					mP.DrawRectangle( mSelPointRect );

					mP.FillMode( DRAW_LINE );
					mP.DrawRectangle( mSelPointRect );
				}

				break;
			}
		}
	}
}

void UIMap::EditingLights( const bool& editing ) {
	mEditingMode = ( editing ) ? EDITING_LIGHT : 0;

	if ( editing && NULL != mMap->GetLightManager() && NULL != mAddLight ) {
		mMap->GetLightManager()->RemoveLight( mAddLight );

		mAddLight = NULL;
	}
}

bool UIMap::EditingLights() {
	return EDITING_LIGHT == mEditingMode;
}

void UIMap::EditingObjects( const bool& editing ) {
	mEditingMode = ( editing ) ? EDITING_OBJECT : 0;
}

bool UIMap::EditingObjects() {
	return EDITING_OBJECT == mEditingMode;
}

void UIMap::EditingDisabled() {
	mEditingMode = 0;
}

MapLight * UIMap::GetSelectedLight() {
	return mSelLight;
}

MapLight * UIMap::GetAddLight() {
	return mAddLight;
}

void UIMap::SetLightSelectCb( LightSelectCb Cb ) {
	mLightSelCb = Cb;
}

void UIMap::SetLightRadiusChangeCb( LightRadiusChangeCb Cb ) {
	mLightRadiusChangeCb = Cb;
}

void UIMap::SetAddObjectCallback( ObjAddCb Cb ) {
	mAddObjectCallback = Cb;
}

void UIMap::ClearLights() {
	mSelLight = NULL;
	mAddLight = NULL;
}

void UIMap::OnAlphaChange() {
	UIComplexControl::OnAlphaChange();

	if ( NULL != mMap ) {
		mMap->BackAlpha( (Uint8)mAlpha );
	}
}

void UIMap::ClampToTile( const bool& clamp ) {
	mClampToTile = clamp;
}

const bool& UIMap::ClampToTile() const {
	return mClampToTile;
}

void UIMap::EditingObjMode( EDITING_OBJ_MODE mode ) {
	mObjPoly.Clear();
	mSelPointIndex = eeINDEX_NOT_FOUND;

	mEditingObjMode = mode;
}

void UIMap::CurLayer( MapLayer * layer ) {
	mCurLayer = layer;
}

void UIMap::SetAlertCb( AlertCb Cb ) {
	mAlertCb = Cb;
}

void UIMap::SetUpdateScrollCb( UpdateScrollCb Cb ) {
	mUpdateScrollCb = Cb;
}

Uint32 UIMap::OnMessage( const UIMessage * Msg ) {
	if ( Msg->Msg() == UIMessage::MsgClick && Msg->Sender() == this && ( Msg->Flags() & EE_BUTTON_RMASK ) ) {
		if ( SELECT_OBJECTS == mEditingObjMode && NULL != mSelObj && mSelObj->PointInside( mMap->GetMouseMapPosf() ) ) {
			CreateObjPopUpMenu();
		}
	}

	return 0;
}

void UIMap::ObjItemClick( const UIEvent * Event ) {
	if ( !Event->Ctrl()->IsType( UI_TYPE_MENUITEM ) )
		return;

	if ( NULL != mSelObj && NULL != mCurLayer && mCurLayer->Type() == MAP_LAYER_OBJECT && mSelObj->Layer() == mCurLayer ) {
		const String& txt = reinterpret_cast<UIMenuItem*> ( Event->Ctrl() )->Text();

		MapObjectLayer * tLayer = reinterpret_cast<MapObjectLayer*>( mCurLayer );

		if ( "Duplicate Object" == txt ) {
			tLayer->AddGameObject( mSelObj->Copy() );
		} else if ( "Remove Object" == txt ) {
			tLayer->RemoveGameObject( mSelObj );

			mSelObj->Selected( false );
			mSelObj = NULL;
		} else if ( "Object Properties..." == txt ) {
			eeNew( MapObjectProperties, ( mSelObj ) );
		}
	}
}

void UIMap::CreateObjPopUpMenu() {
	UIPopUpMenu * Menu = mTheme->CreatePopUpMenu();

	Menu->Add( "Duplicate Object" );
	Menu->Add( "Remove Object" );
	Menu->AddSeparator();
	Menu->Add( "Object Properties..." );
	Menu->AddEventListener( UIEvent::EventOnItemClicked, cb::Make1( this, &UIMap::ObjItemClick ) );

	if ( Menu->Show() ) {
		Vector2i Pos = UIManager::instance()->GetInput()->GetMousePos();
		UIMenu::FixMenuPos( Pos , Menu );
		Menu->Pos( Pos );
	}
}

void UIMap::SetTileBox( UITextBox * tilebox ) {
	mTileBox = tilebox;
}

}}}
