#include "PartMenu.h"

/*--PartMenu--*/
//�R���X�g���N�^
PartMenu::PartMenu(int aType)
{
	Yaw = 0.0f;
	Pitch = 0.0f;
	Roll = 0.0f;
	MoveFlag = FALSE;
	PartType = aType;
	ModeBord = kPART_INFO_OUT;
	OutPos	= D3DXVECTOR3(1500.0f,0.0f,0.0f);
	HeadPos	= D3DXVECTOR3(0.0f,512.0f,0.0f);
	Pos = OutPos;
	switch(PartType)
	{
	case kMENU_KYOUSOUKAISI:
		InPos	= D3DXVECTOR3(0.0f,288.0f,0.0f);
		break;
	case kMENU_SOUSAHOUHOU:
		InPos	= D3DXVECTOR3(0.0f,96.0f,0.0f);
		break;
	case kMENU_RANKING:
		InPos	= D3DXVECTOR3(0.0f,-96.0f,0.0f);
		break;
	case kMENU_SYUURYOU:
		InPos	= D3DXVECTOR3(0.0f,-288.0f,0.0f);
		break;
	case kMENU_MENU:
		InPos	= HeadPos;
		break;
	case kMENU_MODORU:
		InPos	= D3DXVECTOR3(0.0f,-352.0f,0.0f);
		break;
	case kMENU_TOUGE:
		InPos	= D3DXVECTOR3(500.0f,96.0f,0.0f);
		break;
	case kMENU_GAKE:
		InPos	= D3DXVECTOR3(500.0f,-96.0f,0.0f);
		break;
	case kINFO_HOWTOPLAY:
		InPos	= D3DXVECTOR3(0.0f,96.0f,0.0f);
		break;
	case kMENU_POINTER:
		InPos	= OutPos;
		break;
	case kINFO_NAME_IN:
		InPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
		break;
	case kINFO_RANKING:
		InPos	= D3DXVECTOR3(0.0f,96.0f,0.0f);
		break;
	case kINFO_FORM:
		InPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
		break;
	}
	ZeroObject(&Bord);
	SetupSitus(&Bord,PartType);
}
//���[�h��ݒ�
void PartMenu::AppMode(int aMode,D3DXVECTOR3 TargetPos)
{
	if(aMode == kPART_INFO_CUSTOMPOS)
	{
		if(TargetPos != ToPos)
		{
			ModeBord = aMode;
			ToPos = TargetPos;
			MoveFlag = TRUE;
		}
	}
	else if(ModeBord != aMode)
	{
		ModeBord = aMode;
		MoveFlag = TRUE;
		switch(aMode)
		{
		case kPART_INFO_OUT:
			ToPos = OutPos;
			break;
		case kPART_INFO_IN:
			ToPos = InPos;
			break;
		case kPART_INFO_HEAD:
			ToPos = HeadPos;
			break;
		}
	}
}
//�r���{�[�h�ړ�
void PartMenu::MovePart(void)
{
	if(MoveFlag == TRUE)
	{
		int EndFlag;
		Pos = MoveTo(Pos,ToPos,40.0f,&EndFlag);
		Roll += 20.0f * kDEG_TO_RAD;
		if(EndFlag == TRUE)
		{
			MoveFlag = FALSE;
			Roll = 0.0f;
		}
	}
	Bord.objPitch	= Pitch;
	Bord.objRoll	= Roll;
	Bord.objYaw		= Yaw;
	Bord.objPos		= Pos;
}
//Object��Ԃ�
theObject PartMenu::ReturnObject(void)
{
	return Bord;
}
//�����Ă邩�ǂ����`�F�b�N
int PartMenu::ChkMove(void)
{
	return MoveFlag;
}
//�^�C�v��Ԃ�
int PartMenu::ReturnType(void)
{
	return PartType;
}
//�ꏊ��Ԃ�
D3DXVECTOR3	PartMenu::ReturnPos(void)
{
	return Pos;
}
//���E�̊O�����`�F�b�N
int PartMenu::ChkOut(void)
{
	if((MoveFlag==FALSE)&&(ModeBord==kPART_INFO_OUT))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}