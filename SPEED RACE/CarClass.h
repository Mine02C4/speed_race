#ifndef ___CarClass
#define ___CarClass

#include "speedrace_head1.h"
#include "ModelClass.h"
/*--�Ԃ̃N���X--*/
class	CarClass
{
	float		Speed;			//�X�s�[�h
	float		Different;		//�h���t�g���̐i�s�����ƎԂ̂���
	int			SpinFlag;		//�X�s���̏��
	D3DXVECTOR3	Pos;			//�I�u�W�F�N�g�̍��W
	D3DXVECTOR3 BeforePos;		//�O�̍��W���L��
	float		Yaw;			//���[
	float		Roll;			//���[��
	float		Pitch;			//�s�b�`
	int			CarType;		//�Ԃ̎��
	int			fAccel;			//�A�N�Z���̏�� Accel���]�b�g�ŕύX
	int			fBrake;			//�u���[�L�̏�� Brake���]�b�g�ŕύX
	int			fHandle;		//�n���h���̏�� Handle���]�b�g�ŕύX
public:
	theObject	Body;	//�{�f�B�[�̍\����(������^�C���Ȃǂ��ł���)
	CarClass(const TCHAR*,int*);
	~CarClass(void);
	void	Accel(int);
	void	Brake(int);
	void	Handle(int,int);
	void	Stop(void);
	float	ReturnSpeed(void);
//	void	ReturnBody( theObject * );
	void	AppMove(void);
	void	AppObjMove(void);
	D3DXVECTOR3	ReturnPos(void);
	float	ReturnYaw(void);
	theObject ReturnBody(void);
	void	SetPos(D3DXVECTOR3);
	D3DXVECTOR3 ReturnBeforePos(void);
	void	SlowDown(float);
	void	SetSpeed(float);
};
#include "Global.h"

#endif