#include "CarClass.h"

/*--CarClass--*/
//�R���X�g���N�^
CarClass::CarClass(const TCHAR* CarName,int* errorFlag)
: Speed(0.0f),Different(0.0f),SpinFlag(kHAN_NORMAL),Pos(D3DXVECTOR3(0.0f,10000.0f,0.0f)),
BeforePos(D3DXVECTOR3(0.0f,0.0f,0.0f)),Yaw(0.0f),Roll(0.0f),Pitch(0.0f),CarType(NULL)
{
	ZeroObject( &Body );
	SetupSitus( &Body, kMDL_M_CAR );
	g_ObjModels[Body.objType] = new ModelClass(CarName,errorFlag);
}
//�f�X�g���N�^
CarClass::~CarClass(void)
{
	delete g_ObjModels[Body.objType];
}
//�A�N�Z��
void CarClass::Accel(int result)
{
	if(result == TRUE)
	{
		fAccel = kCON_ON;
	}
	else
	{
		fAccel = kCON_OFF;
	}
	
}
//�u���[�L
void CarClass::Brake(int result)
{
	if(result == TRUE)
	{
		fBrake = kCON_ON;
	}
	else
	{
		fBrake = kCON_OFF;
	}
}
//�n���h��
void CarClass::Handle(int LeftResult, int RightResult)
{
	if( LeftResult ^ RightResult )
	{
		if( LeftResult )
		{
			fHandle = kHAN_LEFT;
		}
		if( RightResult )
		{
			fHandle = kHAN_RIGHT;
		}
	}
	else
	{
		fHandle = kHAN_NORMAL;
	}
}
//�X�s�[�h���擾
float CarClass::ReturnSpeed(void)
{
	return Speed;
}
//�Ԃ��~�߂�
void CarClass::Stop(void)
{
	Speed = 0.0f;
	Different = 0.0f;
	fAccel = kCON_OFF;
	fBrake = kCON_OFF;
	fHandle = kHAN_NORMAL;
}
//������K������(fps��60)
void CarClass::AppMove(void)
{
	float	ConFPS = 0.05f;
	BeforePos = Pos;
	float	MoveYawBuf	= 0.0f;
	if( SpinFlag != kHAN_NORMAL )
	{
		Speed -= 10.0f * ConFPS;
		Different = 0.0f;
		if(( Different > kPI_HALF)||( SpinFlag == kHAN_RIGHT ))
		{
			MoveYawBuf += 3.0f * kDEG_TO_RAD;
			SpinFlag = kHAN_RIGHT;
		}
		if(( Different < -kPI_HALF )||( SpinFlag == kHAN_LEFT ))
		{
			MoveYawBuf -= 3.0f * kDEG_TO_RAD;
			SpinFlag = kHAN_LEFT;
		}
		if( Speed <= 0.0f ){ SpinFlag = kHAN_NORMAL; }
	}
	else
	{
		if( Speed > 0.0f )
		{
			if( fHandle == kHAN_RIGHT )
			{
				MoveYawBuf += (2.0f - Speed / 500.0f) * kDEG_TO_RAD;
				if( fBrake == kCON_ON )
				{ //�E�h���t�g�J�n����
					Different -= 1.0f * kDEG_TO_RAD;
					MoveYawBuf += 1.0f * kDEG_TO_RAD;
				}
			}
			if( fHandle == kHAN_LEFT )
			{
				MoveYawBuf -= (2.0f - Speed / 500.0f) * kDEG_TO_RAD;
				if( fBrake == kCON_ON )
				{ //���h���t�g�J�n����
					Different += 1.0f * kDEG_TO_RAD;
					MoveYawBuf -= 1.0f * kDEG_TO_RAD;
				}
			}
		}
		if( fAccel == kCON_ON )
		{
			Speed += (3.0f - Speed / 300.0f) * ConFPS;
		}//�A�N�Z���I���̎�
		if( fBrake == kCON_ON )
		{
			Speed -= (4.0f - Speed / 150.0f) * ConFPS;
		}//�u���[�L�I���̎�
		if( Speed <= 0.0 ){ Different = 0.0; }//��~�����Ƃ��Ƀh���t�g���~�߂�
		if( Different < 0.0 )
		{//�E�h���t�g
			if( fAccel == kCON_ON )
			{//�E�h���t�g�ƃA�N�Z��
				Different -= 0.5f * kDEG_TO_RAD;
				MoveYawBuf += 2.0f * kDEG_TO_RAD;
			}
			if( fHandle == kHAN_RIGHT )
			{
				Different -= 0.5f * kDEG_TO_RAD;
				MoveYawBuf += 0.5f * kDEG_TO_RAD;
			}
			if( fHandle == kHAN_LEFT )
			{
				Different += 0.5f * kDEG_TO_RAD;
				MoveYawBuf -= 0.5f * kDEG_TO_RAD;
			}
			if( Different > 0.0f ){ Different = 0.0f; }
			MoveYawBuf += (Different / -40.0f) * kDEG_TO_RAD;
			Different += 1.0f * kDEG_TO_RAD;
		}
		if( Different > 0.0f )
		{//���h���t�g
			if( fAccel == kCON_ON )
			{//���h���t�g�ƃA�N�Z��
				Different += 0.5f * kDEG_TO_RAD;
				MoveYawBuf -= 2.0f * kDEG_TO_RAD;
			}
			if( fHandle == kHAN_LEFT )
			{
				Different += 0.5f * kDEG_TO_RAD;
				MoveYawBuf -= 0.5f * kDEG_TO_RAD;
			}
			if( fHandle == kHAN_RIGHT )
			{
				Different -= 0.5f * kDEG_TO_RAD;
				MoveYawBuf += 0.5f * kDEG_TO_RAD;
			}
			if( Different < 0.0 ){ Different = 0.0; }
			MoveYawBuf += (Different / -40.0f) * kDEG_TO_RAD;
			Different -= 1.0f * kDEG_TO_RAD;
		}
	}
	Yaw += MoveYawBuf;
	Speed -= 1.0f * ConFPS;
	if( Speed < 0.0 ){ Speed = 0.0; }
	Yaw += Different;
	Pos = MoveForward(Pos, Speed, Yaw);
	Yaw -= Different;
}
//�ʒu��Ԃ�
D3DXVECTOR3	CarClass::ReturnPos(void)
{
	return Pos;
}
//����(XZ���ʏ��)��Ԃ�
float	CarClass::ReturnYaw(void)
{
	return Yaw;
}
//�Ԗ{�̂�Ԃ�
theObject CarClass::ReturnBody(void)
{
	return Body;
}
//�ʒu���Z�b�g����
void CarClass::SetPos(D3DXVECTOR3 PosBuf)
{
	Pos = PosBuf;
}
//�O�̍��W��Ԃ�
D3DXVECTOR3 CarClass::ReturnBeforePos(void)
{
	return BeforePos;
}
//������K������
void CarClass::AppObjMove(void)
{
	Body.objPitch = Pitch;
	Body.objRoll = Roll;
	Body.objYaw = Yaw;
	Body.objPos = Pos;
}
//��������
void CarClass::SlowDown(float DownSpeed)
{
	Speed -= DownSpeed;
}
//�X�s�[�h���Z�b�g����
void CarClass::SetSpeed(float BufSpeed)
{
	Speed = BufSpeed;
}