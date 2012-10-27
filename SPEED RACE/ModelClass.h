#ifndef ___ModelClass
#define ___ModelClass

#include "speedrace_head1.h"
#include "CarClass.h"
#include "RaceClass.h"
/*--モデル格納用クラス--*/
class	ModelClass
{
	LPD3DXMESH				objMesh;			// システム中のメッシュオブジェクト
	D3DMATERIAL9*			objMeshMaterials;	// メッシュの質感(materials)
	LPDIRECT3DTEXTURE9*		objMeshTextures;	// メッシュのテクスチャ(texture)
	DWORD					objNumOfMaterials;	// メッシュ質感の数
	HRESULT		LoadFromXFileModel(LPCTSTR,LPDIRECT3DDEVICE9);
	HRESULT		LoadTexture( LPTSTR, LPDIRECT3DTEXTURE9*,LPCTSTR, LPDIRECT3DDEVICE9);
	void		InitModel(void);
public:
	ModelClass(const TCHAR*,int*);
	~ModelClass(void);
	LPD3DXMESH			ReturnMesh(void);
	DWORD				ReturnNumOfMaterials(void);
	D3DMATERIAL9*		ReturnMeshMaterials(void);
	LPDIRECT3DTEXTURE9*	ReturnMeshTextures(void);
};

#include "Global.h"
#endif