//speedrace_main.cpp
//このソースファイルはWinMainを含むメインとなるソースが含まれています。

#include "speedrace_head1.h"
#include "CarClass.h"
#include "ConFps.h"
#include "MapClass.h"
#include "RaceClass.h"
#include "ModelClass.h"
#include "MenuClass.h"
#include "PartMenu.h"
#include "MenuBack.h"
#include "CourseInfo.h"
#include "OpeningClass.h"
#include "CreditClass.h"
#include "Global.h"

/*--メイン関数--*/

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	// 多重起動のチェック
	HANDLE hMutex = CreateMutex(NULL, FALSE, kWINDOW_TITLE);
	DWORD theErr = GetLastError();
	ShowCursor(false);
	if(theErr == ERROR_ALREADY_EXISTS)
	{
		// 多重起動している
		if(hMutex) CloseHandle(hMutex);
		MessageBox( NULL, TEXT("このアプリケーションは多重起動できません。"), kERROR_MSG, MB_OK|MB_ICONHAND );
		return( E_FAIL );
	}

	srand( (unsigned)time( NULL ) );
	using namespace std;

	// ウィンドウクラスの設定
	WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
						GetModuleHandle(NULL), NULL, NULL, NULL, NULL, kWINDOW_TITLE, NULL };

	if( !RegisterClassEx( &wc ) ){ return(FALSE);}//コケたら帰る

	// ウィンドウの作成
	HWND	hWnd;

	//最大化ボタンを持たない・境界変更のできないwindow
	DWORD	dwWindowStyle= ~(WS_MAXIMIZEBOX | WS_THICKFRAME) & WS_OVERLAPPEDWINDOW;

	hWnd = CreateWindow(	kWINDOW_TITLE, kWINDOW_TITLE, 
							dwWindowStyle, kWINDOW_LEFT, kWINDOW_TOP, kWINDOW_LEFT + kWINDOW_WIDTH, kWINDOW_TOP + kWINDOW_HEIGHT,
							GetDesktopWindow(), NULL, wc.hInstance, NULL );


	// Direct3Dより先に初期化すると、フルスクリーンモードのときエラーダイアログが見えなくなるため
	if(FAILED(InitDirectInput(hWnd,hInst)))
	{
		ZeroMemory( g_pKeyState, sizeof(g_pKeyState) ); //念のためキーボードステータスバッファを初期化
		MessageBox( NULL, TEXT("DirectInputの初期化に失敗しました。"), kERROR_MSG, MB_OK|MB_ICONHAND );
		CleanUpApplication( &wc );
		return( E_FAIL );
	}

	// Direct3Dの初期化
	if(FAILED(InitDirect3D(hWnd)))
	{
		MessageBox( NULL, TEXT("DirectXの初期化に失敗しました。"), kERROR_MSG, MB_OK|MB_ICONHAND );
		CleanUpApplication( &wc );
		return( E_FAIL );
	}
			
//	// シーンのジオメトリを生成
//	if((FAILED( SetupModels( g_pD3DDevice ) ))/*||(FAILED( SetupBillbords( g_pD3DDevice ) ))||(FAILED( SetupFVFTexture( g_pD3DDevice ) ))*/)
//	{
//		MessageBox( NULL, TEXT("モデルの読み込みに失敗しました。"), kERROR_MSG, MB_OK|MB_ICONHAND );
//		CleanUpApplication( &wc );
//		return( E_FAIL );
//	}

	// ウィンドウの表示
	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	// タイマをセットする。
	SetTimer( hWnd, kTIMER_ID, kTIMER_LAP , NULL );

	// メッセージループ
	MsgLoop( hWnd );

	// 本来ウィンドウに関連付けられたタイマはウィンドウの破棄と同時にOSが自動的に破棄するが
	// ここでは念のためアプリケーション側で破棄している
	KillTimer( hWnd, kTIMER_ID );	

	// 後始末とアプリケーションの終了
	CleanUpApplication( &wc );								// DirectXの後始末
	if(hMutex){ CloseHandle(hMutex); }						// Mutexの開放
	return( S_OK );
}


/*--メッセージループ--*/

HRESULT MsgLoop( HWND hWnd )
{
	// メッセージループ
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while( msg.message != WM_QUIT )
	{
		// イベントの取得
		// PM_NOREMOVEとするのは、Window9x/Me系OSで
		// プロセスがゾンビ化するのを防ぐため。
		if( PeekMessage (&msg,NULL,0,0,PM_NOREMOVE) ) 
		{
			// ここでイベントを取得する。
			if( !GetMessage (&msg,NULL,0,0) ){ msg.message = WM_QUIT; }	// 終了
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{ Sleep(kSLEEP); }	//CPU HUG解消のため。
	}

	return( S_OK );
}


/*--アプリケーションウィンドウの後始末--*/

VOID CleanUpApplication( WNDCLASSEX *theWC )
{
	SAFE_DELETE(theMenu);
	SAFE_DELETE(theRace);
	SAFE_DELETE(theResult);
	SAFE_DELETE(theOpening);
	SAFE_DELETE(theCredit);
	delete CFps;
	CleanUpDirectInput();									//DirectInputの後始末
	CleanUpDirect3D();										//D3DXなどの後始末

	UnregisterClass( kWINDOW_TITLE , theWC->hInstance );	//ウィンドウの破棄

	return;
}



/*--アプリケーションウィンドウのメッセージハンドラ--*/

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static BOOL sWinActivFlag = FALSE;
	static BOOL sMsgBoxFlag = FALSE;

	switch( msg )
	{
		//escキーを押したときの自沈処理
        case WM_KEYUP: 
			if(VK_ESCAPE == (int)wParam)
			{
				// フルスクリーンモードの場合はいきなり終わる
				if( g_FullScreenFlag )
				{ PostQuitMessage( 0 ); return (0); }
				else 
				{
					sMsgBoxFlag = TRUE;
					//確認ダイアログが出ている場合は処理を中断する
					if( MessageBoxEx(	hWnd, TEXT("中断しますか？"), kWINDOW_TITLE,
										MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ,LANG_JAPANESE) == IDYES )
					{ PostQuitMessage( 0 ); return (0); }
					sMsgBoxFlag = FALSE;

				}
			} 
		break; 

		case WM_DESTROY:
			PostQuitMessage( 0 );//ウィンドウ破棄時の自沈処理
			return (0);
		break;

		case WM_ACTIVATE:		//ウィンドウが再びアクティブになりました
			if( WA_INACTIVE != wParam)
			{
				// ウィンドウアクティブフラグをtrueに
				sWinActivFlag = TRUE;
			}
			else if( wParam == WA_INACTIVE )
			{
				// もしウィンドウが背後にまわったら処理を中断
				sWinActivFlag = FALSE;
			}
		break;

		case WM_TIMER:					//タイマ割り込み
			if( !sMsgBoxFlag && sWinActivFlag )
			{
				// ウィンドウがアクティブな場合だけ、ゲーム本体の処理を行う。
				//g_FPS = GetFPS();
				if(FAILED(GameBody( hWnd )))
				{
					PostQuitMessage(0);
					return(0);
				}
				CFps->Update();
			}
		break;

		default:
		break;
	}

	return( DefWindowProc( hWnd, msg, wParam, lParam ) );
}


/*--Direct3Dの初期化--*/

HRESULT InitDirect3D( HWND hWnd )
{
	// D3Dオブジェクトの生成。
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ){ return (E_FAIL); }

	// D3Dディバイスを生成するための構造体を設定する。
	ZeroMemory( &g_D3DPParams, sizeof(D3DPRESENT_PARAMETERS) );
	
	// プレゼンテーションパラメータの設定
	if( g_FullScreenFlag )
	{
		//現在のプライマリディスプレイアダプタのモードを取得する
		D3DDISPLAYMODE aDisplayMode;		//ディスプレイモードの情報を収めるため

		if(FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &aDisplayMode )))
		{
			MessageBox(NULL,TEXT("ディスプレイモードの取得に失敗しました"),kWINDOW_TITLE,MB_OK | MB_ICONSTOP);
			return( E_FAIL ); 
		}

		g_D3DPParams.BackBufferHeight			= aDisplayMode.Height;			// バックバッファの高さ
		g_D3DPParams.BackBufferWidth			= aDisplayMode.Width;			// バックバッファの幅

		g_D3DPParams.BackBufferFormat			= aDisplayMode.Format;			// バックバッファのフォーマット

		g_D3DPParams.Windowed					= FALSE;						// フルスクリーンモード
	}
	else
	{
		g_D3DPParams.BackBufferFormat			= D3DFMT_UNKNOWN;				// フォーマットは今の画面モードに従う

		g_D3DPParams.Windowed					= TRUE;							// ウィンドウモード
	}

	//共通パラメータ
	g_D3DPParams.BackBufferCount			= 1;						// バックバッファの数

	g_D3DPParams.MultiSampleType			= D3DMULTISAMPLE_NONE;		// マルチサンプリングは行わない
	g_D3DPParams.MultiSampleQuality			= 0;						// マルチサンプリングは行わないので0

	g_D3DPParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// Direct3Dにスワップエフェクトを任せる

	g_D3DPParams.EnableAutoDepthStencil		= TRUE;						// Direct3Dに深度バッファの管理を任せる
	g_D3DPParams.AutoDepthStencilFormat		= D3DFMT_D16;				// 深度バッファのフォーマット（通常はこの値で問題ない）

	g_D3DPParams.hDeviceWindow				= hWnd;						// カバーウィンドウ＝アプリケーションのウィンドウ

	g_D3DPParams.Flags						= 0;						// フラグは使わない

	g_D3DPParams.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	// 今のリフレッシュレートをそのまま使う

	g_D3DPParams.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	// モニタの垂直回帰を待つ

	// CreateDeviceでDirect3Dを初期化するとき、HAL、HELの組み合わせはプログラマが自由に決めることができるが
	// 普通はこの組み合わせで充分

	//　D3Dディバイスの生成。まず、HALのいいやつ
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, hWnd,
										D3DCREATE_HARDWARE_VERTEXPROCESSING,&g_D3DPParams, &g_pD3DDevice ) ) )
	{
		//コケたら普通のHAL(HEL?)
		if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,&g_D3DPParams, &g_pD3DDevice ) ) )
		{
			//根性でREF
			if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_REF, hWnd,
												D3DCREATE_SOFTWARE_VERTEXPROCESSING,&g_D3DPParams, &g_pD3DDevice ) ) )
			{
				//それでもコケたらおしまい。
				MessageBox(NULL,TEXT("Direct3Dを初期化できませんでした"),kWINDOW_TITLE,MB_OK | MB_ICONSTOP);
				return( E_FAIL ); 
			}
		}
	}


	// 後で変更しないレンダリングステートの設定
	g_pD3DDevice->SetRenderState( D3DRS_ZENABLE,	D3DZB_TRUE );		// Zバッファを有効にする。
	g_pD3DDevice->SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE );		// ポリゴンの裏表チェックをしない
//	g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );						// ライトを有効に
//	g_pD3DDevice->SetRenderState( D3DRS_AMBIENT, 0x00808080 );					// 環境光

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1,	D3DTA_TEXTURE );	// テクスチャの色を使用
	g_pD3DDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2,	D3DTA_DIFFUSE );	// 頂点の色を使用
	g_pD3DDevice->SetTextureStageState( 0 , D3DTSS_COLOROP,		D3DTOP_MODULATE );	// 乗算する

//	SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );

	// フィルタの設定、D3DTEXF_POINTをD3DTEXF_LINEARやD3DTEXF_NONEに変更して
	// テクスチャの変化の仕方を見るとかすると楽しい
//	g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
//	g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );
//	g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );


	return ( S_OK );
}

/*--DirectInputの初期化--*/

HRESULT InitDirectInput(  HWND hWnd , HINSTANCE hInstance )
{
	// 変数とか
	HRESULT		hr = S_OK;			//ずっこけた時用の結果返し用

	// 最初に初期化しておく。
	KillTimer( hWnd, 0 );
	CleanUpDirectInput();

	// DirectInputオブジェクトの生成
	// 以下、ほとんど定型句
	if( FAILED( hr = DirectInput8Create( hInstance , DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDirectInput, NULL ) ) )
	{ return ( hr ); }

	if( FAILED( hr = SetupKeybord( g_pDirectInput , hWnd )) ){ return ( hr ); }
//	if( FAILED( hr = setupJoystick( g_pDirectInput , hWnd )) ){ return ( hr ); }
	if( FAILED( hr = SetupMouse( g_pDirectInput , hWnd )) ){ return ( hr ); }

	return ( hr );
}

VOID SetupSitus( theObject *me, int Type )
{
	//自車
	me->objPos			= (D3DXVECTOR3( 0.0f , 0.0f , 0.0f ));
	me->objYaw			= 0.0f;
	me->objRoll			= 0.0f;
	me->objPitch		= 0.0f;

	me->parent			= kNO_PARENT;// 親は無い

	me->objType			= Type;

}

/*--theObject構造体のリセット--*/

VOID ZeroObject( theObject *me )
{
	ZeroMemory( me, sizeof(theObject) );

	/*念のため、float部分だけ値を代入しておく*/
	me->objPos		= (D3DXVECTOR3( 0.0f , 0.0f , 0.0f ));		// オブジェクトの座標。
	me->objYaw		= 0.0f;										// オブジェクトがどっちをむいているか？
	me->objRoll		= 0.0f;										//
	me->objPitch	= 0.0f;										//
}

/*--ゲームの本体--*/

HRESULT GameBody( HWND hWnd )
{
	int	errorFlag = FALSE;
	switch(g_GameMode)
	{
		case kMODE_INIT:
			ZeroMemory(g_ObjModels,sizeof(ModelClass)*kNUM_OF_MODELS);
			ZeroMemory(&gLightLight,sizeof(D3DLIGHT9));
			kCourseIndex[kCTYPE_TOUGE][0]	= kXMAP_TOUGE;		kCourseIndex[kCTYPE_TOUGE][1]	= kXWALL_TOUGE;		kCourseIndex[kCTYPE_TOUGE][2]	= kXEND_TOUGE;
			kCourseIndex[kCTYPE_GAKE][0]	= kXMAP_GAKE;		kCourseIndex[kCTYPE_GAKE][1]	= kXWALL_GAKE;		kCourseIndex[kCTYPE_GAKE][2]	= kXEND_GAKE;
			theOpening = new OpeningClass(&errorFlag);
			CFps = new ConFps();
			g_GameMode = kMODE_OPENING;
			break;

		case kMODE_OPENING:
			if( g_pKeyboard != NULL ){ GetKeybordState( g_pKeyboard );}
			if( g_pMouse != NULL ){ getMouseState(hWnd,g_pMouse);}
			theOpening->DoOpening();
			theOpening->OpeningRender(g_pD3DDevice);
			break;

		case kMODE_INIT_MENU:
			SAFE_DELETE(theOpening);
			theMenu = new MenuClass(&errorFlag);
			g_GameMode = kMODE_MENU;
			break;
		case kMODE_MENU:
			if( g_pKeyboard != NULL ){ GetKeybordState( g_pKeyboard );}
			if( g_pMouse != NULL ){ getMouseState(hWnd,g_pMouse);}
			theMenu->DoMenu();
			theMenu->MenuRender(g_pD3DDevice);
			break;

		case kMODE_INIT_RACE:
			SAFE_DELETE(theMenu);
			theRace = new RaceClass(kRACEMODE_MAIN, gCourseType,&errorFlag);
			g_GameMode = kMODE_RACE;
			break;

		case kMODE_RACE:

			//入力を受け付ける
			if( g_pKeyboard != NULL ){ GetKeybordState( g_pKeyboard );}
			if( g_pMouse != NULL ){ getMouseState(hWnd,g_pMouse);}

			theRace->DoRace();
			theRace->RaceRender(g_pD3DDevice);
			break;

		case kMODE_END_RACE:
			SAFE_DELETE(theRace);
			theResult = new GameResult(&errorFlag);
			g_GameMode = kMODE_RESULT;
			break;

		case kMODE_RESULT:
			if( g_pKeyboard != NULL ){ GetKeybordState( g_pKeyboard );}
			if( g_pMouse != NULL ){ getMouseState(hWnd,g_pMouse);}
			theResult->DoResult();
			theResult->ResultRender(g_pD3DDevice);
			break;

		case kMODE_END_RESULT:
			SAFE_DELETE(theResult);
			theCredit = new CreditClass(&errorFlag);
			g_GameMode = kMODE_CREDIT;
			break;
		case kMODE_CREDIT:
			if( g_pKeyboard != NULL ){ GetKeybordState( g_pKeyboard );}
			if( g_pMouse != NULL ){ getMouseState(hWnd,g_pMouse);}
			theCredit->DoCredit();
			theCredit->CreditRender(g_pD3DDevice);
			break;
		case kMODE_EXIT:
			break;

		default:
			g_GameMode = kMODE_INIT;
			break;
	}
	if(errorFlag == TRUE)
	{
		return(E_FAIL);
	}

	//カウンタインクリメント
	g_GameCounter++;
	return (S_OK);
}

/*--レンダリングステートの設定 置くだけ置いておく--*/

VOID SetupRanderStates( LPDIRECT3DDEVICE9 theDevice , DWORD aMode )
{
	switch( aMode )
	{
		// DrawSubsetなどで用いる普通の描画
		default:

		case kRS_MESH:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );		// αブレンディング無効
			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// αテストを無効に

			if(g_GameMode == kMODE_RACE)
			{
				theDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
			}
			else
			{
				theDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
			}

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );			// Zバッファへの書き込み許可
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

			theDevice->SetRenderState( D3DRS_AMBIENT, 0x00808080 );				// 環境光

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_ONE );			// ソース側の色の比率
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_ZERO );		// 書き込み側の色の比率

			theDevice->LightEnable( 0, TRUE );									// ライトを有効に

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_DISABLE );		//αチャネルは無効に

			SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );
		break;

		// テクスチャのαチャネルを利用して重ね合わせ
		// Zバッファと描画順序の関係で描画されない部分が出るので
		// 奥から手前の順にソートしておく必要がある
		case kRS_ALPHA:
		case kRS_INFO:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );			// αブレンディング有効

			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	TRUE );			// αテストを有効に
			theDevice->SetRenderState( D3DRS_ALPHAREF,	0x00 );					// αチャネルの値が0x00なら
			theDevice->SetRenderState( D3DRS_ALPHAFUNC,	D3DCMP_NOTEQUAL );		// 書き込みをしない

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );		// Zバッファへの書き込み停止
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// 環境光

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// ソース側の色の比率
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// 書き込み側の色の比率

			theDevice->LightEnable( 0, FALSE );									// ライトを無効に

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );		// αチャネルの色を使用
			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );	// テクスチャの色を使用

			// 情報レイヤの場合はαチャネル合成を行うが、テクスチャは線形補完しない
			if( aMode == kRS_ALPHA ){ SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR ); }
			else{ SetupTexSamplerState( g_pD3DDevice , kTSS_POINT ); }
		break;

		// 加算合成での重ね合わせ
		// αチャネルでの合成同様、置くから手前の順にソートしておく必要がある
		case kRS_ADDBLEND:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );			// αブレンディング有効

			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// αテストを無効に

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );		// Zバッファへの書き込み停止
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
		
			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// 環境光

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_ONE );			// ソース側の色の比率
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_ONE );			// 書き込み側の色の比率

			theDevice->LightEnable( 0, FALSE );									// ライトを無効に


			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );		// αチャネルの色を使用
			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );	// テクスチャの色を使用

			SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );
		break;

		//メニュー用の設定
		case kRS_MENU:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );		// αブレンディング無効
			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// αテストを無効に

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );			// Zバッファへの書き込み許可
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
			
			theDevice->SetRenderState( D3DRS_AMBIENT,	0x80808080 );			// 環境光

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// ソース側の色の比率
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// 書き込み側の色の比率

			theDevice->LightEnable( 0, FALSE );

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_DISABLE );		//αチャネルは無効に

			SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );
			break;

		case kRS_BRIGHT:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );		// αブレンディング無効
			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// αテストを無効に

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );			// Zバッファへの書き込み許可
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
			
			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// 環境光

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// ソース側の色の比率
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// 書き込み側の色の比率

			theDevice->LightEnable( 0, FALSE );

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_DISABLE );		//αチャネルは無効に

			SetupTexSamplerState( g_pD3DDevice , kTSS_POINT );
			break;

		case kRS_BACK:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );		// αブレンディング無効
			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// αテストを無効に

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );			// Zバッファへの書き込み許可
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
			
			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// 環境光

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// ソース側の色の比率
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// 書き込み側の色の比率

			theDevice->LightEnable( 0, FALSE );									// ライトを無効に

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_DISABLE );		//αチャネルは無効に

			SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );
			break;

		case kRS_POINTER:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );			// αブレンディング有効

			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	TRUE );			// αテストを有効に
			theDevice->SetRenderState( D3DRS_ALPHAREF,	0x00 );					// αチャネルの値が0x00なら
			theDevice->SetRenderState( D3DRS_ALPHAFUNC,	D3DCMP_NOTEQUAL );		// 書き込みをしない

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );		// Zバッファへの書き込み停止
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// 環境光

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// ソース側の色の比率
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// 書き込み側の色の比率

			theDevice->LightEnable( 0, FALSE );

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );		// αチャネルの色を使用
			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );	// テクスチャの色を使用

			SetupTexSamplerState( g_pD3DDevice , kTSS_POINT );
		break;

	}
}

/*--ライトのセットアップ--*/

VOID SetupLights(LPDIRECT3DDEVICE9 theDevice,D3DXVECTOR3 VecDir,D3DLIGHT9 light,int LIndex)
{
	// SetRenderStrate()でD3DRS_LIGHTINGを有効にしておかないと、意味がなかったり。


	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &VecDir );
	theDevice->SetLight( LIndex, &light );
}

/*--視点とかカメラ位置とか変換行列とか、そんなこんなを設定--*/

VOID SetupWorldMatrix(LPDIRECT3DDEVICE9 theDevice ,D3DXVECTOR3 *aEye , D3DXVECTOR3 *aLookAt , D3DXVECTOR3* aUpVec )
{
	D3DXMATRIXA16	matView;			// 視界

	D3DXMatrixLookAtLH( &matView, aEye, aLookAt, aUpVec );

	//　投影行列を設定。
	//　視野角、アスペクト比、視覚クリッピング用の最接近距離と最遠方距離の四つが必要。
	D3DXMATRIXA16 matProj;

	FLOAT	tmpAspect = (float)g_D3DPParams.BackBufferWidth / (float)g_D3DPParams.BackBufferHeight;

	D3DXMatrixPerspectiveFovLH(	&matProj,
								D3DX_PI/4,	// 視野角
								tmpAspect,// アスペクト比
								20.0f,		// 最近接距離
								18000.0f );	// 最遠方距離

	// ビューマトリクスの設定
	theDevice->SetTransform( D3DTS_VIEW, &matView );
	
	//ディバイスに対して、投影行列を設定。
	theDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

/*--構造体を受け取ってDrawSubsetで描画--*/

VOID drawMesh( LPDIRECT3DDEVICE9 theDevice, theObject *me ,ModelClass* theModel)
{
	SetupObjMatrix( theDevice , me );	//オブジェクトの表示用行列を設定
	
	// マテリアルの数だけループ
	for(DWORD j0=0; j0 < theModel->ReturnNumOfMaterials() ; j0++ )
	{
		// サブセットにマテリアルとテクスチャを設定。
		theDevice->SetMaterial( &(theModel->ReturnMeshMaterials()[j0]));
		theDevice->SetTexture( 0, theModel->ReturnMeshTextures()[j0] );
		
		// メッシュ・サブセットの描画。
		(theModel->ReturnMesh())->DrawSubset( j0 );
	}
}

/*--D3Dの後始末--*/

VOID CleanUpDirect3D( void )
{
	// モデル以外の解放

	// DirectXディバイスなどの解放	
	SAFE_RELEASE( g_pD3DDevice );
	SAFE_RELEASE( g_pD3D );
}

/*--テクスチャサンプラーステートの設定--*/

VOID SetupTexSamplerState(LPDIRECT3DDEVICE9 theDevice , DWORD aMode )
{

	switch( aMode )
	{
		// 線形補完する
		default:
		case kTSS_LINEAR:
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );
		break;

		// 補完しない
		case kTSS_POINT:
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_POINT );
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_POINT );
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_POINT );
		break;
	}
}

/*--オブジェクト表示のための設定--*/

VOID SetupObjMatrix(LPDIRECT3DDEVICE9 theDevice , theObject *me)
{

	// 自機のための行列
	D3DXMATRIX	thePosMatrix;		// 位置用行列
	D3DXMATRIX	theRotMatrix;		// 回転用行列
	D3DXMATRIX	theResultMatrix;	// 結果の行列

	//単位行列を収める（念のため）
	D3DXMatrixIdentity( &thePosMatrix );
	D3DXMatrixIdentity( &theRotMatrix );
	D3DXMatrixIdentity( &theResultMatrix );

	// 平行移動と回転の行列を設定
	D3DXMatrixTranslation( &thePosMatrix ,me->objPos.x , me->objPos.y , me->objPos.z);
	D3DXMatrixRotationYawPitchRoll( &theRotMatrix ,  me->objYaw , me->objRoll , me->objPitch );

	// 行列の合成
	theResultMatrix = theRotMatrix * thePosMatrix;

	// 描画ディバイスへ適用
	theDevice->SetTransform( D3DTS_WORLD , &theResultMatrix );

}

/*--DirectInputでキーボードを使用可能にする--*/

HRESULT SetupKeybord( LPDIRECTINPUT8 aDirectInput , HWND hWnd )
{
	// 変数とか
	HRESULT		hr;			//ずっこけた時用の結果返し用

	// システムキーボードディバイスを探して確保しておく。
	// キーボードが無い場合（展示用にキーボードをは外すなどしている）は注意が必要
	if( FAILED( hr = aDirectInput->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, NULL ) ) )
	{ return ( hr ); }

	if( g_pKeyboard == NULL )
	{
		MessageBox( NULL, TEXT("キーボードを発見できませんでした。\n（そんなバカな？）"), kERROR_MSG , MB_ICONERROR | MB_OK );
		return( S_OK );		//ここで帰ってしまう
	}

	// DirectInputから受け取るデータフォーマットをキーボードに設定する
	// c_dfDIKeyboardは、DirectInputが用意したグローバル変数
	// const設定されていて変数の値は変更できない
	if( FAILED( hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{ return ( hr ); }

	// システムや、他のDirectInputを利用したアプリケーションと、どのように連携すればよいのか
	// DirectInputに通知するために、協調レベルをセットする。
	//（フォアグランド・直接入力・協調モード・Windowsキー無効に固定）
	if( FAILED(hr = g_pKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)) )
	{ return ( hr ); }

	// キーボードの入力を有効にする。
	g_pKeyboard->Acquire();

	return( S_OK );
}

/*--キーボードの状態を取得する。複数キーの入力を認識する。--*/
/*--バッファを使ってデータ入力を行いたい場合は、変更の必要がある。--*/

int GetKeybordState( LPDIRECTINPUTDEVICE8 aKeybord )
{
//	static int	result[256]
//	int			result		= kKEY_NONE;	// 返り値用変数
	HRESULT		hr;							// システムリザルト
//	BYTE		diks[256];					// DirectInputのキーボードステータスバッファ

	// キーボードディバイスが無い場合は、滑らかに去る。
	if( NULL == aKeybord ) { return ( FALSE ); }

	// 入力ディバイスの状態を確保し、配列におさめる。
	ZeroMemory( g_pKeyState, sizeof(g_pKeyState) );
	hr = aKeybord->GetDeviceState( sizeof(g_pKeyState), g_pKeyState );
	if( FAILED(hr) )
	{
		// もし入力が失われていた場合、確保し、維持を試みる。
		hr = aKeybord->Acquire();
		while( hr == DIERR_INPUTLOST ) { hr = aKeybord->Acquire(); }

		// エラー時は帰る。
		if( FAILED(hr) ) { return FALSE; }
	}

	return 0;
}

/*--オブジェクトをある向きにある程度進める--*/

D3DXVECTOR3	MoveForward(D3DXVECTOR3 myvector, float move, float kakudo)
{
	myvector.x += move * sinf(kakudo);
	myvector.z += move * cosf(kakudo);
	return myvector;
}

/*--DirectInput関係の変数の初期化--*/

VOID CleanUpDirectInput( void )
{
	// DirectInputでキーボード、ジョイスティック、マウスのいずれかが使用中だった場合、使用を終了する。
	if( g_pKeyboard ){ g_pKeyboard->Unacquire(); }
	if( g_pJoystick ){ g_pJoystick->Unacquire(); }
	if( g_pMouse ){ g_pMouse->Unacquire(); }
    
	// ディバイスを開放する
	SAFE_RELEASE( g_pKeyboard );
	SAFE_RELEASE( g_pJoystick );
	SAFE_RELEASE( g_pMouse );

	// DirectInputの解放。
	SAFE_RELEASE( g_pDirectInput );
}

/*--ある座標に向かって移動--*/

D3DXVECTOR3 MoveTo(D3DXVECTOR3 StartPint,D3DXVECTOR3 ToPoint,float MoveVol,int* AtFlag)
{
	*AtFlag = FALSE;
	D3DXVECTOR3 VecBuf,ReturnVec,KariVec;
	VecBuf = ToPoint - StartPint;
	if(D3DXVec3Length(&VecBuf) > MoveVol)
	{
		ReturnVec = StartPint + *D3DXVec3Normalize(&KariVec,&VecBuf) * MoveVol;
	}
	else
	{
		*AtFlag = TRUE;
		ReturnVec = ToPoint;
	}
	return (ReturnVec);
}

/*--integerソート用--*/

int sort_ranking(const void* Ranka, const void* Rankb)
{
	theRanking* pRankinga = (theRanking*) Ranka;
	theRanking* pRankingb = (theRanking*) Rankb;

	if(pRankinga->theTime > pRankingb->theTime ){ return( 1 );}
	else
	if(pRankinga->theTime < pRankingb->theTime ){ return( -1 );}
	else
	{ return( 0 );}
}

/*--マウスをセットアップ--*/

HRESULT SetupMouse( LPDIRECTINPUT8 aDirectInput , HWND hWnd )
{
	HRESULT		hr = S_OK;	//戻り値の設定

	// システムマウスデバイスを探して確保する。
	// キーボードに似ている
	if( FAILED( hr = aDirectInput->CreateDevice( GUID_SysMouse, &g_pMouse, NULL ) ) ){ return ( hr ); }

    // ジョイスティックは確保できた？
	// ジョイスティックが接続されていなかった場合、後から接続しても認識しない。
	if( NULL == g_pMouse )
	{
		MessageBox( NULL, "マウスが見つかりませんでした（そんなバカな？）\n他のデバイスで操作をします", kERROR_MSG , MB_ICONERROR | MB_OK );
		return( hr );		//ここで帰ってしまう
	}

	// DirectInputから受け取るデータフォーマットをマウス（拡張仕様）に設定する
	// c_dfDIMouse2はDirectInputが用意したグローバル変数でconst設定されていて変数の値は変更できない。
	// この部分はJoystickの処理と同様。ただし複数のマウスがある事は考えられていない。
	if( FAILED( hr = g_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
	{ return ( hr ); }

	// 他のデバイスと同様に協調レベルをセットする。
	// ここではフォアグランド・排他的利用モードに固定
	// ゲームの操作のためにマウスを使う場合は、通常このモードでかまわない。
	// しかし、このモードでは、マウスカーソルが見えなくなるため、
	// アプリケーション側でマウスカーソルを独自に表示させるなど工夫が必要になるので要注意。
	if( FAILED( hr = g_pMouse->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
	{
		CleanUpDirectInput();
		if( hr == DIERR_UNSUPPORTED )
		{
			MessageBox( NULL, "DirectInputがサポートしていない協調レベルです。", kERROR_MSG , MB_ICONERROR | MB_OK );
		}
		return ( hr );
	}

	// マウスの入力を有効にする。
	g_pMouse->Acquire();

	return( S_OK );

}

/*--マウスの状態を習得--*/

HRESULT getMouseState( HWND hWND , LPDIRECTINPUTDEVICE8 aMouse)
{
	HRESULT			hr;						// システムリザルト
	DIMOUSESTATE2	aMouseState;			// DirectInputのマウス情報構造体

	// マウスが無い場合は、滑らかに去る。
	if( NULL == aMouse ){ return ( S_OK ); }

	// 入力ディバイスの状態を確保し、配列におさめる。
	// キーボードとよく似ている
	ZeroMemory( &aMouseState, sizeof(DIMOUSESTATE2) );
	hr = g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &aMouseState );
	if( FAILED(hr) )
	{
		// もし入力が失われていた場合、再確保を試みる。
		hr = aMouse->Acquire();
		while( hr == DIERR_INPUTLOST ) { hr = aMouse->Acquire(); }

		// エラー時は他のアプリケーションがフォアグランドになっていたりするので何もしないで帰る。
		if( FAILED(hr) )
		{
			return( S_OK );
		}
	}

	return( S_OK );
}

/*--theBillbordの初期化--*/

VOID ZeroBillbord( theBillbord *me )
{
	ZeroMemory( me, sizeof(theBillbord) );

	// 念のため、float部分だけ値を代入しておく
	me->objPos		= (D3DXVECTOR3( 0.0f , 0.0f , 0.0f ));		// ビルボードの座標。
	me->objPitch	= 0.0f;
	me->objRoll		= 0.0f;
	me->objYaw		= 0.0f;

	me->objScal		= 1.0f;										// ビルボードの拡大縮小率

	me->f0			= 0.0f;										// 汎用float値
	me->f1			= 0.0f;

	me->LDflag		= kTEX_NONE;								// 生死フラグ
}

/*--画像ファイルをテクスチャとして読み込む--*/

HRESULT	LoadTextureFromImageFiles( LPCTSTR aFileName , LPDIRECT3DTEXTURE9* aTexture , LPCTSTR aSearchPath , LPDIRECT3DDEVICE9 theDevice )
{
	// テクスチャの生成
	// これもまずカレントフォルダを探し、見つからない場合は「models」フォルダ内を探す
	if( FAILED( D3DXCreateTextureFromFile(	theDevice,aFileName,aTexture ) ) )
	{
		// もしカレントディレクトリにファイルが無かったら、modelsフォルダを確かめる。
		// バックスラッシュ(\)が二つ重なっているのは、エスケープシーケンスが有効になっているため
		TCHAR strTexture[MAX_PATH];

		DWORD aPathLength = lstrlen( aSearchPath );

		lstrcpyn( strTexture, aSearchPath, MAX_PATH );
		lstrcpyn( strTexture + aPathLength, aFileName, MAX_PATH - aPathLength );

		// テクスチャが無かったら、ひとつ上のディレクトリを確かめる。
		if( FAILED(D3DXCreateTextureFromFile( theDevice, strTexture, aTexture )) ){ return(E_FAIL); }
	}

	return( S_OK );
}

/*--ビルボードの描画--*/

VOID drawFVF(LPDIRECT3DDEVICE9 theDevice,theBillbord *me,LPDIRECT3DTEXTURE9 theTexture,LPDIRECT3DVERTEXBUFFER9 theVertexBuffer)
{
	SetupFVFMatrix(theDevice,me);	//オブジェクトの表示用行列を設定

	int	aTex = me->TexType;

	theDevice->SetTexture( 0, theTexture );
	SetFVFTexturePosition( theVertexBuffer,me->TexType,me->TexIndex );

	theDevice->SetStreamSource( 0, theVertexBuffer , 0, sizeof(theCUSTOMVERTEX) );
	theDevice->SetFVF( kD3DFVF_CUSTOMVERTEX );
	theDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

/*--FVFのための位置と回転の設定。拡大縮小にも対応する。--*/

VOID SetupFVFMatrix(LPDIRECT3DDEVICE9 theDevice,theBillbord *me)
{
	D3DXMATRIX	theSclMatrix;		// 拡縮用行列
	D3DXMATRIX	thePosMatrix;		// 位置用行列
	D3DXMATRIX	theRotMatrix;		// 回転用行列
	D3DXMATRIX	theResultMatrix;	// 結果用行列

	D3DXVECTOR3	vecDir;				// ビルボード位置オフセット用

	// 拡大縮小用の行列を設定
	D3DXMatrixScaling( &theSclMatrix , me->objScal , me->objScal , me->objScal );

	// 平行移動と回転の行列を設定
	D3DXMatrixTranslation( &thePosMatrix , me->objPos.x , me->objPos.y , me->objPos.z);

	D3DXMatrixRotationYawPitchRoll( &theRotMatrix ,  me->objYaw , me->objRoll , me->objPitch );	

	// 行列を合成する
	theResultMatrix = theSclMatrix * theRotMatrix * thePosMatrix;

	// 描画ディバイスへ適用
	theDevice->SetTransform( D3DTS_WORLD , &theResultMatrix );
}

/*--FVFのテクスチャ座標をセット--*/

HRESULT	SetFVFTexturePosition( LPDIRECT3DVERTEXBUFFER9 aFVFBuffer , DWORD aType , DWORD aChar )
{
    theCUSTOMVERTEX*	tmpVertex;		// 頂点バッファロック用
	theTexPosRECT		aTexRECT;		// テクスチャ座標受け取り用

	switch( aType )
	{
		case kTEX_ASCII:
			SetCharTextureRECT( aChar , &aTexRECT );
		break;
	}

	// 頂点バッファをロック
	if( FAILED( aFVFBuffer->Lock( 0, 0, (void**)&tmpVertex, 0 ) ) ){ return( E_FAIL ); }

	// 頂点は四つ、頂点座標、頂点色、テクスチャ座標を設定する
	// D3DPT_TRIANGLESTRIPで描画するので頂点の番号と座標の対応に注意
	tmpVertex[0].tu = aTexRECT.u0 ; tmpVertex[0].tv = aTexRECT.v0;
	tmpVertex[1].tu = aTexRECT.u1 ; tmpVertex[1].tv = aTexRECT.v0;
	tmpVertex[2].tu = aTexRECT.u0 ; tmpVertex[2].tv = aTexRECT.v1;
	tmpVertex[3].tu = aTexRECT.u1 ; tmpVertex[3].tv = aTexRECT.v1;

	// 書き込みが終わったので頂点バッファのロックを解除する
	aFVFBuffer->Unlock();

	return( S_OK );
}

/*--英数字のテクスチャ座標設定--*/

VOID SetCharTextureRECT( DWORD aChar , theTexPosRECT *aRECT )
{
	const float	kCellSize = 1.0 / 16.0f;

	int		upper,lower;

	//溢れ防止
	aChar &= 0xff;
	upper = aChar / 0x10;
	lower = aChar & 0x0f;
	aRECT->u1 = kCellSize * lower; aRECT->u0 = aRECT->u1 + kCellSize; if( aRECT->u0 > 1.0f ){ aRECT->u0 = 1.0f ;}
	aRECT->v1 = kCellSize * upper; aRECT->v0 = aRECT->v1 + kCellSize; if( aRECT->v0 > 1.0f ){ aRECT->v0 = 1.0f ;}

}