#include "ConFps.h"

/*--ConFps--*/
//コンストラクタ
ConFps::ConFps(void)
{
	Fps = 0;
	TimeCount=0;
	Wait = 1000.0 / 60.0;
	OldTime = timeGetTime();
}
//これを呼び出す間隔が1/60秒になるようにする
void ConFps::Update()
{
	//現時間の取得
	DWORD now_time = timeGetTime();
	//---------------------------------------
	//	時間調整
	//---------------------------------------
	while( now_time < Wait )
	{
		Sleep(0);
	}


//	Times[TimeCount] = GetTime()-(OldTime+(int)((WAIT*TimeCount)+0.5));
/*
	//---------------------------------------
	//	FPSの更新
	//---------------------------------------
	if( TimeCount == COUNT-1 )
	{
		int sum = 0;
		for(int i=0;i<COUNT;i++) sum += Times[i];

		double average = double(sum)/double(COUNT);
		if( average != 0 ) Fps = 1000.0/average; else Fps = 0;

		OldTime = GetTime();
	}

	TimeCount = (TimeCount+1)%COUNT;
*/
//	return time_out;
}