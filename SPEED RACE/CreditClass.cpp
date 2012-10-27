#include "CreditClass.h"
#include "Global.h"

/*--CreditClass--*/
//�R���X�g���N�^
CreditClass::CreditClass(int* errorFlag)
{
	ZeroObject(&CreditBord);
	SetupSitus(&CreditBord,NULL);
	ModelCredit = new ModelClass(kXCREDIT_IMAGE,errorFlag);
	CreditCounter = 120;
}
//�f�X�g���N�^
CreditClass::~CreditClass(void)
{
	delete ModelCredit;
}
//�N���W�b�g�����s
void CreditClass::DoCredit(void)
{
	if(CreditCounter>0)
	{
		CreditCounter--;
	}
	if((g_pKeyState[DIK_RETURN] & 0x80)&&(CreditCounter==0))
	{
		PostQuitMessage(0);
	}
}
//�`��
void CreditClass::CreditRender(LPDIRECT3DDEVICE9 theDevice)
{
	// Z�o�b�t�@�ƃo�b�N�o�b�t�@�̃N���A
	theDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(159,207,255), 1.0f, 0 );

	// �V�[���̊J�n
	if( SUCCEEDED( theDevice->BeginScene() ) )
	{

		
		SetupWorldMatrix(theDevice,&D3DXVECTOR3(0.0f,0.0f,1500.0f),&gBillLookatPt,&gBillUp);// �J�������_�ݒ�

		SetupRanderStates(theDevice,kRS_ALPHA);

		drawMesh(theDevice,&CreditBord,ModelCredit);
	
		//�I���̒�^����
		// �V�[���̏I��
		theDevice->EndScene();
	}

	// �o�b�N�o�b�t�@��\��ʂɔ��f������B
	HRESULT hr = theDevice->Present( NULL, NULL, NULL, NULL );
}