#include <eepp/ee.hpp>

EE::Window::Window * win			= NULL;
TTFFont * TTF			= NULL;
TTFFont * TTFO			= NULL;
TTFFont * TTF2			= NULL;
TextureFont * TexF		= NULL;
TextureFont * TexF2	= NULL;
TextCache * TxtCache	= NULL;

void MainLoop()
{
	// Clear the screen buffer
	win->clear();

	// Update the input
	win->getInput()->update();

	// Check if ESCAPE key is pressed
	if ( win->getInput()->isKeyDown( KEY_ESCAPE ) ) {
		// Close the window
		win->close();
	}

	Float YPos = 32;

	// Draw the text on screen
	TTF->Draw( win->getWidth() * 0.5f - TTF->GetTextWidth() * 0.5f, YPos );

	TTFO->Draw( win->getWidth() * 0.5f - TTFO->GetTextWidth() * 0.5f, ( YPos += TTF->GetTextHeight() + 24 ) );

	TTF2->Draw( win->getWidth() * 0.5f - TTF2->GetTextWidth() * 0.5f, ( YPos += TTF->GetTextHeight() + 24 ) );

	TexF->Draw( win->getWidth() * 0.5f - TexF->GetTextWidth() * 0.5f, ( YPos += TTF2->GetTextHeight() + 24 ) );

	TexF2->Draw( win->getWidth() * 0.5f - TexF2->GetTextWidth() * 0.5f, ( YPos += TexF->GetTextHeight() + 24 ) );

	// Draw the cached text
	TxtCache->Draw( 48, ( YPos += TexF2->GetTextHeight() + 24 ) );

	// Text rotated and scaled
	TTF->Draw( win->getWidth() * 0.5f - TTF->GetTextWidth() * 0.5f, 512, FONT_DRAW_LEFT, Vector2f( 0.75f, 0.75f ), 12.5f );

	// Draw frame
	win->display();
}

EE_MAIN_FUNC int main (int argc, char * argv [])
{
	// Create a new window
	win = Engine::instance()->createWindow( WindowSettings( 960, 640, "eepp - Fonts" ), ContextSettings( true ) );

	// Set window background color
	win->backColor( RGB(255,255,255) );

	// Check if created
	if ( win->created() ) {
		// Get the application path
		std::string AppPath = Sys::getProcessPath();

		// Create a new True Type Font
		TTF		= TTFFont::New( "DejaVuSansMonoOutline" );
		TTFO	= TTFFont::New( "DejaVuSansMonoOutlineFreetype" );
		TTF2	= TTFFont::New( "DejaVuSansMono" );
		TexF	= TextureFont::New( "ProggySquareSZ" );
		TexF2	= TextureFont::New( "conchars" );

		// Load the TTF font
		TTF->Load( AppPath + "assets/fonts/DejaVuSansMono.ttf", 18, TTF_STYLE_NORMAL, 128, RGB(255,255,255), 3, RGB(0,0,0), true );

		// Change the default method to use for outlining the font glyphs
		TTFFont::OutlineMethod = TTFFont::OutlineFreetype;

		// Create the exact same font than before but using the new outlining method
		TTFO->Load( AppPath + "assets/fonts/DejaVuSansMono.ttf", 18, TTF_STYLE_NORMAL, 128, RGB(255,255,255), 3, RGB(0,0,0), true );

		TTF2->Load( AppPath + "assets/fonts/DejaVuSansMono.ttf", 24, TTF_STYLE_NORMAL, 128, RGB(255,255,255), 0, RGB(0,0,0), true );

		// Save the TTF font so then it can be loaded as a TextureFont
		TTF->Save( AppPath + "assets/temp/DejaVuSansMono.png", AppPath + "assets/temp/DejaVuSansMono.fnt" );

		// Load the texture font, previusly generated from a True Type Font
		// First load the texture
		Uint32 TexFid = TextureFactory::instance()->Load( AppPath + "assets/fonts/ProggySquareSZ.png" );
		TexF->Load( TexFid,  AppPath + "assets/fonts/ProggySquareSZ.dat" );

		// Load a monospaced texture font from image ( using the texture loader to set the color key )
		TextureLoader TexLoader( AppPath + "assets/fonts/conchars.png" );
		TexLoader.SetColorKey( RGB(0,0,0) );
		TexLoader.load();;
		TexF2->Load( TexLoader.Id(), 32 );

		// Set a text to render
		TTF->SetText( "Lorem ipsum dolor sit amet, consectetur adipisicing elit." );
		TTFO->SetText( TTF->GetText() );
		TTF2->SetText( TTF->GetText() );
		TexF->SetText( TTF->GetText() );
		TexF2->SetText( TTF->GetText() );

		// Set the font color
		TTF2->Color( RGB(0,0,0) );
		TexF->Color( RGB(0,0,0) );

		// Create a new text string
		String Txt( "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum." );

		// Make the text fit the screen width ( wrap the text )
		TTF2->ShrinkText( Txt, win->getWidth() - 96 );

		// Create a new text cache to draw on screen
		// The cached text will
		TxtCache = eeNew( TextCache, ( TTF2, Txt, ColorA(0,0,0,255) ) );

		// Set the text cache to be centered
		TxtCache->Flags( FONT_DRAW_CENTER );

		// Set the font color to a substring of the text
		// To be able to set the color of the font, create the font as white
		// Create a gradient
		size_t size = TxtCache->Text().size();

		for ( size_t i = 0; i < size; i++ ) {
			TxtCache->Color( ColorA(255*i/size,0,0,255), i, i+1 );
		}

		// Application loop
		win->runMainLoop( &MainLoop );
	}

	eeSAFE_DELETE( TxtCache );

	// Destroy the engine instance. Destroys all the windows and engine singletons.
	// Fonts are autoreleased by the engine
	Engine::destroySingleton();

	// If was compiled in debug mode it will print the memory manager report
	MemoryManager::showResults();

	return EXIT_SUCCESS;
}
