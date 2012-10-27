#ifndef ___MapClass
#define ___MapClass

#include "speedrace_head1.h"
#include "ModelClass.h"

/*--地面データのクラス--*/
class MapClass
{
	int MapType;
	theObject Map;
	theObject Wall;
	theObject End;
public:
	MapClass(int,int*);
	~MapClass(void);
	theObject ReturnMap(void);
	theObject ReturnWall(void);
	theObject ReturnEnd(void);
};

#include "Global.h"
#endif