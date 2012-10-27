#include "CreditClass.h"
#include "Global.h"

/*--CreditClass--*/
//コンストラクタ
CreditClass::CreditClass(int* errorFlag)
{
	ZeroObject(&CreditBord);
	SetupSitus(&CreditBord,NULL);
	ModelCredit = new ModelClass(kXCREDIT_IMAGE,errorFlag);
	CreditCounter = 120;
}
//デストラクタ
CreditClass::~CreditClass(void)
{
	delete ModelCredit;
}
//クレジットを実行
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
//描画
void CreditClass::CreditRender(LPDIRECT3DDEVICE9 theDevice)
{
	// Zバッファとバックバッファのクリア
	theDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(159,207,255), 1.0f, 0 );

	// シーンの開始
	if( SUCCEEDED( theDevice->BeginScene() ) )
	{

		
		SetupWorldMatrix(theDevice,&D3DXVECTOR3(0.0f,0.0f,1500.0f),&gBillLookatPt,&gBillUp);// カメラ視点設定

		SetupRanderStates(theDevice,kRS_ALPHA);

		drawMesh(theDevice,&CreditBord,ModelCredit);
	
		//終わりの定型処理
		// シーンの終了
		theDevice->EndScene();
	}

	// バックバッファを表画面に反映させる。
	HRESULT hr = theDevice->Present( NULL, NULL, NULL, NULL );
}