#include <eepp/ee.hpp>

EE::Window::Window * win = NULL;

// Define a interpolation to control the Rock sprite angle
Interpolation RockAngle;

Interpolation PlanetAngle;

// Create a primitive drawer instance to draw the AABB of the Rock
Primitives P;
Sprite * Rock		= NULL;
Sprite * Planet	= NULL;
Sprite * Blindy	= NULL;

// Define a user sprite event
static const Uint32 USER_SPRITE_EVENT = Sprite::SPRITE_EVENT_USER + 1;

// Get the sprite event callback
void spriteCallback( Uint32 Event, Sprite * Sprite, void * UserData ) {
	// Sprite Animation entered the first frame?
	if ( Event == Sprite::SPRITE_EVENT_FIRST_FRAME ) {
		// Fire a user Event
		Sprite->FireEvent( USER_SPRITE_EVENT );
	} else if ( Event == USER_SPRITE_EVENT ) {
		// Create an interpolation to change the angle of the sprite
		Interpolation * RotationInterpolation = reinterpret_cast<Interpolation*>( UserData );
		RotationInterpolation->ClearWaypoints();
		RotationInterpolation->AddWaypoint( Sprite->Angle() );
		RotationInterpolation->AddWaypoint( Sprite->Angle() + 45.f );
		RotationInterpolation->SetTotalTime( Milliseconds( 500 ) );
		RotationInterpolation->Type( Ease::BounceOut ); // Set the easing effect used for the interpolation
		RotationInterpolation->Start();

		// Scale the sprite
		if ( Sprite->Scale().x < 3 ) {
			Sprite->Scale( Sprite->Scale() + 0.25f );
		}
	}
}

void MainLoop()
{
	// Clear the screen buffer
	win->Clear();

	// Update the input
	win->GetInput()->Update();

	// Check if ESCAPE key is pressed
	if ( win->GetInput()->IsKeyDown( KEY_ESCAPE ) ) {
		// Close the window
		win->Close();
	}

	// Check if the D key was pressed
	if ( win->GetInput()->IsKeyUp( KEY_D ) ) {
		// Reverse the Rock animation
		Rock->ReverseAnim( !Rock->ReverseAnim() );
	}

	// Update the angle interpolation
	PlanetAngle.Update( win->Elapsed() );
	RockAngle.Update( win->Elapsed() );

	// Set the Planet and Rock angle from the interpolation
	Planet->Angle( PlanetAngle.GetPos() );
	Rock->Angle( RockAngle.GetPos() );

	// Draw the static planet sprite
	Planet->Draw();

	// Draw the animated Rock sprite
	Rock->Draw();

	// Draw the blindy animation
	Blindy->Draw();

	// Draw the Rock Axis-Aligned Bounding Box
	P.SetColor( ColorA( 255, 255, 255, 255 ) );
	P.DrawRectangle( Rock->GetAABB() );

	// Draw the Rock Quad
	P.SetColor( ColorA( 255, 0, 0, 255 ) );
	P.DrawQuad( Rock->GetQuad() );

	// Draw frame
	win->Display();
}

EE_MAIN_FUNC int main (int argc, char * argv [])
{
	// Create a new window
	win = Engine::instance()->CreateWindow( WindowSettings( 640, 480, "eepp - Sprites" ), ContextSettings( true ) );

	// Check if created
	if ( win->Created() ) {
		// Get the application path
		std::string AppPath = Sys::GetProcessPath();

		// Load the rock texture
		Uint32 PlanetId	= TextureFactory::instance()->Load( AppPath + "assets/sprites/7.png" );
		Uint32 RockId	= TextureFactory::instance()->Load( AppPath + "assets/sprites/5.png" );

		// Load a previously generated texture atlas that contains the SubTextures needed to load an animated sprite
		TextureAtlasLoader Blindies( AppPath + "assets/atlases/bnb.eta" );

		// Create the animated rock spriteR
		Rock	= eeNew( Sprite, () );

		// Load the rock frames from the texture, adding the frames manually
		for ( Int32 my = 0; my < 4; my++ ) {
			for( Int32 mx = 0; mx < 8; mx++ ) {
				// DestSize as 0,0 will use the SubTexture size
				Rock->AddFrame( RockId, Sizef( 0, 0 ), Vector2i( 0, 0 ), Recti( mx * 64, my * 64, mx * 64 + 64, my * 64 + 64 ) );
			}
		}

		// Create a static sprite
		Planet	= eeNew( Sprite, ( PlanetId ) );

		// This constructor is the same that creating sprite and calling Sprite.AddFramesByPattern.
		// It will look for a SubTexture ( in any Texture Atlas loaded, or the GlobalTextureAtlas ) animation by its name, it will search
		// for "gn00" to "gnXX" to create a new animation
		// see TextureAtlasManager::GetSubTexturesByPattern for more information.
		// This is the easiest way to load animated sprites.
		Blindy	= eeNew( Sprite, ( "gn" ) );

		// Set the sprite animation speed, set in Frames per Second
		// Sprites are auto-animated by default.
		Rock->AnimSpeed( 32 );

		// Set the render mode of the sprite
		Blindy->RenderMode( RN_MIRROR );

		// Set the Blend Mode of the sprite
		Blindy->BlendMode( ALPHA_BLENDONE );

		// Set the primitive fill mode
		P.FillMode( DRAW_LINE );

		// Set the sprites position to the screen center
		Vector2i ScreenCenter( Engine::instance()->GetWidth() / 2, Engine::instance()->GetHeight() / 2 );

		Planet->Position( ScreenCenter.x - Planet->GetAABB().Size().Width() / 2, ScreenCenter.y - Planet->GetAABB().Size().Height() / 2 );

		Rock->Position( ScreenCenter.x - Rock->GetAABB().Size().Width() / 2, ScreenCenter.y - Rock->GetAABB().Size().Height() / 2 );

		Blindy->Position( ScreenCenter.x - Blindy->GetAABB().Size().Width() / 2, ScreenCenter.y - Blindy->GetAABB().Size().Height() / 2 );

		// Set the planet angle interpolation
		PlanetAngle.AddWaypoint( 0 );
		PlanetAngle.AddWaypoint( 360 );
		PlanetAngle.SetTotalTime( Seconds( 10 ) );
		PlanetAngle.Loop( true );
		PlanetAngle.Start();

		// Create a Event callback for the rock sprite
		Rock->SetEventsCallback( cb::Make3( &spriteCallback ), &RockAngle );

		// Application loop
		win->RunMainLoop( &MainLoop );
	}

	eeSAFE_DELETE( Rock );
	eeSAFE_DELETE( Planet );
	eeSAFE_DELETE( Blindy );

	// Destroy the engine instance. Destroys all the windows and engine singletons.
	Engine::DestroySingleton();

	// If was compiled in debug mode it will print the memory manager report
	MemoryManager::ShowResults();

	return EXIT_SUCCESS;
}
