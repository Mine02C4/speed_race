#ifndef ___ConFps
#define ___ConFps

#include "speedrace_head1.h"
/*--FPSの調整・管理クラス--*/
class ConFps
{
private:
	const static int COUNT=60;
	int Times[COUNT];
	int TimeCount;
	DWORD OldTime;
	double Fps;
	double Wait;
public:
	ConFps(void);
	void Update(void);
};
#endif