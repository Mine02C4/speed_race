#ifndef ___OpeningClass
#define ___OpeningClass

#include "speedrace_head1.h"
/*--オープニング--*/
class OpeningClass
{
	theObject	SpeedRaceLogoBord;
	theObject	NPCALogoBord;
	ModelClass*	ModelSpeedRace;
	ModelClass*	ModelNPCALogo;
	int			OpeningCounter;
	int			MoveFlag;
public:
	OpeningClass(int*);
	~OpeningClass(void);
	void	OpeningRender(LPDIRECT3DDEVICE9);
	void	DoOpening(void);
};

#endif