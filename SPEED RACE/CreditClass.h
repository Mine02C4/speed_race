#ifndef ___CreditClass
#define ___CreditClass

#include "speedrace_head1.h"
#include "ModelClass.h"
/*--クレジット--*/
class CreditClass
{
	theObject	CreditBord;
	ModelClass*	ModelCredit;
	int CreditCounter;
public:
	CreditClass(int*);
	~CreditClass(void);
	void DoCredit(void);
	void CreditRender(LPDIRECT3DDEVICE9);
};

#endif