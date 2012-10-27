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
EXTERN LPDIRECT3D9             g_pD3D			VAL(NULL);			//D3D�I�u�W�F�N�g
EXTERN LPDIRECT3DDEVICE9       g_pD3DDevice		VAL(NULL);			//D3D�f�o�C�X

EXTERN D3DPRESENT_PARAMETERS	g_D3DPParams;						//�v���[���e�[�V�����p�����[�^�̊i�[

EXTERN BOOL					g_FullScreenFlag	VAL(TRUE);			//�t���X�N���[���t���O�B�������TRUE�Ńt���X�N���[���AFALSE�ŃE�B���h�E���[�h
/*
�����Ӂ@���̒l��TRUE�ɂ���ƃt���X�N���[�����[�h�Ƃ��ăr���g����A�o�O�����������ꍇ�ɉ�ʂ���̂���Ă��Ē�~�ł��Ȃ��Ȃ�ꍇ������܂��B
�o�O�����S�ɏC�����Ă��炱�̒l��TRUE�ɂ��Ă��������B
*/

//--DirectInput
EXTERN LPDIRECTINPUT8			g_pDirectInput	VAL(NULL);		// DirectInput�I�u�W�F�N�g

EXTERN BYTE					g_pKeyState[256];					// DirectInput�̃L�[�{�[�h�X�e�[�^�X�o�b�t�@

EXTERN LPDIRECTINPUTDEVICE8	g_pKeyboard		VAL(NULL);				// �L�[�{�[�h�f�B�o�C�X
EXTERN LPDIRECTINPUTDEVICE8	g_pJoystick		VAL(NULL);				// �W���C�X�e�B�b�N�f�B�o�C�X
EXTERN LPDIRECTINPUTDEVICE8	g_pMouse		VAL(NULL);				// �}�E�X�f�B�o�C�X

//------------------------------------------------------------

EXTERN ModelClass	*g_ObjModels[kNUM_OF_MODELS];		//���f���̎�ނ���

//------------------------------------------------------------
EXTERN int		g_GameMode		VAL(kMODE_INIT);						//�Q�[�����[�h�̏�����
EXTERN int		g_GameCounter	VAL(0);								//�Q�[���p�J�E���^�[�̏�����


//���_�i�����X�i�[�j�̈ʒu���Ȃ�
EXTERN D3DXVECTOR3	gEyePt		VAL((D3DXVECTOR3(0.0f,100.0f,-100.0f)));	//�J�����ʒu
EXTERN D3DXVECTOR3	gLookatPt	VAL((D3DXVECTOR3(0.0f,0.0f,0.0f)));		//�����_

EXTERN D3DXVECTOR3	gUpVec		VAL((D3DXVECTOR3(0.0f,1.0f,0.0f)));		//�J�����̏������

//���j���[�p�̃J�����̏��
EXTERN D3DXVECTOR3	gBillUp			VAL((D3DXVECTOR3(0.0f,1.0f,0.0f)));
EXTERN D3DXVECTOR3	gBillEyePt		VAL((D3DXVECTOR3(0.0f,0.0f,1500.0f)));
EXTERN D3DXVECTOR3	gBillLookatPt	VAL((D3DXVECTOR3(0.0f,0.0f,0.0f)));

//���C�g�̐ݒ�
EXTERN D3DXVECTOR3	gLightVec		VAL((D3DXVECTOR3(0.0f,0.0f,-1.0f)));
EXTERN D3DLIGHT9	gLightLight;

//���[�X�̐錾
EXTERN RaceClass	*theRace;

//fps�����p�N���X
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

//�t�H���_�ݒ�Ȃ�
EXTERN	TCHAR*	kModelsPath			VAL(TEXT(".\\models\\"));		//models�t�H���_
EXTERN	int		kModelsPathLength	VAL(lstrlen( kModelsPath ));

EXTERN	TCHAR*	kImagesPath			VAL(TEXT(".\\images\\"));		//images�t�H���_
EXTERN	int		kImagesPathLength	VAL(lstrlen( kImagesPath ));

EXTERN	TCHAR*	kCourseIndex[kCTYPE_NUM][3];

EXTERN	int		gCourseType;

EXTERN	DWORD	gGameResultTime;

#endif