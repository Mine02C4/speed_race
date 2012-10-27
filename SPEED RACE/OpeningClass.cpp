#include "OpeningClass.h"
#include "Global.h"

/*--OpeningClass--*/
//�R���X�g���N�^
OpeningClass::OpeningClass(int* errorFlag)
{
	ZeroObject(&SpeedRaceLogoBord);
	SetupSitus(&SpeedRaceLogoBord,NULL);
	ZeroObject(&NPCALogoBord);
	SetupSitus(&NPCALogoBord,NULL);
	NPCALogoBord.objPos = D3DXVECTOR3(50.0f,0.0f,1500.0f);
	ModelSpeedRace = new ModelClass(kXOPENING_SPEEDLOGO,errorFlag);
	ModelNPCALogo = new ModelClass(kXOPENING_NPCALOGO,errorFlag);
	OpeningCounter = 0;
	MoveFlag = TRUE;
}
//�f�X�g���N�^
OpeningClass::~OpeningClass(void)
{
	delete ModelNPCALogo;
	delete ModelSpeedRace;
}
//�I�[�v�j���O��i�߂�
void OpeningClass::DoOpening(void)
{
	if(MoveFlag==TRUE)
	{
		OpeningCounter++;
	}
	if((OpeningCounter>30)&&(MoveFlag==TRUE))
	{
		NPCALogoBord.objPos.z -= 10.0f;
		if(NPCALogoBord.objPos.z<0.0f)
		{
			NPCALogoBord.objPos.z = 0.0f;
			MoveFlag = FALSE;
		}
	}
	if(MoveFlag==FALSE)
	{
		if(g_pKeyState[DIK_RETURN] & 0x80)
		{
			g_GameMode = kMODE_INIT_MENU;
		}
	}
}
//�����_�����O
void OpeningClass::OpeningRender(LPDIRECT3DDEVICE9 theDevice)
{
	// Z�o�b�t�@�ƃo�b�N�o�b�t�@�̃N���A
	theDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(159,207,255), 1.0f, 0 );

	// �V�[���̊J�n
	if( SUCCEEDED( theDevice->BeginScene() ) )
	{

		
		SetupWorldMatrix(theDevice,&D3DXVECTOR3(0.0f,0.0f,800.0f),&gBillLookatPt,&gBillUp);// �J�������_�ݒ�

		SetupRanderStates(theDevice,kRS_ALPHA);

		drawMesh(theDevice,&SpeedRaceLogoBord,ModelSpeedRace);
		drawMesh(theDevice,&NPCALogoBord,ModelNPCALogo);
	
		//�I���̒�^����
		// �V�[���̏I��
		theDevice->EndScene();
	}

	// �o�b�N�o�b�t�@��\��ʂɔ��f������B
	HRESULT hr = theDevice->Present( NULL, NULL, NULL, NULL );
}