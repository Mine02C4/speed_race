//speedrace_main.cpp
//���̃\�[�X�t�@�C����WinMain���܂ރ��C���ƂȂ�\�[�X���܂܂�Ă��܂��B

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

/*--���C���֐�--*/

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	// ���d�N���̃`�F�b�N
	HANDLE hMutex = CreateMutex(NULL, FALSE, kWINDOW_TITLE);
	DWORD theErr = GetLastError();
	ShowCursor(false);
	if(theErr == ERROR_ALREADY_EXISTS)
	{
		// ���d�N�����Ă���
		if(hMutex) CloseHandle(hMutex);
		MessageBox( NULL, TEXT("���̃A�v���P�[�V�����͑��d�N���ł��܂���B"), kERROR_MSG, MB_OK|MB_ICONHAND );
		return( E_FAIL );
	}

	srand( (unsigned)time( NULL ) );
	using namespace std;

	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
						GetModuleHandle(NULL), NULL, NULL, NULL, NULL, kWINDOW_TITLE, NULL };

	if( !RegisterClassEx( &wc ) ){ return(FALSE);}//�R�P����A��

	// �E�B���h�E�̍쐬
	HWND	hWnd;

	//�ő剻�{�^���������Ȃ��E���E�ύX�̂ł��Ȃ�window
	DWORD	dwWindowStyle= ~(WS_MAXIMIZEBOX | WS_THICKFRAME) & WS_OVERLAPPEDWINDOW;

	hWnd = CreateWindow(	kWINDOW_TITLE, kWINDOW_TITLE, 
							dwWindowStyle, kWINDOW_LEFT, kWINDOW_TOP, kWINDOW_LEFT + kWINDOW_WIDTH, kWINDOW_TOP + kWINDOW_HEIGHT,
							GetDesktopWindow(), NULL, wc.hInstance, NULL );


	// Direct3D����ɏ���������ƁA�t���X�N���[�����[�h�̂Ƃ��G���[�_�C�A���O�������Ȃ��Ȃ邽��
	if(FAILED(InitDirectInput(hWnd,hInst)))
	{
		ZeroMemory( g_pKeyState, sizeof(g_pKeyState) ); //�O�̂��߃L�[�{�[�h�X�e�[�^�X�o�b�t�@��������
		MessageBox( NULL, TEXT("DirectInput�̏������Ɏ��s���܂����B"), kERROR_MSG, MB_OK|MB_ICONHAND );
		CleanUpApplication( &wc );
		return( E_FAIL );
	}

	// Direct3D�̏�����
	if(FAILED(InitDirect3D(hWnd)))
	{
		MessageBox( NULL, TEXT("DirectX�̏������Ɏ��s���܂����B"), kERROR_MSG, MB_OK|MB_ICONHAND );
		CleanUpApplication( &wc );
		return( E_FAIL );
	}
			
//	// �V�[���̃W�I���g���𐶐�
//	if((FAILED( SetupModels( g_pD3DDevice ) ))/*||(FAILED( SetupBillbords( g_pD3DDevice ) ))||(FAILED( SetupFVFTexture( g_pD3DDevice ) ))*/)
//	{
//		MessageBox( NULL, TEXT("���f���̓ǂݍ��݂Ɏ��s���܂����B"), kERROR_MSG, MB_OK|MB_ICONHAND );
//		CleanUpApplication( &wc );
//		return( E_FAIL );
//	}

	// �E�B���h�E�̕\��
	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	// �^�C�}���Z�b�g����B
	SetTimer( hWnd, kTIMER_ID, kTIMER_LAP , NULL );

	// ���b�Z�[�W���[�v
	MsgLoop( hWnd );

	// �{���E�B���h�E�Ɋ֘A�t����ꂽ�^�C�}�̓E�B���h�E�̔j���Ɠ�����OS�������I�ɔj�����邪
	// �����ł͔O�̂��߃A�v���P�[�V�������Ŕj�����Ă���
	KillTimer( hWnd, kTIMER_ID );	

	// ��n���ƃA�v���P�[�V�����̏I��
	CleanUpApplication( &wc );								// DirectX�̌�n��
	if(hMutex){ CloseHandle(hMutex); }						// Mutex�̊J��
	return( S_OK );
}


/*--���b�Z�[�W���[�v--*/

HRESULT MsgLoop( HWND hWnd )
{
	// ���b�Z�[�W���[�v
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while( msg.message != WM_QUIT )
	{
		// �C�x���g�̎擾
		// PM_NOREMOVE�Ƃ���̂́AWindow9x/Me�nOS��
		// �v���Z�X���]���r������̂�h�����߁B
		if( PeekMessage (&msg,NULL,0,0,PM_NOREMOVE) ) 
		{
			// �����ŃC�x���g���擾����B
			if( !GetMessage (&msg,NULL,0,0) ){ msg.message = WM_QUIT; }	// �I��
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{ Sleep(kSLEEP); }	//CPU HUG�����̂��߁B
	}

	return( S_OK );
}


/*--�A�v���P�[�V�����E�B���h�E�̌�n��--*/

VOID CleanUpApplication( WNDCLASSEX *theWC )
{
	SAFE_DELETE(theMenu);
	SAFE_DELETE(theRace);
	SAFE_DELETE(theResult);
	SAFE_DELETE(theOpening);
	SAFE_DELETE(theCredit);
	delete CFps;
	CleanUpDirectInput();									//DirectInput�̌�n��
	CleanUpDirect3D();										//D3DX�Ȃǂ̌�n��

	UnregisterClass( kWINDOW_TITLE , theWC->hInstance );	//�E�B���h�E�̔j��

	return;
}



/*--�A�v���P�[�V�����E�B���h�E�̃��b�Z�[�W�n���h��--*/

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static BOOL sWinActivFlag = FALSE;
	static BOOL sMsgBoxFlag = FALSE;

	switch( msg )
	{
		//esc�L�[���������Ƃ��̎�������
        case WM_KEYUP: 
			if(VK_ESCAPE == (int)wParam)
			{
				// �t���X�N���[�����[�h�̏ꍇ�͂����Ȃ�I���
				if( g_FullScreenFlag )
				{ PostQuitMessage( 0 ); return (0); }
				else 
				{
					sMsgBoxFlag = TRUE;
					//�m�F�_�C�A���O���o�Ă���ꍇ�͏����𒆒f����
					if( MessageBoxEx(	hWnd, TEXT("���f���܂����H"), kWINDOW_TITLE,
										MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ,LANG_JAPANESE) == IDYES )
					{ PostQuitMessage( 0 ); return (0); }
					sMsgBoxFlag = FALSE;

				}
			} 
		break; 

		case WM_DESTROY:
			PostQuitMessage( 0 );//�E�B���h�E�j�����̎�������
			return (0);
		break;

		case WM_ACTIVATE:		//�E�B���h�E���ĂуA�N�e�B�u�ɂȂ�܂���
			if( WA_INACTIVE != wParam)
			{
				// �E�B���h�E�A�N�e�B�u�t���O��true��
				sWinActivFlag = TRUE;
			}
			else if( wParam == WA_INACTIVE )
			{
				// �����E�B���h�E���w��ɂ܂�����珈���𒆒f
				sWinActivFlag = FALSE;
			}
		break;

		case WM_TIMER:					//�^�C�}���荞��
			if( !sMsgBoxFlag && sWinActivFlag )
			{
				// �E�B���h�E���A�N�e�B�u�ȏꍇ�����A�Q�[���{�̂̏������s���B
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


/*--Direct3D�̏�����--*/

HRESULT InitDirect3D( HWND hWnd )
{
	// D3D�I�u�W�F�N�g�̐����B
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ){ return (E_FAIL); }

	// D3D�f�B�o�C�X�𐶐����邽�߂̍\���̂�ݒ肷��B
	ZeroMemory( &g_D3DPParams, sizeof(D3DPRESENT_PARAMETERS) );
	
	// �v���[���e�[�V�����p�����[�^�̐ݒ�
	if( g_FullScreenFlag )
	{
		//���݂̃v���C�}���f�B�X�v���C�A�_�v�^�̃��[�h���擾����
		D3DDISPLAYMODE aDisplayMode;		//�f�B�X�v���C���[�h�̏������߂邽��

		if(FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &aDisplayMode )))
		{
			MessageBox(NULL,TEXT("�f�B�X�v���C���[�h�̎擾�Ɏ��s���܂���"),kWINDOW_TITLE,MB_OK | MB_ICONSTOP);
			return( E_FAIL ); 
		}

		g_D3DPParams.BackBufferHeight			= aDisplayMode.Height;			// �o�b�N�o�b�t�@�̍���
		g_D3DPParams.BackBufferWidth			= aDisplayMode.Width;			// �o�b�N�o�b�t�@�̕�

		g_D3DPParams.BackBufferFormat			= aDisplayMode.Format;			// �o�b�N�o�b�t�@�̃t�H�[�}�b�g

		g_D3DPParams.Windowed					= FALSE;						// �t���X�N���[�����[�h
	}
	else
	{
		g_D3DPParams.BackBufferFormat			= D3DFMT_UNKNOWN;				// �t�H�[�}�b�g�͍��̉�ʃ��[�h�ɏ]��

		g_D3DPParams.Windowed					= TRUE;							// �E�B���h�E���[�h
	}

	//���ʃp�����[�^
	g_D3DPParams.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�

	g_D3DPParams.MultiSampleType			= D3DMULTISAMPLE_NONE;		// �}���`�T���v�����O�͍s��Ȃ�
	g_D3DPParams.MultiSampleQuality			= 0;						// �}���`�T���v�����O�͍s��Ȃ��̂�0

	g_D3DPParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// Direct3D�ɃX���b�v�G�t�F�N�g��C����

	g_D3DPParams.EnableAutoDepthStencil		= TRUE;						// Direct3D�ɐ[�x�o�b�t�@�̊Ǘ���C����
	g_D3DPParams.AutoDepthStencilFormat		= D3DFMT_D16;				// �[�x�o�b�t�@�̃t�H�[�}�b�g�i�ʏ�͂��̒l�Ŗ��Ȃ��j

	g_D3DPParams.hDeviceWindow				= hWnd;						// �J�o�[�E�B���h�E���A�v���P�[�V�����̃E�B���h�E

	g_D3DPParams.Flags						= 0;						// �t���O�͎g��Ȃ�

	g_D3DPParams.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	// ���̃��t���b�V�����[�g�����̂܂܎g��

	g_D3DPParams.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	// ���j�^�̐�����A��҂�

	// CreateDevice��Direct3D������������Ƃ��AHAL�AHEL�̑g�ݍ��킹�̓v���O���}�����R�Ɍ��߂邱�Ƃ��ł��邪
	// ���ʂ͂��̑g�ݍ��킹�ŏ[��

	//�@D3D�f�B�o�C�X�̐����B�܂��AHAL�̂������
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, hWnd,
										D3DCREATE_HARDWARE_VERTEXPROCESSING,&g_D3DPParams, &g_pD3DDevice ) ) )
	{
		//�R�P���畁�ʂ�HAL(HEL?)
		if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,&g_D3DPParams, &g_pD3DDevice ) ) )
		{
			//������REF
			if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_REF, hWnd,
												D3DCREATE_SOFTWARE_VERTEXPROCESSING,&g_D3DPParams, &g_pD3DDevice ) ) )
			{
				//����ł��R�P���炨���܂��B
				MessageBox(NULL,TEXT("Direct3D���������ł��܂���ł���"),kWINDOW_TITLE,MB_OK | MB_ICONSTOP);
				return( E_FAIL ); 
			}
		}
	}


	// ��ŕύX���Ȃ������_�����O�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState( D3DRS_ZENABLE,	D3DZB_TRUE );		// Z�o�b�t�@��L���ɂ���B
	g_pD3DDevice->SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE );		// �|���S���̗��\�`�F�b�N�����Ȃ�
//	g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );						// ���C�g��L����
//	g_pD3DDevice->SetRenderState( D3DRS_AMBIENT, 0x00808080 );					// ����

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1,	D3DTA_TEXTURE );	// �e�N�X�`���̐F���g�p
	g_pD3DDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2,	D3DTA_DIFFUSE );	// ���_�̐F���g�p
	g_pD3DDevice->SetTextureStageState( 0 , D3DTSS_COLOROP,		D3DTOP_MODULATE );	// ��Z����

//	SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );

	// �t�B���^�̐ݒ�AD3DTEXF_POINT��D3DTEXF_LINEAR��D3DTEXF_NONE�ɕύX����
	// �e�N�X�`���̕ω��̎d��������Ƃ�����Ɗy����
//	g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
//	g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );
//	g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );


	return ( S_OK );
}

/*--DirectInput�̏�����--*/

HRESULT InitDirectInput(  HWND hWnd , HINSTANCE hInstance )
{
	// �ϐ��Ƃ�
	HRESULT		hr = S_OK;			//�������������p�̌��ʕԂ��p

	// �ŏ��ɏ��������Ă����B
	KillTimer( hWnd, 0 );
	CleanUpDirectInput();

	// DirectInput�I�u�W�F�N�g�̐���
	// �ȉ��A�قƂ�ǒ�^��
	if( FAILED( hr = DirectInput8Create( hInstance , DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDirectInput, NULL ) ) )
	{ return ( hr ); }

	if( FAILED( hr = SetupKeybord( g_pDirectInput , hWnd )) ){ return ( hr ); }
//	if( FAILED( hr = setupJoystick( g_pDirectInput , hWnd )) ){ return ( hr ); }
	if( FAILED( hr = SetupMouse( g_pDirectInput , hWnd )) ){ return ( hr ); }

	return ( hr );
}

VOID SetupSitus( theObject *me, int Type )
{
	//����
	me->objPos			= (D3DXVECTOR3( 0.0f , 0.0f , 0.0f ));
	me->objYaw			= 0.0f;
	me->objRoll			= 0.0f;
	me->objPitch		= 0.0f;

	me->parent			= kNO_PARENT;// �e�͖���

	me->objType			= Type;

}

/*--theObject�\���̂̃��Z�b�g--*/

VOID ZeroObject( theObject *me )
{
	ZeroMemory( me, sizeof(theObject) );

	/*�O�̂��߁Afloat���������l�������Ă���*/
	me->objPos		= (D3DXVECTOR3( 0.0f , 0.0f , 0.0f ));		// �I�u�W�F�N�g�̍��W�B
	me->objYaw		= 0.0f;										// �I�u�W�F�N�g���ǂ������ނ��Ă��邩�H
	me->objRoll		= 0.0f;										//
	me->objPitch	= 0.0f;										//
}

/*--�Q�[���̖{��--*/

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

			//���͂��󂯕t����
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

	//�J�E���^�C���N�������g
	g_GameCounter++;
	return (S_OK);
}

/*--�����_�����O�X�e�[�g�̐ݒ� �u�������u���Ă���--*/

VOID SetupRanderStates( LPDIRECT3DDEVICE9 theDevice , DWORD aMode )
{
	switch( aMode )
	{
		// DrawSubset�Ȃǂŗp���镁�ʂ̕`��
		default:

		case kRS_MESH:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );		// ���u�����f�B���O����
			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// ���e�X�g�𖳌���

			if(g_GameMode == kMODE_RACE)
			{
				theDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
			}
			else
			{
				theDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
			}

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );			// Z�o�b�t�@�ւ̏������݋���
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

			theDevice->SetRenderState( D3DRS_AMBIENT, 0x00808080 );				// ����

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_ONE );			// �\�[�X���̐F�̔䗦
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_ZERO );		// �������ݑ��̐F�̔䗦

			theDevice->LightEnable( 0, TRUE );									// ���C�g��L����

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_DISABLE );		//���`���l���͖�����

			SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );
		break;

		// �e�N�X�`���̃��`���l���𗘗p���ďd�ˍ��킹
		// Z�o�b�t�@�ƕ`�揇���̊֌W�ŕ`�悳��Ȃ��������o��̂�
		// �������O�̏��Ƀ\�[�g���Ă����K�v������
		case kRS_ALPHA:
		case kRS_INFO:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );			// ���u�����f�B���O�L��

			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	TRUE );			// ���e�X�g��L����
			theDevice->SetRenderState( D3DRS_ALPHAREF,	0x00 );					// ���`���l���̒l��0x00�Ȃ�
			theDevice->SetRenderState( D3DRS_ALPHAFUNC,	D3DCMP_NOTEQUAL );		// �������݂����Ȃ�

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );		// Z�o�b�t�@�ւ̏������ݒ�~
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// ����

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// �\�[�X���̐F�̔䗦
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// �������ݑ��̐F�̔䗦

			theDevice->LightEnable( 0, FALSE );									// ���C�g�𖳌���

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );		// ���`���l���̐F���g�p
			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );	// �e�N�X�`���̐F���g�p

			// ��񃌃C���̏ꍇ�̓��`���l���������s�����A�e�N�X�`���͐��`�⊮���Ȃ�
			if( aMode == kRS_ALPHA ){ SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR ); }
			else{ SetupTexSamplerState( g_pD3DDevice , kTSS_POINT ); }
		break;

		// ���Z�����ł̏d�ˍ��킹
		// ���`���l���ł̍������l�A�u�������O�̏��Ƀ\�[�g���Ă����K�v������
		case kRS_ADDBLEND:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );			// ���u�����f�B���O�L��

			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// ���e�X�g�𖳌���

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );		// Z�o�b�t�@�ւ̏������ݒ�~
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
		
			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// ����

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_ONE );			// �\�[�X���̐F�̔䗦
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_ONE );			// �������ݑ��̐F�̔䗦

			theDevice->LightEnable( 0, FALSE );									// ���C�g�𖳌���


			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );		// ���`���l���̐F���g�p
			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );	// �e�N�X�`���̐F���g�p

			SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );
		break;

		//���j���[�p�̐ݒ�
		case kRS_MENU:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );		// ���u�����f�B���O����
			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// ���e�X�g�𖳌���

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );			// Z�o�b�t�@�ւ̏������݋���
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
			
			theDevice->SetRenderState( D3DRS_AMBIENT,	0x80808080 );			// ����

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// �\�[�X���̐F�̔䗦
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// �������ݑ��̐F�̔䗦

			theDevice->LightEnable( 0, FALSE );

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_DISABLE );		//���`���l���͖�����

			SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );
			break;

		case kRS_BRIGHT:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );		// ���u�����f�B���O����
			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// ���e�X�g�𖳌���

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );			// Z�o�b�t�@�ւ̏������݋���
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
			
			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// ����

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// �\�[�X���̐F�̔䗦
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// �������ݑ��̐F�̔䗦

			theDevice->LightEnable( 0, FALSE );

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_DISABLE );		//���`���l���͖�����

			SetupTexSamplerState( g_pD3DDevice , kTSS_POINT );
			break;

		case kRS_BACK:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );		// ���u�����f�B���O����
			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );		// ���e�X�g�𖳌���

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );			// Z�o�b�t�@�ւ̏������݋���
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
			
			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// ����

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// �\�[�X���̐F�̔䗦
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// �������ݑ��̐F�̔䗦

			theDevice->LightEnable( 0, FALSE );									// ���C�g�𖳌���

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_DISABLE );		//���`���l���͖�����

			SetupTexSamplerState( g_pD3DDevice , kTSS_LINEAR );
			break;

		case kRS_POINTER:
			theDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );			// ���u�����f�B���O�L��

			theDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	TRUE );			// ���e�X�g��L����
			theDevice->SetRenderState( D3DRS_ALPHAREF,	0x00 );					// ���`���l���̒l��0x00�Ȃ�
			theDevice->SetRenderState( D3DRS_ALPHAFUNC,	D3DCMP_NOTEQUAL );		// �������݂����Ȃ�

			theDevice->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );		// Z�o�b�t�@�ւ̏������ݒ�~
			theDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

			theDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );			// ����

			theDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );	// �\�[�X���̐F�̔䗦
			theDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );	// �������ݑ��̐F�̔䗦

			theDevice->LightEnable( 0, FALSE );

			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );		// ���`���l���̐F���g�p
			theDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );	// �e�N�X�`���̐F���g�p

			SetupTexSamplerState( g_pD3DDevice , kTSS_POINT );
		break;

	}
}

/*--���C�g�̃Z�b�g�A�b�v--*/

VOID SetupLights(LPDIRECT3DDEVICE9 theDevice,D3DXVECTOR3 VecDir,D3DLIGHT9 light,int LIndex)
{
	// SetRenderStrate()��D3DRS_LIGHTING��L���ɂ��Ă����Ȃ��ƁA�Ӗ����Ȃ�������B


	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &VecDir );
	theDevice->SetLight( LIndex, &light );
}

/*--���_�Ƃ��J�����ʒu�Ƃ��ϊ��s��Ƃ��A����Ȃ���Ȃ�ݒ�--*/

VOID SetupWorldMatrix(LPDIRECT3DDEVICE9 theDevice ,D3DXVECTOR3 *aEye , D3DXVECTOR3 *aLookAt , D3DXVECTOR3* aUpVec )
{
	D3DXMATRIXA16	matView;			// ���E

	D3DXMatrixLookAtLH( &matView, aEye, aLookAt, aUpVec );

	//�@���e�s���ݒ�B
	//�@����p�A�A�X�y�N�g��A���o�N���b�s���O�p�̍Őڋߋ����ƍŉ��������̎l���K�v�B
	D3DXMATRIXA16 matProj;

	FLOAT	tmpAspect = (float)g_D3DPParams.BackBufferWidth / (float)g_D3DPParams.BackBufferHeight;

	D3DXMatrixPerspectiveFovLH(	&matProj,
								D3DX_PI/4,	// ����p
								tmpAspect,// �A�X�y�N�g��
								20.0f,		// �ŋߐڋ���
								18000.0f );	// �ŉ�������

	// �r���[�}�g���N�X�̐ݒ�
	theDevice->SetTransform( D3DTS_VIEW, &matView );
	
	//�f�B�o�C�X�ɑ΂��āA���e�s���ݒ�B
	theDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

/*--�\���̂��󂯎����DrawSubset�ŕ`��--*/

VOID drawMesh( LPDIRECT3DDEVICE9 theDevice, theObject *me ,ModelClass* theModel)
{
	SetupObjMatrix( theDevice , me );	//�I�u�W�F�N�g�̕\���p�s���ݒ�
	
	// �}�e���A���̐��������[�v
	for(DWORD j0=0; j0 < theModel->ReturnNumOfMaterials() ; j0++ )
	{
		// �T�u�Z�b�g�Ƀ}�e���A���ƃe�N�X�`����ݒ�B
		theDevice->SetMaterial( &(theModel->ReturnMeshMaterials()[j0]));
		theDevice->SetTexture( 0, theModel->ReturnMeshTextures()[j0] );
		
		// ���b�V���E�T�u�Z�b�g�̕`��B
		(theModel->ReturnMesh())->DrawSubset( j0 );
	}
}

/*--D3D�̌�n��--*/

VOID CleanUpDirect3D( void )
{
	// ���f���ȊO�̉��

	// DirectX�f�B�o�C�X�Ȃǂ̉��	
	SAFE_RELEASE( g_pD3DDevice );
	SAFE_RELEASE( g_pD3D );
}

/*--�e�N�X�`���T���v���[�X�e�[�g�̐ݒ�--*/

VOID SetupTexSamplerState(LPDIRECT3DDEVICE9 theDevice , DWORD aMode )
{

	switch( aMode )
	{
		// ���`�⊮����
		default:
		case kTSS_LINEAR:
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );
		break;

		// �⊮���Ȃ�
		case kTSS_POINT:
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_POINT );
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_POINT );
			g_pD3DDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_POINT );
		break;
	}
}

/*--�I�u�W�F�N�g�\���̂��߂̐ݒ�--*/

VOID SetupObjMatrix(LPDIRECT3DDEVICE9 theDevice , theObject *me)
{

	// ���@�̂��߂̍s��
	D3DXMATRIX	thePosMatrix;		// �ʒu�p�s��
	D3DXMATRIX	theRotMatrix;		// ��]�p�s��
	D3DXMATRIX	theResultMatrix;	// ���ʂ̍s��

	//�P�ʍs������߂�i�O�̂��߁j
	D3DXMatrixIdentity( &thePosMatrix );
	D3DXMatrixIdentity( &theRotMatrix );
	D3DXMatrixIdentity( &theResultMatrix );

	// ���s�ړ��Ɖ�]�̍s���ݒ�
	D3DXMatrixTranslation( &thePosMatrix ,me->objPos.x , me->objPos.y , me->objPos.z);
	D3DXMatrixRotationYawPitchRoll( &theRotMatrix ,  me->objYaw , me->objRoll , me->objPitch );

	// �s��̍���
	theResultMatrix = theRotMatrix * thePosMatrix;

	// �`��f�B�o�C�X�֓K�p
	theDevice->SetTransform( D3DTS_WORLD , &theResultMatrix );

}

/*--DirectInput�ŃL�[�{�[�h���g�p�\�ɂ���--*/

HRESULT SetupKeybord( LPDIRECTINPUT8 aDirectInput , HWND hWnd )
{
	// �ϐ��Ƃ�
	HRESULT		hr;			//�������������p�̌��ʕԂ��p

	// �V�X�e���L�[�{�[�h�f�B�o�C�X��T���Ċm�ۂ��Ă����B
	// �L�[�{�[�h�������ꍇ�i�W���p�ɃL�[�{�[�h���͊O���Ȃǂ��Ă���j�͒��ӂ��K�v
	if( FAILED( hr = aDirectInput->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, NULL ) ) )
	{ return ( hr ); }

	if( g_pKeyboard == NULL )
	{
		MessageBox( NULL, TEXT("�L�[�{�[�h�𔭌��ł��܂���ł����B\n�i����ȃo�J�ȁH�j"), kERROR_MSG , MB_ICONERROR | MB_OK );
		return( S_OK );		//�����ŋA���Ă��܂�
	}

	// DirectInput����󂯎��f�[�^�t�H�[�}�b�g���L�[�{�[�h�ɐݒ肷��
	// c_dfDIKeyboard�́ADirectInput���p�ӂ����O���[�o���ϐ�
	// const�ݒ肳��Ă��ĕϐ��̒l�͕ύX�ł��Ȃ�
	if( FAILED( hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{ return ( hr ); }

	// �V�X�e����A����DirectInput�𗘗p�����A�v���P�[�V�����ƁA�ǂ̂悤�ɘA�g����΂悢�̂�
	// DirectInput�ɒʒm���邽�߂ɁA�������x�����Z�b�g����B
	//�i�t�H�A�O�����h�E���ړ��́E�������[�h�EWindows�L�[�����ɌŒ�j
	if( FAILED(hr = g_pKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)) )
	{ return ( hr ); }

	// �L�[�{�[�h�̓��͂�L���ɂ���B
	g_pKeyboard->Acquire();

	return( S_OK );
}

/*--�L�[�{�[�h�̏�Ԃ��擾����B�����L�[�̓��͂�F������B--*/
/*--�o�b�t�@���g���ăf�[�^���͂��s�������ꍇ�́A�ύX�̕K�v������B--*/

int GetKeybordState( LPDIRECTINPUTDEVICE8 aKeybord )
{
//	static int	result[256]
//	int			result		= kKEY_NONE;	// �Ԃ�l�p�ϐ�
	HRESULT		hr;							// �V�X�e�����U���g
//	BYTE		diks[256];					// DirectInput�̃L�[�{�[�h�X�e�[�^�X�o�b�t�@

	// �L�[�{�[�h�f�B�o�C�X�������ꍇ�́A���炩�ɋ���B
	if( NULL == aKeybord ) { return ( FALSE ); }

	// ���̓f�B�o�C�X�̏�Ԃ��m�ۂ��A�z��ɂ����߂�B
	ZeroMemory( g_pKeyState, sizeof(g_pKeyState) );
	hr = aKeybord->GetDeviceState( sizeof(g_pKeyState), g_pKeyState );
	if( FAILED(hr) )
	{
		// �������͂������Ă����ꍇ�A�m�ۂ��A�ێ������݂�B
		hr = aKeybord->Acquire();
		while( hr == DIERR_INPUTLOST ) { hr = aKeybord->Acquire(); }

		// �G���[���͋A��B
		if( FAILED(hr) ) { return FALSE; }
	}

	return 0;
}

/*--�I�u�W�F�N�g����������ɂ�����x�i�߂�--*/

D3DXVECTOR3	MoveForward(D3DXVECTOR3 myvector, float move, float kakudo)
{
	myvector.x += move * sinf(kakudo);
	myvector.z += move * cosf(kakudo);
	return myvector;
}

/*--DirectInput�֌W�̕ϐ��̏�����--*/

VOID CleanUpDirectInput( void )
{
	// DirectInput�ŃL�[�{�[�h�A�W���C�X�e�B�b�N�A�}�E�X�̂����ꂩ���g�p���������ꍇ�A�g�p���I������B
	if( g_pKeyboard ){ g_pKeyboard->Unacquire(); }
	if( g_pJoystick ){ g_pJoystick->Unacquire(); }
	if( g_pMouse ){ g_pMouse->Unacquire(); }
    
	// �f�B�o�C�X���J������
	SAFE_RELEASE( g_pKeyboard );
	SAFE_RELEASE( g_pJoystick );
	SAFE_RELEASE( g_pMouse );

	// DirectInput�̉���B
	SAFE_RELEASE( g_pDirectInput );
}

/*--������W�Ɍ������Ĉړ�--*/

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

/*--integer�\�[�g�p--*/

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

/*--�}�E�X���Z�b�g�A�b�v--*/

HRESULT SetupMouse( LPDIRECTINPUT8 aDirectInput , HWND hWnd )
{
	HRESULT		hr = S_OK;	//�߂�l�̐ݒ�

	// �V�X�e���}�E�X�f�o�C�X��T���Ċm�ۂ���B
	// �L�[�{�[�h�Ɏ��Ă���
	if( FAILED( hr = aDirectInput->CreateDevice( GUID_SysMouse, &g_pMouse, NULL ) ) ){ return ( hr ); }

    // �W���C�X�e�B�b�N�͊m�ۂł����H
	// �W���C�X�e�B�b�N���ڑ�����Ă��Ȃ������ꍇ�A�ォ��ڑ����Ă��F�����Ȃ��B
	if( NULL == g_pMouse )
	{
		MessageBox( NULL, "�}�E�X��������܂���ł����i����ȃo�J�ȁH�j\n���̃f�o�C�X�ő�������܂�", kERROR_MSG , MB_ICONERROR | MB_OK );
		return( hr );		//�����ŋA���Ă��܂�
	}

	// DirectInput����󂯎��f�[�^�t�H�[�}�b�g���}�E�X�i�g���d�l�j�ɐݒ肷��
	// c_dfDIMouse2��DirectInput���p�ӂ����O���[�o���ϐ���const�ݒ肳��Ă��ĕϐ��̒l�͕ύX�ł��Ȃ��B
	// ���̕�����Joystick�̏����Ɠ��l�B�����������̃}�E�X�����鎖�͍l�����Ă��Ȃ��B
	if( FAILED( hr = g_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
	{ return ( hr ); }

	// ���̃f�o�C�X�Ɠ��l�ɋ������x�����Z�b�g����B
	// �����ł̓t�H�A�O�����h�E�r���I���p���[�h�ɌŒ�
	// �Q�[���̑���̂��߂Ƀ}�E�X���g���ꍇ�́A�ʏ킱�̃��[�h�ł��܂�Ȃ��B
	// �������A���̃��[�h�ł́A�}�E�X�J�[�\���������Ȃ��Ȃ邽�߁A
	// �A�v���P�[�V�������Ń}�E�X�J�[�\����Ǝ��ɕ\��������ȂǍH�v���K�v�ɂȂ�̂ŗv���ӁB
	if( FAILED( hr = g_pMouse->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
	{
		CleanUpDirectInput();
		if( hr == DIERR_UNSUPPORTED )
		{
			MessageBox( NULL, "DirectInput���T�|�[�g���Ă��Ȃ��������x���ł��B", kERROR_MSG , MB_ICONERROR | MB_OK );
		}
		return ( hr );
	}

	// �}�E�X�̓��͂�L���ɂ���B
	g_pMouse->Acquire();

	return( S_OK );

}

/*--�}�E�X�̏�Ԃ��K��--*/

HRESULT getMouseState( HWND hWND , LPDIRECTINPUTDEVICE8 aMouse)
{
	HRESULT			hr;						// �V�X�e�����U���g
	DIMOUSESTATE2	aMouseState;			// DirectInput�̃}�E�X���\����

	// �}�E�X�������ꍇ�́A���炩�ɋ���B
	if( NULL == aMouse ){ return ( S_OK ); }

	// ���̓f�B�o�C�X�̏�Ԃ��m�ۂ��A�z��ɂ����߂�B
	// �L�[�{�[�h�Ƃ悭���Ă���
	ZeroMemory( &aMouseState, sizeof(DIMOUSESTATE2) );
	hr = g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &aMouseState );
	if( FAILED(hr) )
	{
		// �������͂������Ă����ꍇ�A�Ċm�ۂ����݂�B
		hr = aMouse->Acquire();
		while( hr == DIERR_INPUTLOST ) { hr = aMouse->Acquire(); }

		// �G���[���͑��̃A�v���P�[�V�������t�H�A�O�����h�ɂȂ��Ă����肷��̂ŉ������Ȃ��ŋA��B
		if( FAILED(hr) )
		{
			return( S_OK );
		}
	}

	return( S_OK );
}

/*--theBillbord�̏�����--*/

VOID ZeroBillbord( theBillbord *me )
{
	ZeroMemory( me, sizeof(theBillbord) );

	// �O�̂��߁Afloat���������l�������Ă���
	me->objPos		= (D3DXVECTOR3( 0.0f , 0.0f , 0.0f ));		// �r���{�[�h�̍��W�B
	me->objPitch	= 0.0f;
	me->objRoll		= 0.0f;
	me->objYaw		= 0.0f;

	me->objScal		= 1.0f;										// �r���{�[�h�̊g��k����

	me->f0			= 0.0f;										// �ėpfloat�l
	me->f1			= 0.0f;

	me->LDflag		= kTEX_NONE;								// �����t���O
}

/*--�摜�t�@�C�����e�N�X�`���Ƃ��ēǂݍ���--*/

HRESULT	LoadTextureFromImageFiles( LPCTSTR aFileName , LPDIRECT3DTEXTURE9* aTexture , LPCTSTR aSearchPath , LPDIRECT3DDEVICE9 theDevice )
{
	// �e�N�X�`���̐���
	// ������܂��J�����g�t�H���_��T���A������Ȃ��ꍇ�́umodels�v�t�H���_����T��
	if( FAILED( D3DXCreateTextureFromFile(	theDevice,aFileName,aTexture ) ) )
	{
		// �����J�����g�f�B���N�g���Ƀt�@�C��������������Amodels�t�H���_���m���߂�B
		// �o�b�N�X���b�V��(\)����d�Ȃ��Ă���̂́A�G�X�P�[�v�V�[�P���X���L���ɂȂ��Ă��邽��
		TCHAR strTexture[MAX_PATH];

		DWORD aPathLength = lstrlen( aSearchPath );

		lstrcpyn( strTexture, aSearchPath, MAX_PATH );
		lstrcpyn( strTexture + aPathLength, aFileName, MAX_PATH - aPathLength );

		// �e�N�X�`��������������A�ЂƂ�̃f�B���N�g�����m���߂�B
		if( FAILED(D3DXCreateTextureFromFile( theDevice, strTexture, aTexture )) ){ return(E_FAIL); }
	}

	return( S_OK );
}

/*--�r���{�[�h�̕`��--*/

VOID drawFVF(LPDIRECT3DDEVICE9 theDevice,theBillbord *me,LPDIRECT3DTEXTURE9 theTexture,LPDIRECT3DVERTEXBUFFER9 theVertexBuffer)
{
	SetupFVFMatrix(theDevice,me);	//�I�u�W�F�N�g�̕\���p�s���ݒ�

	int	aTex = me->TexType;

	theDevice->SetTexture( 0, theTexture );
	SetFVFTexturePosition( theVertexBuffer,me->TexType,me->TexIndex );

	theDevice->SetStreamSource( 0, theVertexBuffer , 0, sizeof(theCUSTOMVERTEX) );
	theDevice->SetFVF( kD3DFVF_CUSTOMVERTEX );
	theDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

/*--FVF�̂��߂̈ʒu�Ɖ�]�̐ݒ�B�g��k���ɂ��Ή�����B--*/

VOID SetupFVFMatrix(LPDIRECT3DDEVICE9 theDevice,theBillbord *me)
{
	D3DXMATRIX	theSclMatrix;		// �g�k�p�s��
	D3DXMATRIX	thePosMatrix;		// �ʒu�p�s��
	D3DXMATRIX	theRotMatrix;		// ��]�p�s��
	D3DXMATRIX	theResultMatrix;	// ���ʗp�s��

	D3DXVECTOR3	vecDir;				// �r���{�[�h�ʒu�I�t�Z�b�g�p

	// �g��k���p�̍s���ݒ�
	D3DXMatrixScaling( &theSclMatrix , me->objScal , me->objScal , me->objScal );

	// ���s�ړ��Ɖ�]�̍s���ݒ�
	D3DXMatrixTranslation( &thePosMatrix , me->objPos.x , me->objPos.y , me->objPos.z);

	D3DXMatrixRotationYawPitchRoll( &theRotMatrix ,  me->objYaw , me->objRoll , me->objPitch );	

	// �s�����������
	theResultMatrix = theSclMatrix * theRotMatrix * thePosMatrix;

	// �`��f�B�o�C�X�֓K�p
	theDevice->SetTransform( D3DTS_WORLD , &theResultMatrix );
}

/*--FVF�̃e�N�X�`�����W���Z�b�g--*/

HRESULT	SetFVFTexturePosition( LPDIRECT3DVERTEXBUFFER9 aFVFBuffer , DWORD aType , DWORD aChar )
{
    theCUSTOMVERTEX*	tmpVertex;		// ���_�o�b�t�@���b�N�p
	theTexPosRECT		aTexRECT;		// �e�N�X�`�����W�󂯎��p

	switch( aType )
	{
		case kTEX_ASCII:
			SetCharTextureRECT( aChar , &aTexRECT );
		break;
	}

	// ���_�o�b�t�@�����b�N
	if( FAILED( aFVFBuffer->Lock( 0, 0, (void**)&tmpVertex, 0 ) ) ){ return( E_FAIL ); }

	// ���_�͎l�A���_���W�A���_�F�A�e�N�X�`�����W��ݒ肷��
	// D3DPT_TRIANGLESTRIP�ŕ`�悷��̂Œ��_�̔ԍ��ƍ��W�̑Ή��ɒ���
	tmpVertex[0].tu = aTexRECT.u0 ; tmpVertex[0].tv = aTexRECT.v0;
	tmpVertex[1].tu = aTexRECT.u1 ; tmpVertex[1].tv = aTexRECT.v0;
	tmpVertex[2].tu = aTexRECT.u0 ; tmpVertex[2].tv = aTexRECT.v1;
	tmpVertex[3].tu = aTexRECT.u1 ; tmpVertex[3].tv = aTexRECT.v1;

	// �������݂��I������̂Œ��_�o�b�t�@�̃��b�N����������
	aFVFBuffer->Unlock();

	return( S_OK );
}

/*--�p�����̃e�N�X�`�����W�ݒ�--*/

VOID SetCharTextureRECT( DWORD aChar , theTexPosRECT *aRECT )
{
	const float	kCellSize = 1.0 / 16.0f;

	int		upper,lower;

	//���h�~
	aChar &= 0xff;
	upper = aChar / 0x10;
	lower = aChar & 0x0f;
	aRECT->u1 = kCellSize * lower; aRECT->u0 = aRECT->u1 + kCellSize; if( aRECT->u0 > 1.0f ){ aRECT->u0 = 1.0f ;}
	aRECT->v1 = kCellSize * upper; aRECT->v0 = aRECT->v1 + kCellSize; if( aRECT->v0 > 1.0f ){ aRECT->v0 = 1.0f ;}

}