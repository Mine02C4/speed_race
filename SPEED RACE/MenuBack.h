#ifndef ___MenuBack
#define ___MenuBack

#include "speedrace_head1.h"
/*--���j���[�̔w�i�Ǘ��N���X--*/
class MenuBack
{
	theObject	BackBord[kNUM_BACK_MENU];
	int			BackTimeCounter;
	int			MovingNumber1;
	int			MovingNumber2;
	int			MoveFlag;
public:
	MenuBack(void);
	void		MoveBord(void);
	theObject	ReturnBord(int);
};

#endif