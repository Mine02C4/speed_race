#ifndef ___MenuClass
#define ___MenuClass

#include "speedrace_head1.h"
/*--メニューのクラス--*/
class MenuClass
{
	PartMenu*				MenuPart[kNUM_OF_MENU];
	ModelClass*				MenuModels[kNUM_OF_MENU];
	ModelClass*				BackModel;
	ModelClass*				MiniCourse[kCTYPE_NUM];
	theObject				RankingCourse;
	ModelClass*				ModelRankingCourseTouge;
	ModelClass*				ModelRankingCourseGake;
	MenuBack*				BackMenu;
	CourseInfo*				SetCourse;
	theBillbord				RankingBord[5][kNUM_MAX_RANKING];
	char					RankingChar[5][kNUM_MAX_RANKING+1];
	int						MenuInfo;							//メニューの状態
	int						MoveFlag;							//どれか一つでも動いていたらTRUE
	int						PointerMove;
	int						PtPointer;
	int						HeadPart;
	int						ChangeBack;
	int						SettingCourse;
	int						NumList;
	int						RankingCounter;
	int						CoureseRanking;
	int						RankingKeyCounter;
	LPDIRECT3DVERTEXBUFFER9	BillbordFVFBuffer;
	LPDIRECT3DTEXTURE9		BillbordTexture;
	void					AppPoint(void);
	void					MovePointer(void);
	void					ChkMove(void);
	void					UpPointer(void);
	void					DownPointer(void);
	void					AppMove(void);
	void					ChkAct(void);
	void					RankingSet(void);
	void					SetupResultBillbords(LPDIRECT3DDEVICE9,int*);
public:
	MenuClass(int*);
	~MenuClass(void);
	void		DoMenu(void);
	void		MenuRender(LPDIRECT3DDEVICE9);
};

#endif