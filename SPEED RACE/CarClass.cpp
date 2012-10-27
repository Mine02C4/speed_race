#include "CarClass.h"

/*--CarClass--*/
//コンストラクタ
CarClass::CarClass(const TCHAR* CarName,int* errorFlag)
: Speed(0.0f),Different(0.0f),SpinFlag(kHAN_NORMAL),Pos(D3DXVECTOR3(0.0f,10000.0f,0.0f)),
BeforePos(D3DXVECTOR3(0.0f,0.0f,0.0f)),Yaw(0.0f),Roll(0.0f),Pitch(0.0f),CarType(NULL)
{
	ZeroObject( &Body );
	SetupSitus( &Body, kMDL_M_CAR );
	g_ObjModels[Body.objType] = new ModelClass(CarName,errorFlag);
}
//デストラクタ
CarClass::~CarClass(void)
{
	delete g_ObjModels[Body.objType];
}
//アクセル
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
//ブレーキ
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
//ハンドル
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
//スピードを取得
float CarClass::ReturnSpeed(void)
{
	return Speed;
}
//車を止める
void CarClass::Stop(void)
{
	Speed = 0.0f;
	Different = 0.0f;
	fAccel = kCON_OFF;
	fBrake = kCON_OFF;
	fHandle = kHAN_NORMAL;
}
//動きを適応する(fpsは60)
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
				{ //右ドリフト開始動作
					Different -= 1.0f * kDEG_TO_RAD;
					MoveYawBuf += 1.0f * kDEG_TO_RAD;
				}
			}
			if( fHandle == kHAN_LEFT )
			{
				MoveYawBuf -= (2.0f - Speed / 500.0f) * kDEG_TO_RAD;
				if( fBrake == kCON_ON )
				{ //左ドリフト開始動作
					Different += 1.0f * kDEG_TO_RAD;
					MoveYawBuf -= 1.0f * kDEG_TO_RAD;
				}
			}
		}
		if( fAccel == kCON_ON )
		{
			Speed += (3.0f - Speed / 300.0f) * ConFPS;
		}//アクセルオンの時
		if( fBrake == kCON_ON )
		{
			Speed -= (4.0f - Speed / 150.0f) * ConFPS;
		}//ブレーキオンの時
		if( Speed <= 0.0 ){ Different = 0.0; }//停止したときにドリフトを止める
		if( Different < 0.0 )
		{//右ドリフト
			if( fAccel == kCON_ON )
			{//右ドリフトとアクセル
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
		{//左ドリフト
			if( fAccel == kCON_ON )
			{//左ドリフトとアクセル
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
//位置を返す
D3DXVECTOR3	CarClass::ReturnPos(void)
{
	return Pos;
}
//向き(XZ平面上の)を返す
float	CarClass::ReturnYaw(void)
{
	return Yaw;
}
//車本体を返す
theObject CarClass::ReturnBody(void)
{
	return Body;
}
//位置をセットする
void CarClass::SetPos(D3DXVECTOR3 PosBuf)
{
	Pos = PosBuf;
}
//前の座標を返す
D3DXVECTOR3 CarClass::ReturnBeforePos(void)
{
	return BeforePos;
}
//動きを適応する
void CarClass::AppObjMove(void)
{
	Body.objPitch = Pitch;
	Body.objRoll = Roll;
	Body.objYaw = Yaw;
	Body.objPos = Pos;
}
//減速する
void CarClass::SlowDown(float DownSpeed)
{
	Speed -= DownSpeed;
}
//スピードをセットする
void CarClass::SetSpeed(float BufSpeed)
{
	Speed = BufSpeed;
}