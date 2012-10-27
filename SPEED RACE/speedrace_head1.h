//speedrace_head1.h
//���̃w�b�_�t�@�C���͎��WindowsAPI���̃C���N���[�h�A�}�N���̒�`�A�萔�̒�`�A�\���̂̐錾�܂��͒�`�A�֐��v���g�^�C�v�̐錾���s���܂�

#ifndef ___speedrace_head1
#define ___speedrace_head1

//�����ADrawTextA���g�p�ł��Ȃ��o�[�W������DirectX SDK�������ꍇ�́A�ȉ��̍s�́u//�v���폜���ėL���ɂ���
//����ɂ����DrawTextA���g�p�����������R���p�C������Ȃ��Ȃ�
//#define __noDRAW_TEXT_A__

/*----------------*/
/*--�C���N���[�h--*/
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
/*--�}�N���̒�`--*/
/*----------------*/

#define SAFE_DELETE(me)			{ if(me){ delete (me);		( me )=NULL; } }
#define SAFE_DELETE_ARRAY(me)	{ if(me){ delete[] (me);	( me )=NULL; } }
#define SAFE_RELEASE(me)		{ if(me){ (me)->Release();	( me )=NULL; } }

/*--------------*/
/*--�萔�̒�`--*/
/*--------------*/

//------------//
//--�����萔--//
//------------//

//�E�B���h�E�̖��O
#define kWINDOW_TITLE		TEXT("SPEED RACE")
#define	kERROR_MSG			TEXT("SPEED RACE �G���[")

//X�t�@�C��
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

//�e�L�X�g�t�@�C��
#define kTEXT_RANKING_TOUGE	TEXT("ranking-touge.txt")
#define kTEXT_RANKING_GAKE	TEXT("ranking-gake.txt")

//�e�N�X�`��
#define kFILE_TEX_ASCII		TEXT("ASCII.dds")

//--------------//
//--�ݒ�p�萔--//
//--------------//

#define kD3DFVF_CUSTOMVERTEX	(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//------------//
//--���l�萔--//
//------------//

//�E�B���h�E�̌`�Ȃ�
#define	kWINDOW_WIDTH	800
#define	kWINDOW_HEIGHT	600
#define	kWINDOW_TOP		50
#define	kWINDOW_LEFT	50

#define	kSTR_LENGTH		512

#define	kSLEEP			10		//10/1000msec. Sleep���鎞��

#define	kTIMER_ID		1

#define	kNO_PARENT		-1		//�e�̖����I�u�W�F�N�g�����e�ԍ�

//�P�ʎ��Ԃ�����̈ړ��ʐݒ�
#define	kDIV_UNIT		60
#define	kTIMER_LAP		( 1000 / kDIV_UNIT )

#define	kCAR_KASOKU				//�Ԃ̉����x

#define SHORTDISTANCE	1.0f	//���˕Ԃ鋗��

//���̒l�ɓx���|����ƃ��W�A���ɂȂ�
#define	kDEG_TO_RAD		( D3DX_PI * 2.0f / 360.0f)

//���ł�
#define	kPI_1FOURTH		( D3DX_PI / 4.0f )			// 45�x
#define	kPI_HALF		( D3DX_PI / 2.0f )			// 90�x
#define kPI_ROUND		( D3DX_PI * 2.0f )			//��T
//�ƂȂ�

#define kNUM_BACK_MENU	100			//�w�i�̖���

#define	kNUM_MAX_PLAYERNAME	8		//�v���C���[���̍ō�������

#define kNUM_MAX_RANKING 22

#define kSIZE_OF_BILLBOARD 64.0		//�r���{�[�h�����̃T�C�Y

//�Q�[���̏󋵂��L�����邽�߂̒萔
enum{
	kMODE_INIT = 0,			//�����i�N�����Ƃ��j
	kMODE_OPENING,			//�I�[�v�j���O��
	kMODE_DATASELECT,		//���[�h����f�[�^�̑I��
	kMODE_INIT_MENU,
	kMODE_MENU,				//���j���[
	kMODE_INIT_RACE,		//���[�X��������
	kMODE_RACE,				//���[�X��
	kMODE_END_RACE,
	kMODE_RESULT,			//���ʕ\��
	kMODE_END_RESULT,
	kMODE_CREDIT,			//�N���W�b�g
	kMODE_EXIT,				//�����I��
	kMODE_SENTINEL			//�ԕ�
};

//theObject�\���̂Ŏg����\�����f�����ʗp�萔
enum{
	kMDL_NONE = 0,			//���f���Ȃ�
	kMDL_MENU1,				//���j���[�p�̃I�u�W�F�N�g
	kMDL_MENU2,				//
	kMDL_MENU3,				//
	kMDL_MENU4,				//
	kMDL_MENU5,				//
	kMDL_MENU6,				//
	kMDL_M_CAR,				//����
	kMDL_M_TIRE_F_R,		//���Ԃ̑O�E�̃^�C��
	kMDL_M_TIRE_F_L,		//���Ԃ̑O���̃^�C��
	kMDL_M_TIRE_R_R,		//���Ԃ̌�E�̃^�C��
	kMDL_M_TIRE_R_L,		//���Ԃ̌㍶�̃^�C��
	kMDL_E_CAR,				//�G��
	kMDL_E_TIRE_F_R,		//�G�Ԃ̑O�E�̃^�C��
	kMDL_E_TIRE_F_L,		//�G�Ԃ̑O���̃^�C��
	kMDL_E_TIRE_R_R,		//�G�Ԃ̌�E�̃^�C��
	kMDL_E_TIRE_R_L,		//�G�Ԃ̌㍶�̃^�C��
	kMDL_G_MAP01,			// �n�`�I�u�W�F�N�g
	kMDL_G_MAP02,			//
	kMDL_G_MAP03,			//
	kMDL_G_MAP04,			//
	kMDL_G_MAP05,			//
	kMDL_W_WALL01,			//�ǃI�u�W�F�N�g
	kMDL_W_WALL02,			//
	kMDL_W_WALL03,			//
	kMDL_W_WALL04,			//
	kMDL_W_WALL05,			//
	kMDL_E_END,				//�I���n�_
	kNUM_OF_MODELS,			//���f���̎�ނ̐�
	kMDL_END				//�ԕ�
};

//���j���[���ڎ��ʗp
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

//theObject�\���̂Ŏg����R���g���[���̏�Ԏ��ʗp�萔
enum{
	kCON_OFF = 0,
	kCON_ON,
	kCON_END
};

//�n���h���p�̒萔
enum{
	kHAN_NORMAL = 0,
	kHAN_RIGHT,
	kHAN_LEFT,
	kHAN_END
};

//�Ώۂɉ����������_�����O�X�e�[�g�ݒ�̂��߂̒萔
enum{
	kRS_MESH = 0,	// ���b�V���`��p
	kRS_ALPHA,		// ���u�����f�B���O�i�����`��ȂǂŎg�p�j
	kRS_ADDBLEND,	// ���Z�����i�����Z���ȂǂŎg�p�j
	kRS_INFO,		// ��񃌃C���p
	kRS_MENU,		//���j���[�p
	kRS_BACK,		//�w�i�p
	kRS_POINTER,	//�|�C���^�[�p
	kRS_BRIGHT,
	kRS_END			// �ԕ�
};

enum{
	kTSS_LINEAR = 0,	// ���`�⊮ 
	kTSS_POINT,			// �⊮�Ȃ�
	kTSS_END			// �ԕ�
};

//���[�X�^�C�v��ݒ肷�邽�߂̒萔
enum{
	kRACEMODE_MAIN = 0,
	kRACEMODE_TIMEATTACK,
	kRACEMODE_END
};

//�R�[�X�^�C�v��ݒ肷�邽�߂̒萔
enum{
	kCTYPE_TOUGE = 0,
	kCTYPE_GAKE,
	kCTYPE_NUM,
	kCTYPE_END
};

//���j���[�p�̏�ԗp�̒萔
enum{
	kPART_INFO_OUT = 0,
	kPART_INFO_IN,
	kPART_INFO_HEAD,
	kPART_INFO_CUSTOMPOS,
	kPART_INFO_END
};

//���j���[�̏�Ԃ����߂�萔
enum{
	kMENU_INFO_MAIN = 0,	//���C�����j���[
	kMENU_INFO_SET_COURSE,
	kMENU_INFO_SOUSAHOUHOU,
	kMENU_INFO_RANKING,
	kMENU_INFO_BAN
};

//���[�X�̏�Ԃ�ۑ�
enum{
	kRACE_INFO_COUNT = 0,
	kRACE_INFO_RACE,
	kRACE_INFO_FINISH,
	kRACE_INFO_END
};

//���ʕ\���̏��
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
/*--�\���̂̒�`--*/
/*----------------*/

//�I�u�W�F�N�g�̏�Ԃ��L������\����
struct	theObject
{
	D3DXVECTOR3	objPos;		// �I�u�W�F�N�g�̍��W�B
	float		objYaw;		// ���[
	float		objRoll;	// ���[��
	float		objPitch;	// �s�b�`

	float		objScal;	// �I�u�W�F�N�g�̊g��k����

	int			objType;	// �I�u�W�F�N�g�̎��

	int			parent;		// �e�I�u�W�F�N�g�̔ԍ�
};

//�r���{�[�h�p�\����
struct	theBillbord
{
	D3DXVECTOR3	objPos;		// �r���{�[�h�̍��W
	float		objYaw;		// ���[
	float		objRoll;	// ���[��
	float		objPitch;	// �s�b�`

	float		objScal;	// �r���{�[�h�̊g��k����

	int			TexType;	// �e�N�X�`���̃^�C�v
	int			TexIndex;	// �e�N�X�`���̃C���f�b�N�X�i�ʒu�𐔒l�ŕ\�������́j	
	int			objType;	// �r���{�[�h�̎��

	int			LDflag;		// �����t���O

	float		f0;			// �ėpfloat�l
	float		f1;

	int			U0;			// �ėp�J�E���^
	int			U1;
};

//�J�X�^���o�[�e�b�N�X
struct	theCUSTOMVERTEX
{
    D3DXVECTOR3 position;			// ���_���W
    D3DCOLOR    color;				// ���_�F
    FLOAT       tu, tv;				// ���_�ƃe�N�X�`�����W�̑Ή�
};

//�e�N�X�`����̍��W���w�肷�邽�߂̍\����
struct	theTexPosRECT
{
	FLOAT		u0,v0,u1,v1;
};

//�����L���O�p�\����
struct	theRanking
{
	int		RankIndex;
	char	PlayerName[kNUM_MAX_PLAYERNAME];
	float	theTime;
};

/*----------------*/
/*--�N���X�̐錾--*/
/*----------------*/

/*--�Ԃ̃N���X--*/
class CarClass;

/*--FPS�̒����E�Ǘ��N���X--*/
class ConFps;

/*--�n�ʃf�[�^�̃N���X--*/
class MapClass;

/*--���[�X�̃N���X--*/
class RaceClass;

/*--���f���i�[�p�N���X--*/
class ModelClass;

/*--���j���[�̃N���X--*/
class MenuClass;

/*--���j���[���ڂ̃N���X--*/
class PartMenu;

/*--���j���[�̔w�i�Ǘ��N���X--*/
class MenuBack;

/*--���j���[���̃N���X--*/
class CourseInfo;

/*--�Q�[���̌���--*/
class GameResult;

/*--�I�[�v�j���O--*/
class OpeningClass;

/*--�N���W�b�g--*/
class CreditClass;

/*--------------------*/
/*--�֐��v���g�^�C�v--*/
/*--------------------*/

//��`���邽��тɃR�����g�A�E�g������

//Direct3D�֘A
HRESULT	InitDirect3D( HWND );								// D3DX�Ƃ��̏�����
VOID	CleanUpDirect3D( void );							// ���f���ނ̌�n��

VOID	SetupRanderStates( LPDIRECT3DDEVICE9 , DWORD );		// �V�F�[�f�B���O���[�h�Ȃǂ̐ݒ�
VOID	SetupTexSamplerState( LPDIRECT3DDEVICE9 , DWORD );	// �e�N�X�`���T���v���[�X�e�[�g��ݒ�

VOID	SetupLights(LPDIRECT3DDEVICE9,D3DXVECTOR3,D3DLIGHT9,int);					// ���C�g�̃Z�b�g�A�b�v
VOID	SetupWorldMatrix( LPDIRECT3DDEVICE9 ,D3DXVECTOR3* , D3DXVECTOR3* ,D3DXVECTOR3* );//�`��s��̐ݒ�

VOID	drawMesh( LPDIRECT3DDEVICE9 , theObject * ,ModelClass*);
VOID	drawFVF(LPDIRECT3DDEVICE9,theBillbord*,LPDIRECT3DTEXTURE9,LPDIRECT3DVERTEXBUFFER9);

HRESULT	SetFVFTexturePosition( LPDIRECT3DVERTEXBUFFER9 , DWORD , DWORD );
VOID	SetCharTextureRECT(DWORD,theTexPosRECT*);	// �����L�����N�^�̃e�N�X�`�����W�ݒ�
VOID	SetCellTextureRECT(DWORD,theTexPosRECT*);	// ���ʗp�Z���̃e�N�X�`�����W�ݒ�
VOID	SetupFVFMatrix(LPDIRECT3DDEVICE9,theBillbord*);

//DirectInput�֘A
HRESULT	InitDirectInput(  HWND , HINSTANCE );			//DirectInput�̏�����
VOID	CleanUpDirectInput( void );						//DirectInput�̌�n��

HRESULT	SetupKeybord( LPDIRECTINPUT8 , HWND );			//�L�[�{�[�h�̏�����
HRESULT SetupMouse( LPDIRECTINPUT8 , HWND );			//�}�E�X�̏�����

int		GetKeybordState( LPDIRECTINPUTDEVICE8 );		//�L�[���͂̎擾
HRESULT getMouseState( HWND , LPDIRECTINPUTDEVICE8);	//�}�E�X���̎擾

//�\���̂Ƃ�
VOID	SetupSitus( theObject*, int );

VOID	ZeroObject( theObject * );
VOID	ZeroBillbord(theBillbord*);

//�\���̂��߂̍��W�n�ϊ�
VOID	SetupObjMatrix(LPDIRECT3DDEVICE9 , theObject * );

//�A�v���P�[�V�����֘A
HRESULT MsgLoop( HWND hWnd );								//���b�Z�[�W���[�v
VOID	CleanUpApplication( WNDCLASSEX * );					//�A�v���P�[�V�����̌�n��
LRESULT WINAPI MsgProc( HWND , UINT , WPARAM , LPARAM );	//���b�Z�[�W�n���h��

// �Q�[���̃��C������
HRESULT	GameBody( HWND );

//�ړ��p�֐�
D3DXVECTOR3	MoveForward(D3DXVECTOR3, float, float);
D3DXVECTOR3	MoveTo(D3DXVECTOR3,D3DXVECTOR3,float,int*);

//�\�[�g�p�֐�
int sort_ranking(const void* a, const void* b);

HRESULT	LoadTextureFromImageFiles( LPCTSTR , LPDIRECT3DTEXTURE9* , LPCTSTR  , LPDIRECT3DDEVICE9 );	//�e�N�X�`���̃��[�h

#endif