#ifndef ___PartMenu
#define ___PartMenu

#include "speedrace_head1.h"
/*--メニュー項目のクラス--*/
class PartMenu
{
	D3DXVECTOR3	Pos;			//ビルボードの位置
	D3DXVECTOR3	OutPos;
	D3DXVECTOR3	InPos;
	D3DXVECTOR3	HeadPos;
	D3DXVECTOR3	ToPos;
	float		Yaw;			//ヨー
	float		Roll;			//ロール
	float		Pitch;			//ピッチ
	int			PartType;
	theObject	Bord;
	int			ModeBord;
	int			MoveFlag;
public:
	PartMenu(int);
	void		AppMode(int,D3DXVECTOR3);
	void		MovePart(void);
	theObject	ReturnObject(void);
	int			ChkMove(void);
	int			ChkOut(void);
	int			ReturnType(void);
	D3DXVECTOR3	ReturnPos(void);
};

#endif