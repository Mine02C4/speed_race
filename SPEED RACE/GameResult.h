#ifndef ___GameResult
#define ___GameResult

#include "speedrace_head1.h"
#include "ModelClass.h"
#include "MenuBack.h"
#include "PartMenu.h"

/*--ÉQÅ[ÉÄÇÃåãâ --*/
class GameResult
{
	theObject				NumberRoll[5];
	theObject				KoronBord;
	ModelClass*				RollNum;
	ModelClass*				KoronMesh;
	ModelClass*				ModelNameIn;
	ModelClass*				ModelForm;
	ModelClass*				BackModel;
	MenuBack*				BackMenu;
	PartMenu*				NameIn;
	PartMenu*				FormBord;
	std::string*			PlayerName;
	theBillbord				NameBord[kNUM_MAX_PLAYERNAME];
	theBillbord				RaceTime[6];
	theBillbord				RankBord[10];
	LPDIRECT3DVERTEXBUFFER9	BillbordFVFBuffer;
	LPDIRECT3DTEXTURE9		BillbordTexture;
	int						StringPointer;
	int						LastKeyState[256];
	int						MoveFlag;
	int						ResultInfo;
	int						theTime;
	int						NumTime[5];
	int						TimeCounter;
	int						SpinFlag[5];
	int						PlayerRank;
	int						NumList;
	float					FloatTime;
	void					SpinRoll(void);
	void					ChkKey(void);
	void					SetName(void);
	void					SetupResultBillbords(LPDIRECT3DDEVICE9,int*);
	void					UpdateResultInfo(void);
public:
	GameResult(int*);
	~GameResult(void);
	void	DoResult(void);
	void	ResultRender(LPDIRECT3DDEVICE9);
};

#endif