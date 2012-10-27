#include "ModelClass.h"

/*--ModelClass--*/
//コンストラクタ
ModelClass::ModelClass(const TCHAR* ModelName,int* errorFlag)
{
	InitModel();
	if(FAILED(LoadFromXFileModel(ModelName,g_pD3DDevice)))
	{
		*errorFlag = TRUE;
	}
}
//デストラクタ
ModelClass::~ModelClass(void)
{
	/*メッシュマテリアルの解放*/
	SAFE_DELETE_ARRAY(objMeshMaterials);

	/*テクスチャの解放*/
	if( objMeshTextures )
	{
		for(DWORD j0 = 0; j0 < objNumOfMaterials ; j0++ ){ SAFE_RELEASE( objMeshTextures[j0] );}
		SAFE_DELETE_ARRAY( objMeshTextures );
	}

	/*メッシュの開放*/
	SAFE_RELEASE( objMesh );
}
//Xファイルからのモデルデータの読み込み
HRESULT ModelClass::LoadFromXFileModel(LPCTSTR FileName, LPDIRECT3DDEVICE9 theDevice)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;	/*一時バッファ用*/

	// Xファイルの読み込み。
	if(FAILED(D3DXLoadMeshFromX(FileName, D3DXMESH_SYSTEMMEM,theDevice, NULL,&pD3DXMtrlBuffer,NULL,&objNumOfMaterials,&objMesh)))
	{
		TCHAR cautionString[kSTR_LENGTH];
		wsprintf(cautionString,TEXT("Xファイル『%s』が見つかりませんでした。"),FileName);
		MessageBox(NULL,cautionString,kWINDOW_TITLE,MB_OK);
		return(E_FAIL);
	}

	//=========================================================================
	// pD3DXMtrlBufferから、マテリアルやテクスチャの名称などの情報を取得する。
	D3DXMATERIAL* d3dxMaterials	= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	objMeshMaterials= new D3DMATERIAL9[objNumOfMaterials];
	objMeshTextures	= new LPDIRECT3DTEXTURE9[objNumOfMaterials];

	for(DWORD i0=0; i0 < objNumOfMaterials ; i0++ )
	{
		// 質感（material）のコピー
		objMeshMaterials[i0] = d3dxMaterials[i0].MatD3D;

		// 質感（material）に環境光を設定する（D3DXが何もしないため）
		objMeshMaterials[i0].Ambient = objMeshMaterials[i0].Diffuse;

		objMeshTextures[i0] = NULL;
		if( d3dxMaterials[i0].pTextureFilename != NULL && lstrlen(d3dxMaterials[i0].pTextureFilename) > 0 )
		{
			if(FAILED( LoadTexture(d3dxMaterials[i0].pTextureFilename,&objMeshTextures[i0],kModelsPath,theDevice)))
			{
				TCHAR cautionString[kSTR_LENGTH];

				wsprintf(cautionString,TEXT("Xファイル『%s』で指定されたテクスチャマップファイル『%s』が見つかりませんでした。"),FileName,d3dxMaterials[i0].pTextureFilename);
				MessageBox(NULL,cautionString,kWINDOW_TITLE,MB_OK);
				return( E_FAIL );
			}
		}
	}

	// 質感（material）バッファの開放。
	pD3DXMtrlBuffer->Release();

	return(S_OK);
}
//画像ファイルをテクスチャとして読み込む
HRESULT ModelClass::LoadTexture(LPTSTR FileName, LPDIRECT3DTEXTURE9* aTexture, LPCTSTR aSearchPath, LPDIRECT3DDEVICE9 theDevice)
{
	// テクスチャの生成
	// これもまずカレントフォルダを探し、見つからない場合は「models」フォルダ内を探す
	if(FAILED(D3DXCreateTextureFromFile(theDevice,FileName,aTexture)))
	{
		return(E_FAIL);
	}

	return( S_OK );
}
//モデルの初期化
void ModelClass::InitModel(void)
{
	objMesh				= NULL;
	objMeshMaterials	= NULL;
	objMeshTextures		= NULL;
	objNumOfMaterials	= 0;
}
//メッシュを返す
LPD3DXMESH ModelClass::ReturnMesh(void)
{
	return objMesh;
}
//マテリアルの数を返す
DWORD ModelClass::ReturnNumOfMaterials(void)
{
	return objNumOfMaterials;
}
//マテリアルを返す
D3DMATERIAL9* ModelClass::ReturnMeshMaterials(void)
{
	return objMeshMaterials;
}
//テクスチャを返す
LPDIRECT3DTEXTURE9* ModelClass::ReturnMeshTextures(void)
{
	return objMeshTextures;
}