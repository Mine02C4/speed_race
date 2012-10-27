#ifndef ___CarClass
#define ___CarClass

#include "speedrace_head1.h"
#include "ModelClass.h"
/*--車のクラス--*/
class	CarClass
{
	float		Speed;			//スピード
	float		Different;		//ドリフト時の進行方向と車のずれ
	int			SpinFlag;		//スピンの状態
	D3DXVECTOR3	Pos;			//オブジェクトの座標
	D3DXVECTOR3 BeforePos;		//前の座標を記憶
	float		Yaw;			//ヨー
	float		Roll;			//ロール
	float		Pitch;			//ピッチ
	int			CarType;		//車の種類
	int			fAccel;			//アクセルの状態 Accelメゾットで変更
	int			fBrake;			//ブレーキの状態 Brakeメゾットで変更
	int			fHandle;		//ハンドルの状態 Handleメゾットで変更
public:
	theObject	Body;	//ボディーの構造体(いずれタイヤなどもできる)
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