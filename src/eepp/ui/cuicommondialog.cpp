#include <eepp/ui/cuicommondialog.hpp>
#include <eepp/ui/cuimanager.hpp>
#include <eepp/ui/cuilistboxitem.hpp>
#include <eepp/ui/cuithememanager.hpp>
#include <eepp/system/filesystem.hpp>
#include <algorithm>

namespace EE { namespace UI {

#define CDLG_MIN_WIDTH 420
#define CDLG_MIN_HEIGHT 300

cUICommonDialog::cUICommonDialog( const cUICommonDialog::CreateParams& Params ) :
	cUIWindow( Params ),
	mCurPath( Params.DefaultDirectory ),
	mCDLFlags( Params.CDLFlags )
{
	if ( mSize.Width() < CDLG_MIN_WIDTH )
		mSize.x = CDLG_MIN_WIDTH;

	if ( mSize.Height() < CDLG_MIN_HEIGHT )
		mSize.y = CDLG_MIN_HEIGHT;

	if ( mMinWindowSize.Width() < CDLG_MIN_WIDTH )
		mMinWindowSize.Width( CDLG_MIN_WIDTH );

	if ( mMinWindowSize.Height() < CDLG_MIN_HEIGHT )
		mMinWindowSize.Height( CDLG_MIN_HEIGHT );

	if ( AllowFolderSelect() ) {
		Title( "Select a folder" );
	} else {
		Title( "Select a file" );
	}

	cUITextBox::CreateParams TxtBoxParams;
	TxtBoxParams.Parent( Container() );
	TxtBoxParams.PosSet( 6, 13 );
	TxtBoxParams.Flags |= UI_AUTO_SIZE;
	cUITextBox * TBox = eeNew( cUITextBox, ( TxtBoxParams ) );
	TBox->Visible( true );
	TBox->Enabled( false );
	TBox->Text( "Look in:" );

	cUIPushButton::CreateParams ButtonParams;
	ButtonParams.Flags = UI_HALIGN_CENTER | UI_ANCHOR_RIGHT | UI_VALIGN_CENTER | UI_AUTO_SIZE;
	ButtonParams.Parent( Container() );
	ButtonParams.PosSet( Container()->Size().Width() - 86, Container()->Size().Height() - 24 );
	ButtonParams.SizeSet( 80, 22 );
	mButtonCancel = eeNew( cUIPushButton, ( ButtonParams ) );
	mButtonCancel->Visible( true );
	mButtonCancel->Enabled( true );
	mButtonCancel->Text( "Cancel" );
	mButtonCancel->Pos( eeVector2i( mButtonCancel->Pos().x, Container()->Size().Height() - mButtonCancel->Size().Height() - 2 ) );
	mButtonCancel->UpdateAnchorsDistances();

	ButtonParams.PosSet( mButtonCancel->Pos().x, mButtonCancel->Pos().y - mButtonCancel->Size().Height() );
	mButtonOpen = eeNew( cUIPushButton, ( ButtonParams ) );
	mButtonOpen->Visible( true );
	mButtonOpen->Enabled( true );

	if ( IsSaveDialog() )
		mButtonOpen->Text( "Save" );
	else
		mButtonOpen->Text( "Open" );

	cUITextInput::CreateParams TInputParams;
	TInputParams.Parent( Container() );
	TInputParams.Flags = UI_AUTO_PADDING | UI_CLIP_ENABLE | UI_ANCHOR_RIGHT | UI_ANCHOR_LEFT | UI_ANCHOR_TOP | UI_VALIGN_CENTER | UI_TEXT_SELECTION_ENABLED;
	TInputParams.PosSet( 70, 6 );
	TInputParams.SizeSet( Container()->Size().Width() - TInputParams.Pos.x - 42, 22 );
	mPath = eeNew( cUITextInput, ( TInputParams ) );
	mPath->AddEventListener( cUIEvent::EventOnPressEnter, cb::Make1( this, &cUICommonDialog::OnPressEnter ) );
	mPath->Visible( true );
	mPath->Enabled( true );
	mPath->Text( mCurPath );

	ButtonParams.PosSet( TInputParams.Pos.x + TInputParams.Size.Width() + 6, TInputParams.Pos.y );
	ButtonParams.SizeSet( 24, 22 );
	ButtonParams.Flags |= UI_ANCHOR_TOP;
	mButtonUp = eeNew( cUIPushButton, ( ButtonParams ) );
	mButtonUp->Visible( true );
	mButtonUp->Enabled( true );
	mButtonUp->Text( "Up" );

	cUIListBox::CreateParams LBParams;
	LBParams.Parent( Container() );
	LBParams.PosSet( 6, mButtonUp->Pos().y + mButtonUp->Size().Height() + 4 );
	LBParams.Size = eeSize( Container()->Size().Width() - 12,
							Container()->Size().Height() -
								mButtonUp->Size().Height() -
								mButtonUp->Pos().y -
								mButtonOpen->Size().Height() -
								mButtonCancel->Size().Height() -
								8
						);

	LBParams.Flags = UI_AUTO_PADDING | UI_ANCHOR_RIGHT | UI_ANCHOR_LEFT | UI_ANCHOR_TOP | UI_ANCHOR_BOTTOM | UI_CLIP_ENABLE;
	LBParams.FontSelectedColor = ColorA( 255, 255, 255, 255 );

	if ( NULL != cUIThemeManager::instance()->DefaultTheme() ) {
		cUITheme * Theme = cUIThemeManager::instance()->DefaultTheme();

		LBParams.FontSelectedColor = Theme->FontSelectedColor();
	}

	mList = eeNew( cUIListBox, ( LBParams ) );
	mList->Visible( true );
	mList->Enabled( true );

	TxtBoxParams.PosSet( 6, Container()->Size().Height() - 54 );
	TxtBoxParams.SizeSet( 74, 19 );
	TxtBoxParams.Flags = UI_ANCHOR_LEFT | UI_VALIGN_CENTER;
	TBox = eeNew( cUITextBox, ( TxtBoxParams ) );
	TBox->Visible( true );
	TBox->Enabled( false );
	TBox->Text( "File Name:" );

	TxtBoxParams.PosSet( TBox->Pos().x, TBox->Pos().y + TBox->Size().Height()+ 6 );
	cUITextBox * TBox2 = eeNew( cUITextBox, ( TxtBoxParams ) );
	TBox2->Visible( true );
	TBox2->Enabled( false );
	TBox2->Text( "Files of type:" );

	TInputParams.Flags &= ~UI_ANCHOR_TOP;
	TInputParams.PosSet( TBox->Pos().x + TBox->Size().Width(), TBox->Pos().y );
	TInputParams.SizeSet( Container()->Size().Width() - mButtonOpen->Size().Width() - TInputParams.Pos.x - 20, TInputParams.Size.Height() );
	mFile = eeNew( cUITextInput, ( TInputParams ) );
	mFile->Visible( true );
	mFile->Enabled( true );
	mFile->AddEventListener( cUIEvent::EventOnPressEnter, cb::Make1( this, &cUICommonDialog::OnPressFileEnter ) );

	cUIDropDownList::CreateParams DDLParams;
	DDLParams.Parent( Container() );
	DDLParams.PosSet( TBox2->Pos().x + TBox2->Size().Width(), TBox2->Pos().y );
	DDLParams.SizeSet( Container()->Size().Width() - mButtonCancel->Size().Width() - DDLParams.Pos.x - 20, 22 );
	DDLParams.Flags = UI_CLIP_ENABLE | UI_AUTO_PADDING | UI_VALIGN_CENTER | UI_HALIGN_LEFT | UI_ANCHOR_LEFT | UI_ANCHOR_RIGHT | UI_AUTO_SIZE;
	DDLParams.PopUpToMainControl = true;
	mFiletype = eeNew( cUIDropDownList, ( DDLParams ) );
	mFiletype->Visible( true );
	mFiletype->Enabled( true );
	mFiletype->ListBox()->AddListBoxItem( Params.DefaultFilePattern );
	mFiletype->ListBox()->SetSelected(0);

	ApplyDefaultTheme();

	RefreshFolder();
}

cUICommonDialog::~cUICommonDialog() {
}

Uint32 cUICommonDialog::Type() const {
	return UI_TYPE_COMMONDIALOG;
}

bool cUICommonDialog::IsType( const Uint32& type ) const {
	return cUICommonDialog::Type() == type ? true : cUIWindow::IsType( type );
}

void cUICommonDialog::SetTheme( cUITheme * Theme ) {
	cUIWindow::SetTheme( Theme );

	cSubTexture * Icon = Theme->GetIconByName( "go-up" );

	if ( NULL != Icon ) {
		mButtonUp->Text( "" );
		mButtonUp->Icon( Icon );
	}
}

void cUICommonDialog::RefreshFolder() {
	std::vector<String>			flist = FileSystem::FilesGetInPath( String( mCurPath ) );
	std::vector<String>			files;
	std::vector<String>			folders;
	std::vector<std::string>	patterns;
	bool						accepted;
	Uint32 i, z;

	if ( "*" != mFiletype->Text() ) {
		patterns = String::Split( mFiletype->Text().ToUtf8(), ';' );

		for ( i = 0; i < patterns.size(); i++ )
			patterns[i] = FileSystem::FileExtension( patterns[i] );
	}

	for ( i = 0; i < flist.size(); i++ ) {
		if ( FoldersFirst() && FileSystem::IsDirectory( mCurPath + flist[i] ) ) {
			folders.push_back( flist[i] );
		} else {
			accepted = false;

			if ( patterns.size() ) {
				for ( z = 0; z < patterns.size(); z++ ) {
					if ( patterns[z] == FileSystem::FileExtension( flist[i] ) ) {
						accepted = true;
						break;
					}
				}
			} else {
				accepted = true;
			}

			if ( accepted )
				files.push_back( flist[i] );
		}
	}

	if ( SortAlphabetically() ) {
		std::sort( folders.begin(), folders.end() );
		std::sort( files.begin(), files.end() );
	}

	mList->Clear();

	if ( FoldersFirst() ) {
		mList->AddListBoxItems( folders );
	}

	mList->AddListBoxItems( files );

	if ( NULL != mList->VerticalScrollBar() ) {
		mList->VerticalScrollBar()->ClickStep( 1.f / ( ( mList->Count() * mList->RowHeight() ) / (Float)mList->Size().Height() ) );
	}
}

void cUICommonDialog::OpenSaveClick() {
	if ( IsSaveDialog() ) {
		Save();
	} else {
		Open();
	}
}

void cUICommonDialog::OnPressFileEnter( const cUIEvent * Event ) {
	OpenSaveClick();
}

void cUICommonDialog::DisableButtons() {
	mButtonOpen->Enabled( false );
	mButtonCancel->Enabled( false );
	mButtonUp->Enabled( false );

	if ( NULL != mButtonClose )
		mButtonClose->Enabled( false );

	if ( NULL != mButtonMinimize )
		mButtonMinimize->Enabled( false );

	if ( NULL != mButtonMaximize )
		mButtonMaximize->Enabled( false );
}

Uint32 cUICommonDialog::OnMessage( const cUIMessage * Msg ) {
	switch ( Msg->Msg() ) {
		case cUIMessage::MsgClick:
		{
			if ( Msg->Flags() & EE_BUTTON_LMASK ) {
				if ( Msg->Sender() == mButtonOpen ) {
					OpenSaveClick();
				} else if ( Msg->Sender() == mButtonCancel ) {
					DisableButtons();

					CloseWindow();
				} else if ( Msg->Sender() == mButtonUp ) {
					mCurPath = FileSystem::RemoveLastFolderFromPath( mCurPath );
					mPath->Text( mCurPath );
					RefreshFolder();
				}
			}

			break;
		}
		case cUIMessage::MsgDoubleClick:
		{
			if ( Msg->Flags() & EE_BUTTON_LMASK ) {
				if ( Msg->Sender()->IsType( UI_TYPE_LISTBOXITEM ) ) {
					std::string newPath = mCurPath + mList->GetItemSelectedText();

					if ( FileSystem::IsDirectory( newPath ) ) {
						mCurPath = newPath + FileSystem::GetOSlash();
						mPath->Text( mCurPath );
						RefreshFolder();
					} else {
						Open();
					}
				}
			}

			break;
		}
		case cUIMessage::MsgSelected:
		{
			if ( Msg->Sender() == mList ) {
				if ( !IsSaveDialog() ) {
					if ( AllowFolderSelect() ) {
						mFile->Text( mList->GetItemSelectedText() );
					} else {
						if ( !FileSystem::IsDirectory( GetTempFullPath() ) ) {
							mFile->Text( mList->GetItemSelectedText() );
						}
					}
				} else {
					if ( !FileSystem::IsDirectory( GetTempFullPath() ) ) {
						mFile->Text( mList->GetItemSelectedText() );
					}
				}
			} else if ( Msg->Sender() == mFiletype ) {
				RefreshFolder();
			}

			break;
		}
	}

	return cUIWindow::OnMessage( Msg );
}

void cUICommonDialog::Save() {
	SendCommonEvent( cUIEvent::EventSaveFile );

	DisableButtons();

	CloseWindow();
}

void cUICommonDialog::Open() {
	if ( "" != mList->GetItemSelectedText() || AllowFolderSelect() ) {
		if ( !AllowFolderSelect() ) {
			if ( FileSystem::IsDirectory( GetFullPath() ) )
				return;
		} else {
			if ( !FileSystem::IsDirectory( GetFullPath() ) && !FileSystem::IsDirectory( GetCurPath() ) )
				return;
		}

		SendCommonEvent( cUIEvent::EventOpenFile );

		DisableButtons();

		CloseWindow();
	}
}

void cUICommonDialog::OnPressEnter( const cUIEvent * Event ) {
	if ( FileSystem::IsDirectory( mPath->Text() ) ) {
		std::string tpath = mPath->Text();
		FileSystem::DirPathAddSlashAtEnd( tpath );
		mPath->Text( tpath );
		mCurPath = mPath->Text();
		RefreshFolder();
	}
}

void cUICommonDialog::AddFilePattern( std::string pattern, bool select ) {
	Uint32 index = mFiletype->ListBox()->AddListBoxItem( pattern );

	if ( select ) {
		mFiletype->ListBox()->SetSelected( index );

		RefreshFolder();
	}
}

bool cUICommonDialog::IsSaveDialog() {
	return 0 != ( mCDLFlags & CDL_FLAG_SAVE_DIALOG );
}

bool cUICommonDialog::SortAlphabetically() {
	return 0 != ( mCDLFlags & CDL_FLAG_SORT_ALPHABETICALLY );
}

bool cUICommonDialog::FoldersFirst() {
	return 0 != ( mCDLFlags & CDL_FLAG_FOLDERS_FISRT );
}

bool cUICommonDialog::AllowFolderSelect() {
	return 0 != ( mCDLFlags & CDL_FLAG_ALLOW_FOLDER_SELECT );
}

void cUICommonDialog::SortAlphabetically( const bool& sortAlphabetically ) {
	BitOp::SetBitFlagValue( &mCDLFlags, CDL_FLAG_SORT_ALPHABETICALLY, sortAlphabetically ? 1 : 0 );
	RefreshFolder();
}

void cUICommonDialog::FoldersFirst( const bool& foldersFirst ) {
	BitOp::SetBitFlagValue( &mCDLFlags, CDL_FLAG_FOLDERS_FISRT , foldersFirst ? 1 : 0 );
	RefreshFolder();
}

void cUICommonDialog::AllowFolderSelect( const bool& allowFolderSelect ) {
	BitOp::SetBitFlagValue( &mCDLFlags, CDL_FLAG_ALLOW_FOLDER_SELECT, allowFolderSelect ? 1 : 0 );
}

std::string cUICommonDialog::GetFullPath() {
	std::string tPath = mCurPath;

	FileSystem::DirPathAddSlashAtEnd( tPath );

	tPath += GetCurFile();

	return tPath;
}

std::string	cUICommonDialog::GetTempFullPath() {
	std::string tPath = mCurPath;

	FileSystem::DirPathAddSlashAtEnd( tPath );

	tPath += mList->GetItemSelectedText().ToUtf8();

	return tPath;
}

std::string cUICommonDialog::GetCurPath() const {
	return mCurPath;
}

std::string cUICommonDialog::GetCurFile() const {
	if ( mCDLFlags & CDL_FLAG_SAVE_DIALOG )
		return mFile->Text();

	return mList->GetItemSelectedText().ToUtf8();
}

cUIPushButton *	cUICommonDialog::GetButtonOpen() const {
	return mButtonOpen;
}

cUIPushButton *	cUICommonDialog::GetButtonCancel() const {
	return mButtonCancel;
}

cUIPushButton *	cUICommonDialog::GetButtonUp() const {
	return mButtonUp;
}

cUIListBox * cUICommonDialog::GetList() const {
	return mList;
}

cUITextInput * cUICommonDialog::GetPathInput() const {
	return mPath;
}

cUITextInput * cUICommonDialog::GetFileInput() const {
	return mFile;
}

cUIDropDownList * cUICommonDialog::GetFiletypeList() const {
	return mFiletype;
}

}}
