#ifndef ___CourseInfo
#define ___CourseInfo

#include "speedrace_head1.h"
/*--メニュー情報のクラス--*/
class CourseInfo
{
	D3DXVECTOR3	Pos;
	float		Yaw;			//ヨー
	float		Roll;			//ロール
	float		Pitch;			//ピッチ
	theObject	Map;
public:
	CourseInfo(void);
	void		MoveMap(void);
	theObject	ReturnObject(void);
};

#endif