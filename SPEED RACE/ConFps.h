#ifndef ___ConFps
#define ___ConFps

#include "speedrace_head1.h"
/*--FPS‚Ì’²®EŠÇ—ƒNƒ‰ƒX--*/
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