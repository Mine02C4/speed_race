#ifndef ___RaceClass
#define ___RaceClass

#include "speedrace_head1.h"
/*--レースのクラス--*/
class RaceClass
{
	CarClass	*MyCar;
	MapClass	*RaceMap;
	ModelClass*	Counter[4];
	ModelClass*	RaceFinish;
	int			AfterCount;
	int			RaceType;
	int			CourseType;
	int			RaceInfo;
	int			CountDown;
	int			HyouziCount;
	int			finishCounter;
	void		MoveRaceObjects(void);
	void		MoveRaceCamera(void);
	void		AppRaceKeyState(void);
	void		AppObjectsMove(void);
	void		ChkRaceObjects(void);
	void		ChkEnd(void);
	void		RaceCount(void);
	void		FinishCount(void);
	BOOL		ChkHitWall(D3DXVECTOR3*,D3DXVECTOR3*,FLOAT*,DWORD*);
	DWORD		StartTime;
	theObject	CountBord;
public:
	RaceClass(int,int,int*);
	~RaceClass(void);
	theObject	ReturnMyCarBody(void);
	theObject	ReturnMap(void);
	theObject	ReturnWall(void);
	void		DoRace(void);
	void		RaceRender(LPDIRECT3DDEVICE9);
};
#endif