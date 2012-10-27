#ifndef ___Global
#define ___Global

#ifdef ___GLOBAL_DEFINE
#define EXTERN /* */
#define VAL(x) =(x)
#else
#define EXTERN extern
#define VAL(x) /* */
#endif

#include "speedrace_head1.h"
#include "CarClass.h"
#include "ConFps.h"
#include "MapClass.h"
#include "RaceClass.h"
#include "ModelClass.h"
#include "GameResult.h"

//--Direct3D
EXTERN LPDIRECT3D9             g_pD3D			VAL(NULL);			//D3Dオブジェクト
EXTERN LPDIRECT3DDEVICE9       g_pD3DDevice		VAL(NULL);			//D3Dデバイス

EXTERN D3DPRESENT_PARAMETERS	g_D3DPParams;						//プレゼンテーションパラメータの格納

EXTERN BOOL					g_FullScreenFlag	VAL(TRUE);			//フルスクリーンフラグ。もちろんTRUEでフルスクリーン、FALSEでウィンドウモード
/*
※注意　この値をTRUEにするとフルスクリーンモードとしてビルトされ、バグが発生した場合に画面が占領されていて停止できなくなり場合があります。
バグを完全に修正してからこの値をTRUEにしてください。
*/

//--DirectInput
EXTERN LPDIRECTINPUT8			g_pDirectInput	VAL(NULL);		// DirectInputオブジェクト

EXTERN BYTE					g_pKeyState[256];					// DirectInputのキーボードステータスバッファ

EXTERN LPDIRECTINPUTDEVICE8	g_pKeyboard		VAL(NULL);				// キーボードディバイス
EXTERN LPDIRECTINPUTDEVICE8	g_pJoystick		VAL(NULL);				// ジョイスティックディバイス
EXTERN LPDIRECTINPUTDEVICE8	g_pMouse		VAL(NULL);				// マウスディバイス

//------------------------------------------------------------

EXTERN ModelClass	*g_ObjModels[kNUM_OF_MODELS];		//モデルの種類だけ

//------------------------------------------------------------
EXTERN int		g_GameMode		VAL(kMODE_INIT);						//ゲームモードの初期化
EXTERN int		g_GameCounter	VAL(0);								//ゲーム用カウンターの初期化


//視点（＝リスナー）の位置情報など
EXTERN D3DXVECTOR3	gEyePt		VAL((D3DXVECTOR3(0.0f,100.0f,-100.0f)));	//カメラ位置
EXTERN D3DXVECTOR3	gLookatPt	VAL((D3DXVECTOR3(0.0f,0.0f,0.0f)));		//注視点

EXTERN D3DXVECTOR3	gUpVec		VAL((D3DXVECTOR3(0.0f,1.0f,0.0f)));		//カメラの上方方向

//メニュー用のカメラの情報
EXTERN D3DXVECTOR3	gBillUp			VAL((D3DXVECTOR3(0.0f,1.0f,0.0f)));
EXTERN D3DXVECTOR3	gBillEyePt		VAL((D3DXVECTOR3(0.0f,0.0f,1500.0f)));
EXTERN D3DXVECTOR3	gBillLookatPt	VAL((D3DXVECTOR3(0.0f,0.0f,0.0f)));

//ライトの設定
EXTERN D3DXVECTOR3	gLightVec		VAL((D3DXVECTOR3(0.0f,0.0f,-1.0f)));
EXTERN D3DLIGHT9	gLightLight;

//レースの宣言
EXTERN RaceClass	*theRace;

//fps調整用クラス
EXTERN ConFps		*CFps;

EXTERN MenuClass	*theMenu;
EXTERN GameResult	*theResult;
EXTERN OpeningClass	*theOpening;
EXTERN CreditClass	*theCredit;

EXTERN HANDLE	hMapPosFile;

EXTERN float	MapPosBufX[3000];
EXTERN float	MapPosBufY[3000];
EXTERN float	MapPosBufZ[3000];

EXTERN DWORD dwReadByte;
EXTERN int NumBuf;
EXTERN int SizeBuf;

//フォルダ設定など
EXTERN	TCHAR*	kModelsPath			VAL(TEXT(".\\models\\"));		//modelsフォルダ
EXTERN	int		kModelsPathLength	VAL(lstrlen( kModelsPath ));

EXTERN	TCHAR*	kImagesPath			VAL(TEXT(".\\images\\"));		//imagesフォルダ
EXTERN	int		kImagesPathLength	VAL(lstrlen( kImagesPath ));

EXTERN	TCHAR*	kCourseIndex[kCTYPE_NUM][3];

EXTERN	int		gCourseType;

EXTERN	DWORD	gGameResultTime;

#endif