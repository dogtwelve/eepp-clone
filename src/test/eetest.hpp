#ifndef EE_EETEST_HPP
#define EE_EETEST_HPP

#include <eepp/ee.hpp>

namespace Demo_Test {

class UITest : public UIControlAnim {
	public:
		UITest( UIControlAnim::CreateParams& Params ) : UIControlAnim( Params ) 	{ mOldColor = mBackground->getColors(); }

		virtual Uint32 onMouseEnter( const Vector2i& Pos, const Uint32 Flags )	{
			if ( 4 == mOldColor.size() ) {
				mBackground->setColors( ColorA( mOldColor[0].r(), mOldColor[0].g(), mOldColor[0].b(), 200 ),
									ColorA( mOldColor[1].r(), mOldColor[1].g(), mOldColor[1].b(), 200 ),
									ColorA( mOldColor[2].r(), mOldColor[2].g(), mOldColor[2].b(), 200 ),
									ColorA( mOldColor[3].r(), mOldColor[3].g(), mOldColor[3].b(), 200 )
								);
			} else {
				mBackground->setColor( ColorA( mOldColor[0].r(), mOldColor[0].g(), mOldColor[0].b(), 200 ) );
			}

			return 1;
		}

		virtual Uint32 onMouseExit( const Vector2i& Pos, const Uint32 Flags )	{
			if ( 4 == mOldColor.size() ) {
				mBackground->setColors( mOldColor[0], mOldColor[1], mOldColor[2], mOldColor[3] );
			} else {
				mBackground->setColor( mOldColor[0] );
			}

			return 1;
		}

		virtual Uint32 onMouseUp( const Vector2i& Pos, const Uint32 Flags ) {
			UIDragable::onMouseUp( Pos, Flags );

			if ( Engine::instance()->getCurrentWindow()->getInput()->mouseWheelScrolledUp() )
				setScale( getScale() + 0.1f );
			else if ( Engine::instance()->getCurrentWindow()->getInput()->mouseWheelScrolledDown() )
				setScale( getScale() - 0.1f );

			return 1;
		}

		virtual Uint32 onFocus() {
			toFront();

			return 1;
		}

		const std::vector<ColorA>& OldColor() { return mOldColor; }
	protected:
		std::vector<ColorA> mOldColor;
};

enum CollisionTypes {
	BALL_TYPE,
	BLOCKING_SENSOR_TYPE,
	CATCH_SENSOR_TYPE
};

struct Emitter {
	int queue;
	int blocked;
	cVect position;
};

class EETest : private Thread {
	public:
		typedef cb::Callback0<void> SceneCb;

		void init();
		void update();
		void end();
		void process();
		void render();
		void input();
		void particlesCallback(Particle* P, ParticleSystem* Me);

		void particlesThread();
		void particles();
		void updateParticles();
		void loadTextures();
		void cmdSetPartsNum ( const std::vector < String >& params );

		Clock cElapsed;
		Time PSElapsed;
	private:
		Engine * EE;
		EE::Window::Window * mWindow;
		TextureFactory* TF;
		System::Log* Log;
		EE::Window::Input* KM;
		InputTextBuffer InBuf;

		bool side, aside;
		Float ang, scale, alpha, Ang;
		Time et;
		Int32 x, y;
		Uint32 lasttick;

		std::vector<Uint32> TN;
		std::vector<Texture *> TNP;

		std::vector<SubTexture*> Tiles;
		std::vector<ParticleSystem> PS;

		Vector2i Mouse;
		Vector2f Mousef;

		Sprite SP;
		Sprite CL1, CL2;
		Font * FF;
		Font * FF2;
		Font * TTF;
		Font * TTFB;

		Primitives PR;
		bool iL1, iL2;
		Float HWidth, HHeight;

		Music * Mus;
		SoundManager SndMng;

		bool DrawBack;

		Console Con;
		virtual void run();

		Vector2f Point;

		std::string MyPath;
		bool ShowParticles;

		TileMap Map;

		Uint8 Screen;
		SceneCb Scenes[6];
		void screen1();
		void screen2();
		void screen3();
		void screen4();
		void screen5();

		Zip * PakTest;

		std::vector<Uint8> tmpv;
		std::vector<Uint8> MySong;

		Waypoints WP;
		Int32 PartsNum;
		Uint32 Cursor[1];
		Texture * CursorP[1];
		std::string mInfo;

		bool MultiViewportMode;

		BatchRenderer Batch;
		Float AnimVal;
		bool AnimSide;

		View Views[2];

		ShaderProgram * mShaderProgram;

		Float mBlurFactor;
		bool mUseShaders;
		bool mJoyEnabled;
		bool mMusEnabled;

		Uint32 mLastFPSLimit;
		bool mWasMinimized;

		String mBuda;

		ResourceLoader mResLoad;
		void onTextureLoaded( ResourceLoader * ObjLoaded );

		void createUI();
		void createShaders();

		void loadFonts();

		ResourceLoader mFontLoader;
		void onFontLoaded( ResourceLoader * ObjLoaded );

		JoystickManager * JM;
		Float mAxisX;
		Float mAxisY;

		TextureAtlasLoader * mTGL;
		Sprite mBlindy;

		FrameBuffer * mFBO;
		VertexBuffer * mVBO;

		Clock	mFTE;

		void createCommonDialog();
		void onItemClick( const UIEvent * Event );
		void onMainClick( const UIEvent * Event );
		void onQuitClick( const UIEvent * Event );
		void onCloseClick( const UIEvent * Event );
		void onButtonClick( const UIEvent * Event );
		void onValueChange( const UIEvent * Event );
		void onSliderValueChange( const UIEvent * Event );
		void onWinMouseUp( const UIEvent * Event );
		void createDecoratedWindow();
		void createWinMenu();
		void createUIThemeTextureAtlas();

		UIControlAnim * C;
		UIScrollBar * mScrollBar;
		UITextBox * mTextBoxValue;
		UISlider * mSlider;
		UIProgressBar * mProgressBar;
		UIListBox * mListBox;
		UIPopUpMenu * Menu;
		UIWindow * mUIWindow;
		MapEditor * mMapEditor;
		TextureAtlasEditor * mETGEditor;

		TextCache mEEText;
		TextCache mFBOText;
		TextCache mInfoText;

		std::string mThemeName;

		Space * mSpace;

		#ifndef EE_PLATFORM_TOUCH
		Body * mMouseBody;
		cVect mMousePoint;
		cVect mMousePoint_last;
		Constraint * mMouseJoint;
		#else
		Body * mMouseBody[ EE_MAX_FINGERS ];
		cVect mMousePoint[ EE_MAX_FINGERS ];
		cVect mMousePoint_last[ EE_MAX_FINGERS ];
		Constraint * mMouseJoint[ EE_MAX_FINGERS ];
		#endif

		void physicsCreate();
		void physicsUpdate();
		void physicsDestroy();

		void setScreen( Uint32 num );

		cpBool blockerBegin( Arbiter *arb, Space *space, void *unused );
		void blockerSeparate( Arbiter *arb, Space *space, void *unused );
		void postStepRemove( Space *space, void * tshape, void *unused );
		cpBool catcherBarBegin( Arbiter *arb, Space *space, void *unused );

		void demo1Create();
		void demo1Update();
		void demo1Destroy();

		void demo2Create();
		void demo2Update();
		void demo2Destroy();

		void showMenu();

		Emitter emitterInstance;

		void changeDemo( Uint32 num );

		struct physicDemo {
			SceneCb init;
			SceneCb update;
			SceneCb destroy;
		};

		std::vector<physicDemo> mDemo;
		Uint32					mCurDemo;
		Sprite *				mBoxSprite;
		Sprite *				mCircleSprite;

		UITheme *				mTheme;

		bool					mTerrainUp;
		UIPushButton *			mShowMenu;
		UIPushButton *			mTerrainBut;

		void createMapEditor();

		void onMapEditorClose();

		void onETGEditorClose();

		void createETGEditor();

		void createJointAndBody();

		void destroyBody();

		void onShowMenu( const UIEvent * Event );

		void onWindowResize( EE::Window::Window * win );
};

}
#endif

