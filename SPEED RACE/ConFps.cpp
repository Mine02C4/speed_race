#include "ConFps.h"

/*--ConFps--*/
//�R���X�g���N�^
ConFps::ConFps(void)
{
	Fps = 0;
	TimeCount=0;
	Wait = 1000.0 / 60.0;
	OldTime = timeGetTime();
}
//������Ăяo���Ԋu��1/60�b�ɂȂ�悤�ɂ���
void ConFps::Update()
{
	//�����Ԃ̎擾
	DWORD now_time = timeGetTime();
	//---------------------------------------
	//	���Ԓ���
	//---------------------------------------
	while( now_time < Wait )
	{
		Sleep(0);
	}


//	Times[TimeCount] = GetTime()-(OldTime+(int)((WAIT*TimeCount)+0.5));
/*
	//---------------------------------------
	//	FPS�̍X�V
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