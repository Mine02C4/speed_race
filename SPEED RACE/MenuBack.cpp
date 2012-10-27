#include "MenuBack.h"
#include "Global.h"

/*--MenuBack--*/
//コンストラクタ
MenuBack::MenuBack(void)
{
	int i;
	for(i=0;i<kNUM_BACK_MENU;i++)
	{
		ZeroObject(&BackBord[i]);
		SetupSitus(&BackBord[i],NULL);
		BackBord[i].objPos = D3DXVECTOR3((i%10)*512.0f-2304.0f,int(i/10)*512.0f-2304.0f,-2000.0f);
	}
	MoveFlag = FALSE;
	BackTimeCounter = 0;
}
//Bordを返す
theObject MenuBack::ReturnBord(int BordNumber)
{
	return BackBord[BordNumber];
}
//Bordを動かす
void MenuBack::MoveBord(void)
{
	if((BackTimeCounter==0)&&(MoveFlag==FALSE))
	{
		MoveFlag = TRUE;
		MovingNumber1 = rand()%100;
		do
		{
			MovingNumber2 = rand()%100;
		}while(MovingNumber1==MovingNumber2);
	}
	if(MoveFlag==TRUE)
	{
		BackBord[MovingNumber1].objYaw+=12*kDEG_TO_RAD;
		BackBord[MovingNumber2].objYaw+=12*kDEG_TO_RAD;
		if((BackBord[MovingNumber1].objYaw >= kPI_ROUND)&&(BackBord[MovingNumber2].objYaw >= kPI_ROUND))
		{
			MoveFlag = FALSE;
			BackBord[MovingNumber1].objYaw = 0.0f;
			BackBord[MovingNumber2].objYaw = 0.0f;
		}
	}
	BackTimeCounter++;
	BackTimeCounter%=30;
}