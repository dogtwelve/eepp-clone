#include <eepp/ui/uitheme.hpp>
#include <eepp/ui/uiskinsimple.hpp>
#include <eepp/ui/uiskincomplex.hpp>
#include <eepp/graphics/sprite.hpp>
#include <eepp/graphics/textureatlas.hpp>
#include <eepp/graphics/font.hpp>
#include <eepp/graphics/texturefactory.hpp>
#include <eepp/graphics/textureatlasmanager.hpp>
#include <eepp/system/filesystem.hpp>

#include <eepp/ui/uicheckbox.hpp>
#include <eepp/ui/uicombobox.hpp>
#include <eepp/ui/uidropdownlist.hpp>
#include <eepp/ui/uilistbox.hpp>
#include <eepp/ui/uipopupmenu.hpp>
#include <eepp/ui/uiprogressbar.hpp>
#include <eepp/ui/uipushbutton.hpp>
#include <eepp/ui/uiradiobutton.hpp>
#include <eepp/ui/uiscrollbar.hpp>
#include <eepp/ui/uislider.hpp>
#include <eepp/ui/uispinbox.hpp>
#include <eepp/ui/uitextbox.hpp>
#include <eepp/ui/uitextedit.hpp>
#include <eepp/ui/uitextinput.hpp>
#include <eepp/ui/uitextinputpassword.hpp>
#include <eepp/ui/uitooltip.hpp>
#include <eepp/ui/uiwindow.hpp>
#include <eepp/ui/uiwinmenu.hpp>
#include <eepp/ui/uigfx.hpp>
#include <eepp/ui/uisprite.hpp>
#include <eepp/ui/uicommondialog.hpp>
#include <eepp/ui/uimessagebox.hpp>
#include <eepp/ui/uitabwidget.hpp>

namespace EE { namespace UI {

static void LoadThemeElements( std::list<std::string>& UI_THEME_ELEMENTS, std::list<std::string>& UI_THEME_ICONS ) {
		UI_THEME_ELEMENTS.push_back( "control" );
		UI_THEME_ELEMENTS.push_back( "button" );
		UI_THEME_ELEMENTS.push_back( "textinput" );
		UI_THEME_ELEMENTS.push_back( "checkbox" );
		UI_THEME_ELEMENTS.push_back( "checkbox_active" );
		UI_THEME_ELEMENTS.push_back( "checkbox_inactive" );
		UI_THEME_ELEMENTS.push_back( "button" );
		UI_THEME_ELEMENTS.push_back( "radiobutton" );
		UI_THEME_ELEMENTS.push_back( "radiobutton_active" );
		UI_THEME_ELEMENTS.push_back( "radiobutton_inactive" );
		UI_THEME_ELEMENTS.push_back( "hslider" );
		UI_THEME_ELEMENTS.push_back( "hslider_bg" );
		UI_THEME_ELEMENTS.push_back( "hslider_button" );
		UI_THEME_ELEMENTS.push_back( "vslider" );
		UI_THEME_ELEMENTS.push_back( "vslider_bg" );
		UI_THEME_ELEMENTS.push_back( "vslider_button" );
		UI_THEME_ELEMENTS.push_back( "spinbox" );
		UI_THEME_ELEMENTS.push_back( "spinbox_input" );
		UI_THEME_ELEMENTS.push_back( "spinbox_btnup" );
		UI_THEME_ELEMENTS.push_back( "spinbox_btndown" );
		UI_THEME_ELEMENTS.push_back( "hscrollbar" );
		UI_THEME_ELEMENTS.push_back( "hscrollbar_slider" );
		UI_THEME_ELEMENTS.push_back( "hscrollbar_bg" );
		UI_THEME_ELEMENTS.push_back( "hscrollbar_button" );
		UI_THEME_ELEMENTS.push_back( "hscrollbar_btnup" );
		UI_THEME_ELEMENTS.push_back( "hscrollbar_btndown" );
		UI_THEME_ELEMENTS.push_back( "vscrollbar" );
		UI_THEME_ELEMENTS.push_back( "vscrollbar_slider" );
		UI_THEME_ELEMENTS.push_back( "vscrollbar_bg" );
		UI_THEME_ELEMENTS.push_back( "vscrollbar_button" );
		UI_THEME_ELEMENTS.push_back( "vscrollbar_btnup" );
		UI_THEME_ELEMENTS.push_back( "vscrollbar_btndown" );
		UI_THEME_ELEMENTS.push_back( "progressbar" );
		UI_THEME_ELEMENTS.push_back( "progressbar_filler" );
		UI_THEME_ELEMENTS.push_back( "listbox" );
		UI_THEME_ELEMENTS.push_back( "listboxitem" );
		UI_THEME_ELEMENTS.push_back( "dropdownlist" );
		UI_THEME_ELEMENTS.push_back( "combobox" );
		UI_THEME_ELEMENTS.push_back( "menu" );
		UI_THEME_ELEMENTS.push_back( "menuitem" );
		UI_THEME_ELEMENTS.push_back( "separator" );
		UI_THEME_ELEMENTS.push_back( "menucheckbox_active" );
		UI_THEME_ELEMENTS.push_back( "menucheckbox_inactive" );
		UI_THEME_ELEMENTS.push_back( "menuarrow" );
		UI_THEME_ELEMENTS.push_back( "textedit" );
		UI_THEME_ELEMENTS.push_back( "textedit_box" );
		UI_THEME_ELEMENTS.push_back( "tooltip" );
		UI_THEME_ELEMENTS.push_back( "genericgrid" );
		UI_THEME_ELEMENTS.push_back( "gridcell" );
		UI_THEME_ELEMENTS.push_back( "windeco" );
		UI_THEME_ELEMENTS.push_back( "winback" );
		UI_THEME_ELEMENTS.push_back( "winborderleft" );
		UI_THEME_ELEMENTS.push_back( "winborderright" );
		UI_THEME_ELEMENTS.push_back( "winborderbottom" );
		UI_THEME_ELEMENTS.push_back( "winclose" );
		UI_THEME_ELEMENTS.push_back( "winmax" );
		UI_THEME_ELEMENTS.push_back( "winmin" );
		UI_THEME_ELEMENTS.push_back( "winshade" );
		UI_THEME_ELEMENTS.push_back( "winmenu" );
		UI_THEME_ELEMENTS.push_back( "winmenubutton" );
		UI_THEME_ELEMENTS.push_back( "tabwidget" );
		UI_THEME_ELEMENTS.push_back( "tabcontainer" );
		UI_THEME_ELEMENTS.push_back( "tab" );
		UI_THEME_ELEMENTS.push_back( "tab_left" );
		UI_THEME_ELEMENTS.push_back( "tab_right" );

		UI_THEME_ICONS.push_back( "ok" );
		UI_THEME_ICONS.push_back( "cancel" );
		UI_THEME_ICONS.push_back( "remove" );
		UI_THEME_ICONS.push_back( "go-up" );
		UI_THEME_ICONS.push_back( "quit" );
		UI_THEME_ICONS.push_back( "add" );
		UI_THEME_ICONS.push_back( "document-open" );
		UI_THEME_ICONS.push_back( "document-close" );
		UI_THEME_ICONS.push_back( "document-new" );
		UI_THEME_ICONS.push_back( "document-save" );
		UI_THEME_ICONS.push_back( "document-save-as" );
}

void UITheme::AddThemeElement( const std::string& Element ) {
	mUIElements.push_back( Element );
}

void UITheme::AddThemeIcon( const std::string& Icon ) {
	mUIIcons.push_back( Icon );
}

UITheme * UITheme::LoadFromTextureAtlas( UITheme * tTheme, Graphics::TextureAtlas * TextureAtlas ) {
	eeASSERT( NULL != tTheme && NULL != TextureAtlas );

	/** Themes use nearest filter by default, force the filter to the textures. */
	for ( Uint32 tC = 0; tC < TextureAtlas->GetTexturesCount(); tC++ ) {
		TextureAtlas->GetTexture( tC )->Filter( TEX_FILTER_NEAREST );
	}

	Clock TE;

	LoadThemeElements( tTheme->mUIElements, tTheme->mUIIcons );

	Uint32 i;
	bool Found;
	std::string Element;
	std::vector<std::string> 	ElemFound;
	std::vector<Uint32> 		ElemType;

	tTheme->TextureAtlas( TextureAtlas );

	for ( std::list<std::string>::iterator it = tTheme->mUIElements.begin() ; it != tTheme->mUIElements.end(); it++ ) {
		Uint32 IsComplex = 0;

		Element = std::string( tTheme->Abbr() + "_" + *it );

		Found 	= SearchFilesInAtlas( TextureAtlas, Element, IsComplex );

		if ( Found ) {
			ElemFound.push_back( Element );
			ElemType.push_back( IsComplex );
		}
	}

	for ( i = 0; i < ElemFound.size(); i++ ) {
		if ( ElemType[i] )
			tTheme->Add( eeNew( UISkinComplex, ( ElemFound[i] ) ) );
		else
			tTheme->Add( eeNew( UISkinSimple, ( ElemFound[i] ) ) );
	}

	eePRINTL( "UI Theme Loaded in: %4.3f ms ( from TextureAtlas )", TE.Elapsed().AsMilliseconds() );

	return tTheme;
}

UITheme * UITheme::LoadFromPath( UITheme * tTheme, const std::string& Path, const std::string ImgExt ) {
	Clock TE;

	LoadThemeElements( tTheme->mUIElements, tTheme->mUIIcons );

	Uint32 i;
	bool Found;
	std::string Element;
	std::string ElemName;
	std::string RPath( Path );

	FileSystem::DirPathAddSlashAtEnd( RPath );

	if ( !FileSystem::IsDirectory( RPath ) )
		return NULL;

	std::vector<std::string> 	ElemFound;
	std::vector<Uint32> 		ElemType;

	Graphics::TextureAtlas * tSG = eeNew( Graphics::TextureAtlas, ( tTheme->Abbr() ) );

	tTheme->TextureAtlas( tSG );

	for ( std::list<std::string>::iterator it = tTheme->mUIElements.begin() ; it != tTheme->mUIElements.end(); it++ ) {
		Uint32 IsComplex = 0;

		Element = tTheme->Abbr() + "_" + *it;

		Found 	= SearchFilesOfElement( tSG, RPath, Element, IsComplex, ImgExt );

		if ( Found ) {
			ElemFound.push_back( Element );
			ElemType.push_back( IsComplex );
		}
	}

	// Load the icons from path.
	for ( std::list<std::string>::iterator it = tTheme->mUIIcons.begin() ; it != tTheme->mUIIcons.end(); it++ ) {
		ElemName	= tTheme->Abbr() + "_icon_" + *it;
		Element		= RPath + ElemName + "." + ImgExt;

		if ( FileSystem::FileExists( Element ) ) {
			tSG->Add( eeNew( SubTexture, ( TextureFactory::instance()->Load( Element ), ElemName ) ) );
		}
	}

	if ( tSG->Count() )
		TextureAtlasManager::instance()->Add( tSG );
	else
		eeSAFE_DELETE( tSG );

	for ( i = 0; i < ElemFound.size(); i++ ) {
		if ( ElemType[i] )
			tTheme->Add( eeNew( UISkinComplex, ( ElemFound[i] ) ) );
		else
			tTheme->Add( eeNew( UISkinSimple, ( ElemFound[i] ) ) );
	}

	eePRINTL( "UI Theme Loaded in: %4.3f ms ( from path )", TE.Elapsed().AsMilliseconds() );

	return tTheme;
}

UITheme * UITheme::LoadFromPath( const std::string& Path, const std::string& Name, const std::string& NameAbbr, const std::string ImgExt ) {
	return LoadFromPath( eeNew( UITheme, ( Name, NameAbbr ) ), Path, ImgExt );
}

UITheme * UITheme::LoadFromTextureAtlas( Graphics::TextureAtlas * TextureAtlas, const std::string& Name, const std::string NameAbbr ) {
	return LoadFromTextureAtlas( eeNew( UITheme, ( Name, NameAbbr ) ), TextureAtlas );
}

bool UITheme::SearchFilesInAtlas( Graphics::TextureAtlas * SG, std::string Element, Uint32& IsComplex ) {
	bool Found = false;
	Uint32 i = 0, s = 0;
	std::string ElemName;
	IsComplex = 0;

	// Search Complex Skin
	for ( i = 0; i < UISkinState::StateCount; i++ ) {
		for ( s = 0; s < UISkinComplex::SideCount; s++ ) {
			ElemName = Element + "_" + UISkin::GetSkinStateName( i ) + "_" + UISkinComplex::GetSideSuffix( s );

			if ( SG->GetByName( ElemName ) ) {
				IsComplex = 1;
				Found = true;
				break;
			}
		}

		if ( Found ) {
			break;
		}
	}

	// Search Simple Skin
	if ( !IsComplex ) {
		for ( i = 0; i < UISkinState::StateCount; i++ ) {
			ElemName = Element + "_" + UISkin::GetSkinStateName( i );

			if ( SG->GetByName( ElemName ) ) {
				Found = true;
				break;
			}
		}
	}

	return Found;
}

bool UITheme::SearchFilesOfElement( Graphics::TextureAtlas * SG, const std::string& Path, std::string Element, Uint32& IsComplex, const std::string ImgExt ) {
	bool Found = false;
	Uint32 i = 0, s = 0;
	std::string ElemPath;
	std::string ElemFullPath;
	std::string ElemName;
	IsComplex = 0;

	// Search Complex Skin
	for ( i = 0; i < UISkinState::StateCount; i++ ) {
		for ( s = 0; s < UISkinComplex::SideCount; s++ ) {
			ElemName = Element + "_" + UISkin::GetSkinStateName( i ) + "_" + UISkinComplex::GetSideSuffix( s );
			ElemPath = Path + ElemName;
			ElemFullPath = ElemPath + "." + ImgExt;

			if ( FileSystem::FileExists( ElemFullPath ) ) {
				SG->Add( eeNew( SubTexture, ( TextureFactory::instance()->Load( ElemFullPath ), ElemName ) ) );

				IsComplex = 1;
				Found = true;
				break;
			}
		}
	}

	// Seach Simple Skin
	if ( !IsComplex ) {
		for ( i = 0; i < UISkinState::StateCount; i++ ) {
			ElemName = Element + "_" + UISkin::GetSkinStateName( i );
			ElemPath = Path + ElemName;
			ElemFullPath = ElemPath + "." + ImgExt;

			if ( FileSystem::FileExists( ElemFullPath ) ) {
				SG->Add( eeNew( SubTexture, ( TextureFactory::instance()->Load( ElemFullPath ), ElemName ) ) );

				Found = true;
			}
		}
	}

	return Found;
}

UITheme::UITheme( const std::string& Name, const std::string& Abbr, Graphics::Font * DefaultFont ) :
	ResourceManager<UISkin> ( false ),
	mName( Name ),
	mNameHash( String::Hash( mName ) ),
	mAbbr( Abbr ),
	mTextureAtlas( NULL ),
	mFont( DefaultFont ),
	mFontColor( 0, 0, 0, 255 ),
	mFontShadowColor( 255, 255, 255, 200 ),
	mFontOverColor( 0, 0, 0, 255 ),
	mFontSelectedColor( 0, 0, 0, 255 ),
	mUseDefaultThemeValues( true )
{
}

UITheme::~UITheme() {

}

const std::string& UITheme::Name() const {
	return mName;
}

void UITheme::Name( const std::string& name ) {
	mName = name;
	mNameHash = String::Hash( mName );
}

const Uint32& UITheme::Id() const {
	return mNameHash;
}

const std::string& UITheme::Abbr() const {
	return mAbbr;
}

UISkin * UITheme::Add( UISkin * Resource ) {
	Resource->Theme( this );

	return ResourceManager<UISkin>::Add( Resource );
}

void UITheme::Font( Graphics::Font * Font ) {
	mFont = Font;
}

Graphics::Font * UITheme::Font() const {
	return mFont;
}

const ColorA& UITheme::FontColor() const {
	return mFontColor;
}

const ColorA& UITheme::FontShadowColor() const {
	return mFontShadowColor;
}

const ColorA& UITheme::FontOverColor() const {
	return mFontOverColor;
}

const ColorA& UITheme::FontSelectedColor() const {
	return mFontSelectedColor;
}

void UITheme::FontColor( const ColorA& Color ) {
	mFontColor = Color;
}

void UITheme::FontShadowColor( const ColorA& Color ) {
	mFontShadowColor = Color;
}

void UITheme::FontOverColor( const ColorA& Color ) {
	mFontOverColor = Color;
}

void UITheme::FontSelectedColor( const ColorA& Color ) {
	mFontSelectedColor = Color;
}

void UITheme::UseDefaultThemeValues( const bool& Use ) {
	mUseDefaultThemeValues = Use;
}

const bool& UITheme::UseDefaultThemeValues() const {
	return mUseDefaultThemeValues;
}

Graphics::TextureAtlas * UITheme::TextureAtlas() const {
	return mTextureAtlas;
}

void UITheme::TextureAtlas( Graphics::TextureAtlas * SG ) {
	mTextureAtlas = SG;
}

SubTexture * UITheme::GetIconByName( const std::string& name ) {
	if ( NULL != mTextureAtlas )
		return mTextureAtlas->GetByName( mAbbr + "_icon_" + name );

	return NULL;
}

UIGfx * UITheme::CreateGfx( SubTexture * SubTexture, UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, ColorA SubTextureColor, EE_RENDER_MODE SubTextureRender ) {
	UIGfx::CreateParams GfxParams;
	GfxParams.Parent( Parent );
	GfxParams.PosSet( Pos );
	GfxParams.SizeSet( Size );
	GfxParams.Flags = Flags;
	GfxParams.SubTexture = SubTexture;
	GfxParams.SubTextureColor = SubTextureColor;
	GfxParams.SubTextureRender = SubTextureRender;
	UIGfx * Gfx = eeNew( UIGfx, ( GfxParams ) );
	Gfx->Visible( true );
	Gfx->Enabled( true );
	return Gfx;
}

UISprite * UITheme::CreateSprite( Sprite * Sprite, UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, bool DealloSprite, EE_RENDER_MODE SpriteRender ) {
	UISprite::CreateParams SpriteParams;
	SpriteParams.Parent( Parent );
	SpriteParams.PosSet( Pos );
	SpriteParams.SizeSet( Size );
	SpriteParams.Flags = Flags;
	SpriteParams.Sprite = Sprite;
	SpriteParams.SpriteRender = SpriteRender;
	SpriteParams.DealloSprite = DealloSprite;
	UISprite * Spr = eeNew( UISprite, ( SpriteParams ) );
	Spr->Visible( true );
	Spr->Enabled( true );
	return Spr;
}

UICheckBox * UITheme::CreateCheckBox( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags ) {
	UICheckBox::CreateParams CheckBoxParams;
	CheckBoxParams.Parent( Parent );
	CheckBoxParams.PosSet( Pos );
	CheckBoxParams.SizeSet( Size );
	CheckBoxParams.Flags = Flags;
	UICheckBox * Ctrl = eeNew( UICheckBox, ( CheckBoxParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIRadioButton * UITheme::CreateRadioButton( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags ) {
	UIRadioButton::CreateParams RadioButtonParams;
	RadioButtonParams.Parent( Parent );
	RadioButtonParams.PosSet( Pos );
	RadioButtonParams.SizeSet( Size );
	RadioButtonParams.Flags = Flags;
	UIRadioButton * Ctrl = eeNew( UIRadioButton, ( RadioButtonParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UITextBox * UITheme::CreateTextBox( const String& Text, UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags ) {
	UITextBox::CreateParams TextBoxParams;
	TextBoxParams.Parent( Parent );
	TextBoxParams.PosSet( Pos );
	TextBoxParams.SizeSet( Size );
	TextBoxParams.Flags = Flags;
	UITextBox * Ctrl = eeNew( UITextBox, ( TextBoxParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( false );
	Ctrl->Text( Text );
	return Ctrl;
}

UITooltip * UITheme::CreateTooltip( UIControl * TooltipOf, UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags ) {
	UITooltip::CreateParams TooltipParams;
	TooltipParams.Parent( Parent );
	TooltipParams.PosSet( Pos );
	TooltipParams.SizeSet( Size );
	TooltipParams.Flags = Flags;
	UITooltip * Ctrl = eeNew( UITooltip, ( TooltipParams, TooltipOf ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UITextEdit * UITheme::CreateTextEdit( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, UI_SCROLLBAR_MODE HScrollBar, UI_SCROLLBAR_MODE VScrollBar, bool WordWrap ) {
	UITextEdit::CreateParams TextEditParams;
	TextEditParams.Parent( Parent );
	TextEditParams.PosSet( Pos );
	TextEditParams.SizeSet( Size );
	TextEditParams.Flags = Flags;
	TextEditParams.HScrollBar = HScrollBar;
	TextEditParams.VScrollBar = VScrollBar;
	TextEditParams.WordWrap = WordWrap;
	UITextEdit * Ctrl = eeNew( UITextEdit, ( TextEditParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UITextInput * UITheme::CreateTextInput( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, bool SupportFreeEditing, Uint32 MaxLength ) {
	UITextInput::CreateParams TextInputParams;
	TextInputParams.Parent( Parent );
	TextInputParams.PosSet( Pos );
	TextInputParams.SizeSet( Size );
	TextInputParams.Flags = Flags;
	TextInputParams.SupportFreeEditing = SupportFreeEditing;
	TextInputParams.MaxLength = MaxLength;
	UITextInput * Ctrl = eeNew( UITextInput, ( TextInputParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UITextInputPassword * UITheme::CreateTextInputPassword( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, bool SupportFreeEditing, Uint32 MaxLength ) {
	UITextInput::CreateParams TextInputParams;
	TextInputParams.Parent( Parent );
	TextInputParams.PosSet( Pos );
	TextInputParams.SizeSet( Size );
	TextInputParams.Flags = Flags;
	TextInputParams.SupportFreeEditing = SupportFreeEditing;
	TextInputParams.MaxLength = MaxLength;
	UITextInputPassword * Ctrl = eeNew( UITextInputPassword, ( TextInputParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UISpinBox * UITheme::CreateSpinBox( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Float DefaultValue, bool AllowDotsInNumbers ) {
	UISpinBox::CreateParams SpinBoxParams;
	SpinBoxParams.Parent( Parent );
	SpinBoxParams.PosSet( Pos );
	SpinBoxParams.SizeSet( Size );
	SpinBoxParams.Flags = Flags;
	SpinBoxParams.DefaultValue = DefaultValue;
	SpinBoxParams.AllowDotsInNumbers = AllowDotsInNumbers;
	UISpinBox * Ctrl = eeNew( UISpinBox, ( SpinBoxParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIScrollBar * UITheme::CreateScrollBar( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, bool VerticalScrollBar ) {
	UIScrollBar::CreateParams ScrollBarParams;
	ScrollBarParams.Parent( Parent );
	ScrollBarParams.PosSet( Pos );
	ScrollBarParams.SizeSet( Size );
	ScrollBarParams.Flags = Flags;
	ScrollBarParams.VerticalScrollBar = VerticalScrollBar;
	UIScrollBar * Ctrl = eeNew( UIScrollBar, ( ScrollBarParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UISlider * UITheme::CreateSlider( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, bool VerticalSlider, bool AllowHalfSliderOut, bool ExpandBackground ) {
	UISlider::CreateParams SliderParams;
	SliderParams.Parent( Parent );
	SliderParams.PosSet( Pos );
	SliderParams.SizeSet( Size );
	SliderParams.Flags = Flags;
	SliderParams.VerticalSlider = VerticalSlider;
	SliderParams.AllowHalfSliderOut = AllowHalfSliderOut;
	SliderParams.ExpandBackground = ExpandBackground;
	UISlider * Ctrl = eeNew( UISlider, ( SliderParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIComboBox * UITheme::CreateComboBox( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 MinNumVisibleItems, bool PopUpToMainControl, UIListBox * ListBox ) {
	UIComboBox::CreateParams ComboParams;
	ComboParams.Parent( Parent );
	ComboParams.PosSet( Pos );
	ComboParams.SizeSet( Size );
	ComboParams.Flags = Flags;
	ComboParams.MinNumVisibleItems = MinNumVisibleItems;
	ComboParams.PopUpToMainControl = PopUpToMainControl;
	ComboParams.ListBox = ListBox;
	UIComboBox * Ctrl = eeNew( UIComboBox, ( ComboParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIDropDownList * UITheme::CreateDropDownList( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 MinNumVisibleItems, bool PopUpToMainControl, UIListBox * ListBox ) {
	UIDropDownList::CreateParams DDLParams;
	DDLParams.Parent( Parent );
	DDLParams.PosSet( Pos );
	DDLParams.SizeSet( Size );
	DDLParams.Flags = Flags;
	DDLParams.MinNumVisibleItems = MinNumVisibleItems;
	DDLParams.PopUpToMainControl = PopUpToMainControl;
	DDLParams.ListBox = ListBox;
	UIDropDownList * Ctrl = eeNew( UIDropDownList, ( DDLParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIListBox * UITheme::CreateListBox( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, bool SmoothScroll, Uint32 RowHeight, UI_SCROLLBAR_MODE VScrollMode, UI_SCROLLBAR_MODE HScrollMode, Recti PaddingContainer ) {
	UIListBox::CreateParams LBParams;
	LBParams.Parent( Parent );
	LBParams.PosSet( Pos );
	LBParams.SizeSet( Size );
	LBParams.Flags = Flags;
	LBParams.SmoothScroll = SmoothScroll;
	LBParams.RowHeight = RowHeight;
	LBParams.VScrollMode = VScrollMode;
	LBParams.HScrollMode = HScrollMode;
	LBParams.PaddingContainer = PaddingContainer;
	UIListBox * Ctrl = eeNew( UIListBox, ( LBParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIMenu * UITheme::CreateMenu( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 RowHeight, Recti PaddingContainer, Uint32 MinWidth, Uint32 MinSpaceForIcons, Uint32 MinRightMargin ) {
	UIMenu::CreateParams MenuParams;
	MenuParams.Parent( Parent );
	MenuParams.PosSet( Pos );
	MenuParams.SizeSet( Size );
	MenuParams.Flags = Flags;
	MenuParams.RowHeight = RowHeight;
	MenuParams.PaddingContainer = PaddingContainer;
	MenuParams.MinWidth = MinWidth;
	MenuParams.MinSpaceForIcons = MinSpaceForIcons;
	MenuParams.MinRightMargin = MinRightMargin;

	UIMenu * Ctrl = eeNew( UIMenu, ( MenuParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIPopUpMenu * UITheme::CreatePopUpMenu( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 RowHeight, Recti PaddingContainer, Uint32 MinWidth, Uint32 MinSpaceForIcons, Uint32 MinRightMargin ) {
	UIPopUpMenu::CreateParams MenuParams;
	MenuParams.Parent( Parent );
	MenuParams.PosSet( Pos );
	MenuParams.SizeSet( Size );
	MenuParams.Flags = Flags;
	MenuParams.RowHeight = RowHeight;
	MenuParams.PaddingContainer = PaddingContainer;
	MenuParams.MinWidth = MinWidth;
	MenuParams.MinSpaceForIcons = MinSpaceForIcons;
	MenuParams.MinRightMargin = MinRightMargin;
	return eeNew( UIPopUpMenu, ( MenuParams ) );
}

UIProgressBar * UITheme::CreateProgressBar( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, bool DisplayPercent, bool VerticalExpand, Vector2f MovementSpeed, Rectf FillerMargin ) {
	UIProgressBar::CreateParams PBParams;
	PBParams.Parent( Parent );
	PBParams.PosSet( Pos );
	PBParams.SizeSet( Size );
	PBParams.Flags = Flags;
	PBParams.DisplayPercent = DisplayPercent;
	PBParams.VerticalExpand = VerticalExpand;
	PBParams.MovementSpeed = MovementSpeed;
	PBParams.FillerMargin = FillerMargin;

	UIProgressBar * Ctrl = eeNew( UIProgressBar, ( PBParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIPushButton * UITheme::CreatePushButton( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, SubTexture * Icon, Int32 IconHorizontalMargin, bool IconAutoMargin ) {
	UIPushButton::CreateParams ButtonParams;
	ButtonParams.Parent( Parent );
	ButtonParams.PosSet( Pos );
	ButtonParams.SizeSet( Size );
	ButtonParams.Flags = Flags;
	ButtonParams.Icon = Icon;
	ButtonParams.IconHorizontalMargin = IconHorizontalMargin;
	ButtonParams.IconAutoMargin = IconAutoMargin;

	if ( NULL != Icon )
		ButtonParams.SetIcon( Icon );

	UIPushButton * Ctrl = eeNew( UIPushButton, ( ButtonParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UISelectButton * UITheme::CreateSelectButton( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, SubTexture * Icon, Int32 IconHorizontalMargin, bool IconAutoMargin ) {
	UIPushButton::CreateParams ButtonParams;
	ButtonParams.Parent( Parent );
	ButtonParams.PosSet( Pos );
	ButtonParams.SizeSet( Size );
	ButtonParams.Flags = Flags;
	ButtonParams.Icon = Icon;
	ButtonParams.IconHorizontalMargin = IconHorizontalMargin;
	ButtonParams.IconAutoMargin = IconAutoMargin;

	if ( NULL != Icon )
		ButtonParams.SetIcon( Icon );

	UISelectButton * Ctrl = eeNew( UISelectButton, ( ButtonParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIWinMenu * UITheme::CreateWinMenu( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 MarginBetweenButtons, Uint32 ButtonMargin, Uint32 MenuHeight, Uint32 FirstButtonMargin ) {
	UIWinMenu::CreateParams WinMenuParams;
	WinMenuParams.Parent( Parent );
	WinMenuParams.PosSet( Pos );
	WinMenuParams.SizeSet( Size );
	WinMenuParams.Flags = Flags;
	WinMenuParams.MarginBetweenButtons = MarginBetweenButtons;
	WinMenuParams.ButtonMargin = ButtonMargin;
	WinMenuParams.MenuHeight = MenuHeight;
	WinMenuParams.FirstButtonMargin = FirstButtonMargin;

	UIWinMenu * Ctrl = eeNew( UIWinMenu, ( WinMenuParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

UIWindow * UITheme::CreateWindow( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 WinFlags, Sizei MinWindowSize, Uint8 BaseAlpha ) {
	UIWindow::CreateParams WinParams;
	WinParams.Parent( Parent );
	WinParams.PosSet( Pos );
	WinParams.SizeSet( Size );
	WinParams.Flags = Flags;
	WinParams.WinFlags = WinFlags;
	WinParams.MinWindowSize = MinWindowSize;
	WinParams.BaseAlpha = BaseAlpha;
	return eeNew( UIWindow, ( WinParams ) );
}

UICommonDialog * UITheme::CreateCommonDialog( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 WinFlags, Sizei MinWindowSize, Uint8 BaseAlpha, Uint32 CDLFlags, std::string DefaultFilePattern, std::string DefaultDirectory ) {
	UICommonDialog::CreateParams DLGParams;
	DLGParams.Parent( Parent );
	DLGParams.PosSet( Pos );
	DLGParams.SizeSet( Size );
	DLGParams.Flags = Flags;
	DLGParams.WinFlags = WinFlags;
	DLGParams.MinWindowSize = MinWindowSize;
	DLGParams.BaseAlpha = BaseAlpha;
	DLGParams.DefaultDirectory = DefaultDirectory;
	DLGParams.DefaultFilePattern = DefaultFilePattern;
	DLGParams.CDLFlags = CDLFlags;
	return eeNew( UICommonDialog, ( DLGParams ) );
}

UIMessageBox * UITheme::CreateMessageBox( UI_MSGBOX_TYPE Type, const String& Message, Uint32 WinFlags, UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Sizei MinWindowSize, Uint8 BaseAlpha ) {
	UIMessageBox::CreateParams MsgBoxParams;
	MsgBoxParams.Parent( Parent );
	MsgBoxParams.PosSet( Pos );
	MsgBoxParams.SizeSet( Size );
	MsgBoxParams.Flags = Flags;
	MsgBoxParams.WinFlags = WinFlags;
	MsgBoxParams.MinWindowSize = MinWindowSize;
	MsgBoxParams.BaseAlpha = BaseAlpha;
	MsgBoxParams.Type = Type;
	MsgBoxParams.Message = Message;
	return eeNew( UIMessageBox, ( MsgBoxParams ) );
}

UITabWidget * UITheme::CreateTabWidget( UIControl *Parent, const Sizei &Size, const Vector2i &Pos, const Uint32 &Flags, const bool &TabsClosable, const bool &SpecialBorderTabs, const Int32 &TabSeparation, const Uint32 &MaxTextLength, const Uint32 &TabWidgetHeight, const Uint32 &TabTextAlign, const Uint32 &MinTabWidth, const Uint32 &MaxTabWidth ) {
	UITabWidget::CreateParams TabWidgetParams;
	TabWidgetParams.Parent( Parent );
	TabWidgetParams.PosSet( Pos );
	TabWidgetParams.SizeSet( Size );
	TabWidgetParams.Flags = Flags;
	TabWidgetParams.TabsClosable = TabsClosable;
	TabWidgetParams.SpecialBorderTabs = SpecialBorderTabs;
	TabWidgetParams.TabSeparation = TabSeparation;
	TabWidgetParams.MaxTextLength = MaxTextLength;
	TabWidgetParams.TabWidgetHeight = TabWidgetHeight;
	TabWidgetParams.TabTextAlign = TabTextAlign;
	TabWidgetParams.MinTabWidth = MinTabWidth;
	TabWidgetParams.MaxTabWidth = MaxTabWidth;

	UITabWidget * Ctrl = eeNew( UITabWidget, ( TabWidgetParams ) );
	Ctrl->Visible( true );
	Ctrl->Enabled( true );
	return Ctrl;
}

}}
