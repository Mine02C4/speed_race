//speedrace_head1.h
//このヘッダファイルは主にWindowsAPI等のインクルード、マクロの定義、定数の定義、構造体の宣言または定義、関数プロトタイプの宣言を行います

#ifndef ___speedrace_head1
#define ___speedrace_head1

//もし、DrawTextAが使用できないバージョンのDirectX SDKだった場合は、以下の行の「//」を削除して有効にする
//これによってDrawTextAを使用した部分がコンパイルされなくなる
//#define __noDRAW_TEXT_A__

/*----------------*/
/*--インクルード--*/
/*----------------*/

#include <windows.h>
#include <basetsd.h>

#include <d3dx9.h>
#include <dinput.h>

#include <tchar.h>

#include <string>
#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <cstdlib>
#include <ctime>

#include <time.h>

#include <mmsystem.h>

/*----------------*/
/*--マクロの定義--*/
/*----------------*/

#define SAFE_DELETE(me)			{ if(me){ delete (me);		( me )=NULL; } }
#define SAFE_DELETE_ARRAY(me)	{ if(me){ delete[] (me);	( me )=NULL; } }
#define SAFE_RELEASE(me)		{ if(me){ (me)->Release();	( me )=NULL; } }

/*--------------*/
/*--定数の定義--*/
/*--------------*/

//------------//
//--文字定数--//
//------------//

//ウィンドウの名前
#define kWINDOW_TITLE		TEXT("SPEED RACE")
#define	kERROR_MSG			TEXT("SPEED RACE エラー")

//Xファイル
#define kXCAR_AE86			TEXT("ae86.x")
#define kXMAP_TOUGE			TEXT("tougeway.x")
#define kXWALL_TOUGE		TEXT("tougewall.x")
#define kXEND_TOUGE			TEXT("tougeend.x")
#define kXMAP_GAKE			TEXT("gakeway.x")
#define kXWALL_GAKE			TEXT("gakewall.x")
#define kXEND_GAKE			TEXT("gakeend.x")
#define kXMENU_KYOUSOUKAISI	TEXT("kyousoukaisi.x")
#define kXMENU_SOUSAHOUHOU	TEXT("sousahouhou.x")
#define kXMENU_RANKING		TEXT("ranking.x")
#define kXMENU_SYUURYOU		TEXT("syuuryou.x")
#define kXMENU_MENU			TEXT("menu.x")
#define kXMENU_MODORU		TEXT("modoru.x")
#define kXMENU_TOUGE		TEXT("menutouge.x")
#define kXMENU_GAKE			TEXT("menugake.x")
#define kXMENU_POINTER		TEXT("pointer.x")
#define kXMENU_BACK			TEXT("menuback.x")
#define kXMENU_TOUGE_RANK	TEXT("menurankingcoursetouge.x")
#define kXMENU_GAKE_RANK	TEXT("menurankingcoursegake.x")
#define kXINFO_HOWTOPLAY	TEXT("howtoplay.x")
#define kXINFO_RANKING		TEXT("inforanking.x")
#define kXMESH_TOUGE		TEXT("touge.x")
#define kXMESH_GAKE			TEXT("gake.x")
#define kXCOUNTER_KAISI		TEXT("counterkaisi.x")
#define kXCOUNTER_1			TEXT("counter1.x")
#define kXCOUNTER_2			TEXT("counter2.x")
#define kXCOUNTER_3			TEXT("counter3.x")
#define kXRACEFINISH		TEXT("racefinish.x")
#define kXRESULT_NUMROLL	TEXT("numroll.x")
#define kXRESULT_KORON		TEXT("koron.x")
#define kXRESULT_NAME_IN	TEXT("namaenyuuryoku.x")
#define kXRESULT_FORM		TEXT("form.x")
#define kXOPENING_SPEEDLOGO	TEXT("speedracelogo.x")
#define kXOPENING_NPCALOGO	TEXT("npcalogo.x")
#define kXCREDIT_IMAGE		TEXT("credit.x")

//テキストファイル
#define kTEXT_RANKING_TOUGE	TEXT("ranking-touge.txt")
#define kTEXT_RANKING_GAKE	TEXT("ranking-gake.txt")

//テクスチャ
#define kFILE_TEX_ASCII		TEXT("ASCII.dds")

//--------------//
//--設定用定数--//
//--------------//

#define kD3DFVF_CUSTOMVERTEX	(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//------------//
//--数値定数--//
//------------//

//ウィンドウの形など
#define	kWINDOW_WIDTH	800
#define	kWINDOW_HEIGHT	600
#define	kWINDOW_TOP		50
#define	kWINDOW_LEFT	50

#define	kSTR_LENGTH		512

#define	kSLEEP			10		//10/1000msec. Sleepする時間

#define	kTIMER_ID		1

#define	kNO_PARENT		-1		//親の無いオブジェクトが持つ親番号

//単位時間あたりの移動量設定
#define	kDIV_UNIT		60
#define	kTIMER_LAP		( 1000 / kDIV_UNIT )

#define	kCAR_KASOKU				//車の加速度

#define SHORTDISTANCE	1.0f	//跳ね返る距離

//この値に度を掛けるとラジアンになる
#define	kDEG_TO_RAD		( D3DX_PI * 2.0f / 360.0f)

//ついでに
#define	kPI_1FOURTH		( D3DX_PI / 4.0f )			// 45度
#define	kPI_HALF		( D3DX_PI / 2.0f )			// 90度
#define kPI_ROUND		( D3DX_PI * 2.0f )			//一週
//となる

#define kNUM_BACK_MENU	100			//背景の枚数

#define	kNUM_MAX_PLAYERNAME	8		//プレイヤー名の最高文字数

#define kNUM_MAX_RANKING 22

#define kSIZE_OF_BILLBOARD 64.0		//ビルボード文字のサイズ

//ゲームの状況を記憶するための定数
enum{
	kMODE_INIT = 0,			//準備（起動時とか）
	kMODE_OPENING,			//オープニング中
	kMODE_DATASELECT,		//ロードするデータの選択
	kMODE_INIT_MENU,
	kMODE_MENU,				//メニュー
	kMODE_INIT_RACE,		//レースを初期化
	kMODE_RACE,				//レース中
	kMODE_END_RACE,
	kMODE_RESULT,			//結果表示
	kMODE_END_RESULT,
	kMODE_CREDIT,			//クレジット
	kMODE_EXIT,				//強制終了
	kMODE_SENTINEL			//番兵
};

//theObject構造体で使われる表示モデル識別用定数
enum{
	kMDL_NONE = 0,			//モデルなし
	kMDL_MENU1,				//メニュー用のオブジェクト
	kMDL_MENU2,				//
	kMDL_MENU3,				//
	kMDL_MENU4,				//
	kMDL_MENU5,				//
	kMDL_MENU6,				//
	kMDL_M_CAR,				//自車
	kMDL_M_TIRE_F_R,		//自車の前右のタイヤ
	kMDL_M_TIRE_F_L,		//自車の前左のタイヤ
	kMDL_M_TIRE_R_R,		//自車の後右のタイヤ
	kMDL_M_TIRE_R_L,		//自車の後左のタイヤ
	kMDL_E_CAR,				//敵車
	kMDL_E_TIRE_F_R,		//敵車の前右のタイヤ
	kMDL_E_TIRE_F_L,		//敵車の前左のタイヤ
	kMDL_E_TIRE_R_R,		//敵車の後右のタイヤ
	kMDL_E_TIRE_R_L,		//敵車の後左のタイヤ
	kMDL_G_MAP01,			// 地形オブジェクト
	kMDL_G_MAP02,			//
	kMDL_G_MAP03,			//
	kMDL_G_MAP04,			//
	kMDL_G_MAP05,			//
	kMDL_W_WALL01,			//壁オブジェクト
	kMDL_W_WALL02,			//
	kMDL_W_WALL03,			//
	kMDL_W_WALL04,			//
	kMDL_W_WALL05,			//
	kMDL_E_END,				//終了地点
	kNUM_OF_MODELS,			//モデルの種類の数
	kMDL_END				//番兵
};

//メニュー項目識別用
enum{
	kMENU_KYOUSOUKAISI = 0,
	kMENU_SOUSAHOUHOU,
	kMENU_RANKING,
	kMENU_SYUURYOU,
	kMENU_MENU,
	kMENU_MODORU,
	kMENU_TOUGE,
	kMENU_GAKE,
	kINFO_HOWTOPLAY,
	kINFO_RANKING,
	kMENU_POINTER,
	kNUM_OF_MENU,
	kINFO_NAME_IN,
	kINFO_FORM,
	kNUM_OF_RESULT,
	kMENU_END
};

//theObject構造体で使われるコントロールの状態識別用定数
enum{
	kCON_OFF = 0,
	kCON_ON,
	kCON_END
};

//ハンドル用の定数
enum{
	kHAN_NORMAL = 0,
	kHAN_RIGHT,
	kHAN_LEFT,
	kHAN_END
};

//対象に応じたレンダリングステート設定のための定数
enum{
	kRS_MESH = 0,	// メッシュ描画用
	kRS_ALPHA,		// αブレンディング（文字描画などで使用）
	kRS_ADDBLEND,	// 加算合成（爆発セルなどで使用）
	kRS_INFO,		// 情報レイヤ用
	kRS_MENU,		//メニュー用
	kRS_BACK,		//背景用
	kRS_POINTER,	//ポインター用
	kRS_BRIGHT,
	kRS_END			// 番兵
};

enum{
	kTSS_LINEAR = 0,	// 線形補完 
	kTSS_POINT,			// 補完なし
	kTSS_END			// 番兵
};

//レースタイプを設定するための定数
enum{
	kRACEMODE_MAIN = 0,
	kRACEMODE_TIMEATTACK,
	kRACEMODE_END
};

//コースタイプを設定するための定数
enum{
	kCTYPE_TOUGE = 0,
	kCTYPE_GAKE,
	kCTYPE_NUM,
	kCTYPE_END
};

//メニュー用の状態用の定数
enum{
	kPART_INFO_OUT = 0,
	kPART_INFO_IN,
	kPART_INFO_HEAD,
	kPART_INFO_CUSTOMPOS,
	kPART_INFO_END
};

//メニューの状態を決める定数
enum{
	kMENU_INFO_MAIN = 0,	//メインメニュー
	kMENU_INFO_SET_COURSE,
	kMENU_INFO_SOUSAHOUHOU,
	kMENU_INFO_RANKING,
	kMENU_INFO_BAN
};

//レースの状態を保存
enum{
	kRACE_INFO_COUNT = 0,
	kRACE_INFO_RACE,
	kRACE_INFO_FINISH,
	kRACE_INFO_END
};

//結果表示の状態
enum{
	kRESULT_INFO_SPIN = 0,
	kRESULT_INFO_STOP,
	kRESULT_INFO_NAME_SET,
	kRESULT_INFO_END
};

enum{
	kTEX_NONE = 0,
	kTEX_ASCII,
	kNUM_OF_TEXTURES,
	kTEX_END
};

/*----------------*/
/*--構造体の定義--*/
/*----------------*/

//オブジェクトの状態を記憶する構造体
struct	theObject
{
	D3DXVECTOR3	objPos;		// オブジェクトの座標。
	float		objYaw;		// ヨー
	float		objRoll;	// ロール
	float		objPitch;	// ピッチ

	float		objScal;	// オブジェクトの拡大縮小率

	int			objType;	// オブジェクトの種類

	int			parent;		// 親オブジェクトの番号
};

//ビルボード用構造体
struct	theBillbord
{
	D3DXVECTOR3	objPos;		// ビルボードの座標
	float		objYaw;		// ヨー
	float		objRoll;	// ロール
	float		objPitch;	// ピッチ

	float		objScal;	// ビルボードの拡大縮小率

	int			TexType;	// テクスチャのタイプ
	int			TexIndex;	// テクスチャのインデックス（位置を数値で表したもの）	
	int			objType;	// ビルボードの種類

	int			LDflag;		// 生死フラグ

	float		f0;			// 汎用float値
	float		f1;

	int			U0;			// 汎用カウンタ
	int			U1;
};

//カスタムバーテックス
struct	theCUSTOMVERTEX
{
    D3DXVECTOR3 position;			// 頂点座標
    D3DCOLOR    color;				// 頂点色
    FLOAT       tu, tv;				// 頂点とテクスチャ座標の対応
};

//テクスチャ上の座標を指定するための構造体
struct	theTexPosRECT
{
	FLOAT		u0,v0,u1,v1;
};

//ランキング用構造体
struct	theRanking
{
	int		RankIndex;
	char	PlayerName[kNUM_MAX_PLAYERNAME];
	float	theTime;
};

/*----------------*/
/*--クラスの宣言--*/
/*----------------*/

/*--車のクラス--*/
class CarClass;

/*--FPSの調整・管理クラス--*/
class ConFps;

/*--地面データのクラス--*/
class MapClass;

/*--レースのクラス--*/
class RaceClass;

/*--モデル格納用クラス--*/
class ModelClass;

/*--メニューのクラス--*/
class MenuClass;

/*--メニュー項目のクラス--*/
class PartMenu;

/*--メニューの背景管理クラス--*/
class MenuBack;

/*--メニュー情報のクラス--*/
class CourseInfo;

/*--ゲームの結果--*/
class GameResult;

/*--オープニング--*/
class OpeningClass;

/*--クレジット--*/
class CreditClass;

/*--------------------*/
/*--関数プロトタイプ--*/
/*--------------------*/

//定義するたんびにコメントアウトを解除

//Direct3D関連
HRESULT	InitDirect3D( HWND );								// D3DXとかの初期化
VOID	CleanUpDirect3D( void );							// モデル類の後始末

VOID	SetupRanderStates( LPDIRECT3DDEVICE9 , DWORD );		// シェーディングモードなどの設定
VOID	SetupTexSamplerState( LPDIRECT3DDEVICE9 , DWORD );	// テクスチャサンプラーステートを設定

VOID	SetupLights(LPDIRECT3DDEVICE9,D3DXVECTOR3,D3DLIGHT9,int);					// ライトのセットアップ
VOID	SetupWorldMatrix( LPDIRECT3DDEVICE9 ,D3DXVECTOR3* , D3DXVECTOR3* ,D3DXVECTOR3* );//描画行列の設定

VOID	drawMesh( LPDIRECT3DDEVICE9 , theObject * ,ModelClass*);
VOID	drawFVF(LPDIRECT3DDEVICE9,theBillbord*,LPDIRECT3DTEXTURE9,LPDIRECT3DVERTEXBUFFER9);

HRESULT	SetFVFTexturePosition( LPDIRECT3DVERTEXBUFFER9 , DWORD , DWORD );
VOID	SetCharTextureRECT(DWORD,theTexPosRECT*);	// 文字キャラクタのテクスチャ座標設定
VOID	SetCellTextureRECT(DWORD,theTexPosRECT*);	// 効果用セルのテクスチャ座標設定
VOID	SetupFVFMatrix(LPDIRECT3DDEVICE9,theBillbord*);

//DirectInput関連
HRESULT	InitDirectInput(  HWND , HINSTANCE );			//DirectInputの初期化
VOID	CleanUpDirectInput( void );						//DirectInputの後始末

HRESULT	SetupKeybord( LPDIRECTINPUT8 , HWND );			//キーボードの初期化
HRESULT SetupMouse( LPDIRECTINPUT8 , HWND );			//マウスの初期化

int		GetKeybordState( LPDIRECTINPUTDEVICE8 );		//キー入力の取得
HRESULT getMouseState( HWND , LPDIRECTINPUTDEVICE8);	//マウス情報の取得

//構造体とか
VOID	SetupSitus( theObject*, int );

VOID	ZeroObject( theObject * );
VOID	ZeroBillbord(theBillbord*);

//表示のための座標系変換
VOID	SetupObjMatrix(LPDIRECT3DDEVICE9 , theObject * );

//アプリケーション関連
HRESULT MsgLoop( HWND hWnd );								//メッセージループ
VOID	CleanUpApplication( WNDCLASSEX * );					//アプリケーションの後始末
LRESULT WINAPI MsgProc( HWND , UINT , WPARAM , LPARAM );	//メッセージハンドラ

// ゲームのメイン部分
HRESULT	GameBody( HWND );

//移動用関数
D3DXVECTOR3	MoveForward(D3DXVECTOR3, float, float);
D3DXVECTOR3	MoveTo(D3DXVECTOR3,D3DXVECTOR3,float,int*);

//ソート用関数
int sort_ranking(const void* a, const void* b);

HRESULT	LoadTextureFromImageFiles( LPCTSTR , LPDIRECT3DTEXTURE9* , LPCTSTR  , LPDIRECT3DDEVICE9 );	//テクスチャのロード

#endif