#ifndef ___PartMenu
#define ___PartMenu

#include "speedrace_head1.h"
/*--���j���[���ڂ̃N���X--*/
class PartMenu
{
	D3DXVECTOR3	Pos;			//�r���{�[�h�̈ʒu
	D3DXVECTOR3	OutPos;
	D3DXVECTOR3	InPos;
	D3DXVECTOR3	HeadPos;
	D3DXVECTOR3	ToPos;
	float		Yaw;			//���[
	float		Roll;			//���[��
	float		Pitch;			//�s�b�`
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