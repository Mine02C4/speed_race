#include "RaceClass.h"
#include "CarClass.h"
#include "MapClass.h"
#include "Global.h"
#include "ModelClass.h"

/*--RceClass--*/
//コンストラクタ
RaceClass::RaceClass(int typeRace, int typeMap, int* errorFlag)
{
	MyCar = new CarClass(kXCAR_AE86,errorFlag);
	RaceMap = new MapClass(typeMap,errorFlag);
	RaceType = typeRace;
	RaceInfo = kRACE_INFO_COUNT;
	CountDown = 180;
	AfterCount = FALSE;

	finishCounter = 180;

	ZeroObject(&CountBord);
	SetupSitus(&CountBord,NULL);
	
	Counter[0] = new ModelClass(kXCOUNTER_KAISI,errorFlag);
	Counter[1] = new ModelClass(kXCOUNTER_1,errorFlag);
	Counter[2] = new ModelClass(kXCOUNTER_2,errorFlag);
	Counter[3] = new ModelClass(kXCOUNTER_3,errorFlag);
	RaceFinish = new ModelClass(kXRACEFINISH,errorFlag);
}
//デストラクタ
RaceClass::~RaceClass()
{
	delete MyCar;
	delete RaceMap;
	int i;
	for(i=0;i<4;i++)
	{
		delete Counter[i];
	}
	delete RaceFinish;
}
//自車の本体を返す
theObject RaceClass::ReturnMyCarBody()
{
	return MyCar->ReturnBody();
}
//MapのObjectを返す
theObject RaceClass::ReturnMap()
{
	return RaceMap->ReturnMap();
}
//レースを進める(fps60)
void RaceClass::DoRace(void)
{
	if(RaceInfo==kRACE_INFO_COUNT)
	{
		RaceCount();
	}
	if(RaceInfo==kRACE_INFO_RACE)
	{
		AppRaceKeyState();
	}
	if(RaceInfo==kRACE_INFO_FINISH)
	{
		FinishCount();
	}
	MoveRaceObjects();
	ChkRaceObjects();
	AppObjectsMove();
	MoveRaceCamera();
	ChkEnd();
}
//Objectを動かす
void RaceClass::MoveRaceObjects(void)
{
	MyCar->AppMove();
}
//カメラの移動
void RaceClass::MoveRaceCamera(void)
{
	BOOL pHit;
	DWORD pFaceIndex;
	FLOAT pU;
	FLOAT pV;
	FLOAT pDist;
	LPD3DXBUFFER ppAllHits;
	DWORD pCountOfHits;

//	D3DXVECTOR3 PosBuf;

	SetupObjMatrix( g_pD3DDevice , &RaceMap->ReturnMap() );

	D3DXVECTOR3 BufVec,BufPos;
	BufPos = MyCar->ReturnPos();
	BufPos.y += 15.0f;
	BufVec = MoveForward(MyCar->ReturnPos(), -100.0f, MyCar->ReturnYaw());
//	PosBuf = BufVec;
//	PosBuf.y += 1000.0f;
//	if(D3DXIntersect(g_ObjModels[(RaceMap->ReturnMap()).objType]->ReturnMesh(),&PosBuf,&D3DXVECTOR3( 0.0f , -1.0f , 0.0f ),&pHit,&pFaceIndex,&pU,&pV,&pDist,&ppAllHits,&pCountOfHits) == D3D_OK)
//	{		
//		if(pHit == TRUE)
//		{
//			PosBuf.y -= pDist;
//			BufVec = PosBuf;
//		}
//	}
	BufVec.y += 40.0f;
	gEyePt = BufVec;
	gLookatPt = BufPos;
}
//キーの状態を適応
void RaceClass::AppRaceKeyState(void)
{
	D3DXVECTOR3 PosBuf = MyCar->ReturnPos();
//	DWORD	dwWriteByte;
	//char Textbuf[64];
	if(g_pKeyState[DIK_UP] & 0x80)
	{
		MyCar->Accel(TRUE);
	}
	else
	{
		MyCar->Accel(FALSE);
	}
	if(g_pKeyState[DIK_Z] & 0x80)
	{
		MyCar->Brake(TRUE);
	}
	else
	{
		MyCar->Brake(FALSE);
	}
	int LeftBuf;
	int RightBuf;
	if(g_pKeyState[DIK_LEFT] & 0x80)
	{
		LeftBuf = TRUE;
	}
	else
	{
		LeftBuf = FALSE;
	}
	if(g_pKeyState[DIK_RIGHT] & 0x80)
	{
		RightBuf = TRUE;
	}
	else
	{
		RightBuf = FALSE;
	}
	MyCar->Handle(LeftBuf, RightBuf);
}
//あたり判定
void RaceClass::ChkRaceObjects(void)
{
	theObject ObjBuf,WallBuf;
	D3DXVECTOR3 PosBuf,Pos2Buf,VecBuf,BeforePosBuf,NowPosBuf;
	ObjBuf = RaceMap->ReturnMap();
	WallBuf = RaceMap->ReturnWall();

	DWORD pFaceIndex;
	FLOAT pDist;

	BOOL pHit;
	FLOAT pU;
	FLOAT pV;
	FLOAT pDist1;
	LPD3DXBUFFER ppAllHits;
	DWORD pCountOfHits;
	DWORD pFaceIndex1;

	IDirect3DIndexBuffer9* IndexBuf;
	IDirect3DVertexBuffer9* VertexBuf;
	WORD* pIndices;
	BYTE* pVertices;
	D3DXVECTOR3 v0, v1, v2;
	DWORD fvfSize;
	D3DXVECTOR3 arm0, arm1;
	D3DXVECTOR3 nrm;

	D3DXPLANE plane;

	int HitCounter = 0;

	BOOL HitFlag = FALSE;

	D3DXVECTOR3 BufVec;

	g_ObjModels[WallBuf.objType]->ReturnMesh()->GetIndexBuffer(&IndexBuf);
	g_ObjModels[WallBuf.objType]->ReturnMesh()->GetVertexBuffer(&VertexBuf);

	SetupObjMatrix( g_pD3DDevice , &RaceMap->ReturnMap() );

	
	if(CourseType==kCTYPE_GAKE)
	{
		NowPosBuf.y += 10.0f;
		BeforePosBuf.y +=10.0f;
	}
	int i;
	for(i=0;i<4;i++)
	{
		switch(i)
		{
			case 0:
				PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()+kPI_HALF);
				break;
			case 1:
				PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()-kPI_HALF);
				break;
			case 2:
				PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),-10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()+kPI_HALF);
				break;
			case 3:
				PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),-10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()-kPI_HALF);
				break;

		}
		Pos2Buf = PosBuf;
		NowPosBuf = MyCar->ReturnPos();
		BeforePosBuf = MyCar->ReturnBeforePos();
		Pos2Buf.y = PosBuf.y + 500.0f;
		if(D3DXIntersect(g_ObjModels[ObjBuf.objType]->ReturnMesh(),&Pos2Buf,&D3DXVECTOR3( 0.0f , -1.0f , 0.0f ),&pHit,&pFaceIndex1,&pU,&pV,&pDist1,&ppAllHits,&pCountOfHits) == D3D_OK)
		{		
			if(pHit == TRUE)
			{
				PosBuf.y = Pos2Buf.y - pDist1;
				MyCar->SetPos(PosBuf);
			}
			else
			{
				if(ChkHitWall(&BeforePosBuf,&NowPosBuf,&pDist,&pFaceIndex)==TRUE)
				{
					IndexBuf->Lock(3*sizeof(WORD)*pFaceIndex, 3*sizeof(WORD), (VOID**)&pIndices, D3DLOCK_READONLY);
					VertexBuf->Lock(0, 0, (VOID**)&pVertices, D3DLOCK_READONLY);
	
					fvfSize = D3DXGetFVFVertexSize((g_ObjModels[WallBuf.objType]->ReturnMesh())->GetFVF());
	
					v0 = *(D3DXVECTOR3*)( pVertices + fvfSize*pIndices[0] );
					v1 = *(D3DXVECTOR3*)( pVertices + fvfSize*pIndices[1] );
					v2 = *(D3DXVECTOR3*)( pVertices + fvfSize*pIndices[2] );
		
					IndexBuf->Unlock();
					VertexBuf->Unlock();
					IndexBuf->Release();
					VertexBuf->Release();
				
					arm0 = v1 - v0;
					arm1 = v2 - v0;

					D3DXVec3Cross(&nrm, &arm0, &arm1);
					D3DXVec3Normalize(&nrm,&nrm);
					D3DXVECTOR3 HitPos;
					HitPos = BeforePosBuf + (*D3DXVec3Normalize(&BufVec,&(NowPosBuf - BeforePosBuf)))*pDist;
					float BeforeX,BeforeZ,AfterX,AfterZ;
					BeforeX = BeforePosBuf.x - HitPos.x;
					BeforeZ = BeforePosBuf.z - HitPos.z;
					AfterX = 2*(BeforeX*nrm.x+BeforeZ*nrm.z)/(nrm.x*nrm.x+nrm.z*nrm.z)*nrm.x-BeforeX;
					AfterZ = 2*(BeforeX*nrm.x+BeforeZ*nrm.z)/(nrm.x*nrm.x+nrm.z*nrm.z)*nrm.z-BeforeZ;
/*					AfterX = 1.0f;
					AfterZ = tan(2*(atan2(nrm.z,nrm.x))-atan2(BeforeZ,BeforeX));*/
/*					float DistBuf;
					DistBuf = abs(nrm.x*BeforeX+(-1*nrm.z)*BeforeZ)/sqrt(nrm.x*nrm.x+(-1*nrm.z)*(-1*nrm.z));
					AfterX = BeforeX + -2*DistBuf*nrm.z;
					AfterZ = BeforeZ + 2*DistBuf*nrm.x;*/
					//(D3DXVECTOR3(AfterX,BeforePosBuf.y,AfterZ)+HitPos-BeforePosBuf)/2+BeforePosBuf
					//MyCar->SetPos(D3DXVECTOR3(AfterX,BeforePosBuf.y,AfterZ));
					MyCar->SetPos((D3DXVECTOR3(AfterX,0.0f,AfterZ)+HitPos-BeforePosBuf)/2.0f+BeforePosBuf);
					//MyCar->SetSpeed(D3DXVec3Length(&((D3DXVECTOR3(AfterX,0.0f,AfterZ)+HitPos-BeforePosBuf)/2.0f)));
					MyCar->SetSpeed(MyCar->ReturnSpeed()/2);

					switch(i)
					{
						case 0:
							PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()+kPI_HALF);
							break;
						case 1:
							PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()-kPI_HALF);
							break;
						case 2:
							PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),-10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()+kPI_HALF);
							break;
						case 3:
							PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),-10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()-kPI_HALF);
							break;

					}
					Pos2Buf = PosBuf;
					NowPosBuf = MyCar->ReturnPos();
					BeforePosBuf = MyCar->ReturnBeforePos();
					Pos2Buf.y = PosBuf.y + 500.0f;
					if(D3DXIntersect(g_ObjModels[ObjBuf.objType]->ReturnMesh(),&Pos2Buf,&D3DXVECTOR3( 0.0f , -1.0f , 0.0f ),&pHit,&pFaceIndex,&pU,&pV,&pDist,&ppAllHits,&pCountOfHits) == D3D_OK)
					{
						if(pHit == TRUE)
						{
							PosBuf.y = Pos2Buf.y - pDist;
							MyCar->SetPos(PosBuf);
						}
						else
						{
							BufVec = MyCar->ReturnPos();
							MyCar->SetPos(BufVec+nrm);
							switch(i)
							{
								case 0:
									PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()+kPI_HALF);
									break;
								case 1:
									PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()-kPI_HALF);
									break;
								case 2:
									PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),-10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()+kPI_HALF);
									break;
								case 3:
									PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),-10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()-kPI_HALF);
									break;

							}
							Pos2Buf = PosBuf;
							NowPosBuf = MyCar->ReturnPos();
							BeforePosBuf = MyCar->ReturnBeforePos();
							Pos2Buf.y = PosBuf.y + 500.0f;
							if(D3DXIntersect(g_ObjModels[ObjBuf.objType]->ReturnMesh(),&Pos2Buf,&D3DXVECTOR3( 0.0f , -1.0f , 0.0f ),&pHit,&pFaceIndex,&pU,&pV,&pDist,&ppAllHits,&pCountOfHits) == D3D_OK)
							{
								if(pHit == TRUE)
								{
									PosBuf.y = Pos2Buf.y - pDist;
									MyCar->SetPos(PosBuf);
								}
								else
								{
									MyCar->SetPos(BufVec-nrm);
									switch(i)
									{
										case 0:
											PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()+kPI_HALF);
											break;
										case 1:
											PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()-kPI_HALF);
											break;
										case 2:
											PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),-10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()+kPI_HALF);
											break;
										case 3:
											PosBuf = MoveForward(MoveForward(MyCar->ReturnPos(),-10.0f,MyCar->ReturnYaw()),6.0f,MyCar->ReturnYaw()-kPI_HALF);
											break;

									}
									Pos2Buf = PosBuf;
									NowPosBuf = MyCar->ReturnPos();
									BeforePosBuf = MyCar->ReturnBeforePos();
									Pos2Buf.y = PosBuf.y + 500.0f;
									if(D3DXIntersect(g_ObjModels[ObjBuf.objType]->ReturnMesh(),&Pos2Buf,&D3DXVECTOR3( 0.0f , -1.0f , 0.0f ),&pHit,&pFaceIndex,&pU,&pV,&pDist,&ppAllHits,&pCountOfHits) == D3D_OK)
									{
										if(pHit == TRUE)
										{
											PosBuf.y = Pos2Buf.y - pDist;
											MyCar->SetPos(PosBuf);
										}
									}
								}
							}
						}
					}
					/*MyCar->SetSpeed(D3DXVec3Length(&((MyCar->ReturnPos())-BeforePosBuf))/2);
					Buf2Vec *= 0.001f; 
					BufVec += Buf2Vec;
					Pos2Buf = BufVec;
					Pos2Buf.y += 500.0f;
					
					do
					{
						if(D3DXIntersect(g_ObjModels[ObjBuf.objType]->ReturnMesh(),&Pos2Buf,&D3DXVECTOR3( 0.0f , -1.0f , 0.0f ),&pHit,&pFaceIndex,&pU,&pV,&pDist,&ppAllHits,&pCountOfHits) == D3D_OK)
						{
							BufVec += Buf2Vec;
							Pos2Buf = BufVec;
							Pos2Buf.y += 500.0f;
							HitCounter++;
						}
						else
						{
							break;
						}
					}while((pHit==FALSE)&&(HitCounter<50));
					if(HitCounter>=10)
					{
						PosBuf = MyCar->ReturnPos();
						Pos2Buf = MyCar->ReturnPos();
						Pos2Buf.y = PosBuf.y + 500.0f;
						BufVec = MyCar->ReturnBeforePos();

						IndexBuf->Lock(3*sizeof(WORD)*pFaceIndex, 3*sizeof(WORD), (VOID**)&pIndices, D3DLOCK_READONLY);
						VertexBuf->Lock(0, 0, (VOID**)&pVertices, D3DLOCK_READONLY);

						fvfSize = D3DXGetFVFVertexSize((g_ObjModels[WallBuf.objType]->ReturnMesh())->GetFVF());

						v0 = *(D3DXVECTOR3*)( pVertices + fvfSize*pIndices[0] );
						v1 = *(D3DXVECTOR3*)( pVertices + fvfSize*pIndices[1] );
						v2 = *(D3DXVECTOR3*)( pVertices + fvfSize*pIndices[2] );

						IndexBuf->Unlock();
						VertexBuf->Unlock();
						IndexBuf->Release();
						VertexBuf->Release();

						arm0 = v1 - v0;
						arm1 = v2 - v0;

						Buf2Vec = *D3DXVec3Normalize(&nrm,&(*D3DXVec3Cross(&nrm, &arm0, &arm1)))*(D3DXVec3Length(&(NowPosBuf-BeforePosBuf))+1.0f);
						Buf2Vec *= 0.001f; 
						BufVec += Buf2Vec;
						Pos2Buf = BufVec;
						Pos2Buf.y += 500.0f;

						HitCounter = 0;
						do
						{
							if(D3DXIntersect(g_ObjModels[ObjBuf.objType]->ReturnMesh(),&Pos2Buf,&D3DXVECTOR3( 0.0f , -1.0f , 0.0f ),&pHit,&pFaceIndex,&pU,&pV,&pDist,&ppAllHits,&pCountOfHits) == D3D_OK)
							{
								BufVec += Buf2Vec;
								Pos2Buf = BufVec;
								Pos2Buf.y += 500.0f;
								HitCounter++;
							}
							else
							{
								break;
							}
						}while((pHit==FALSE)&&(HitCounter<50));
					}
					if(D3DXIntersect(g_ObjModels[ObjBuf.objType]->ReturnMesh(),&Pos2Buf,&D3DXVECTOR3( 0.0f , -1.0f , 0.0f ),&pHit,&pFaceIndex,&pU,&pV,&pDist,&ppAllHits,&pCountOfHits) == D3D_OK)
					{		
						if(pHit == TRUE)
						{
							BufVec.y = Pos2Buf.y - pDist;
							MyCar->SetPos(BufVec);
						}
					}

					//MyCar->SlowDown(D3DXVec3Length(&Buf2Vec)/10.0f);
					MyCar->SetSpeed(D3DXVec3Length(&(BufVec-(MyCar->ReturnBeforePos()))));*/
				}
			}
		}
	}
}
//壁のObjectを返す
theObject RaceClass::ReturnWall()
{
	return RaceMap->ReturnWall();
}
//動きを適応する
void RaceClass::AppObjectsMove(void)
{
	MyCar->AppObjMove();
}
//レースの描画
void RaceClass::RaceRender(LPDIRECT3DDEVICE9 theDevice)
{
	// Zバッファとバックバッファのクリア
	theDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,64), 1.0f, 0 );

	// シーンの開始
	if( SUCCEEDED( theDevice->BeginScene() ) )
	{
//		DWORD i0;						// カウンタ

		//=================================================================キャラクタレイヤの描画
		SetupRanderStates( theDevice , kRS_MESH );	// レンダリングステート設定

		gLightLight.Type       = D3DLIGHT_DIRECTIONAL;
		gLightLight.Diffuse.r  = 1.0f;
		gLightLight.Diffuse.g  = 1.0f;
		gLightLight.Diffuse.b  = 1.0f;
		gLightLight.Diffuse.a  = 0.0f;
		gLightVec = D3DXVECTOR3(0.0f,-1.0f,0.0f);
		SetupLights(theDevice,gLightVec,gLightLight,0);		// ライトの設定
		//SetupSpotLights( theDevice );	// スポットライトの設定

		SetupWorldMatrix( theDevice , &gEyePt , &gLookatPt , &gUpVec );// カメラ視点設定

		// モデルの描画
		drawMesh( theDevice , &ReturnMyCarBody() ,g_ObjModels[ReturnMyCarBody().objType]);
		drawMesh( theDevice , &ReturnMap() ,g_ObjModels[ReturnMap().objType]);
		drawMesh( theDevice , &ReturnWall() ,g_ObjModels[ReturnWall().objType]);
	
		if((HyouziCount > -1)||(RaceInfo == kRACE_INFO_FINISH))
		{
			SetupWorldMatrix(theDevice,&gBillEyePt,&gBillLookatPt,&gBillUp);
			SetupRanderStates(theDevice,kRS_INFO);
			if(HyouziCount > -1)
			{
				drawMesh(theDevice,&CountBord,Counter[HyouziCount]);
			}
			if(RaceInfo == kRACE_INFO_FINISH)
			{
				drawMesh(theDevice,&CountBord,RaceFinish);
			}
		}
		if(AfterCount == TRUE)
		{
			CountDown--;
			if(CountDown < -60)
			{
				HyouziCount = -1;
				AfterCount =FALSE;
			}
		}
/*
		// ビルボードをαブレンディングで描画
		// 適切なテクスチャを用意すれば、モデルもαブレンディングで描画できる
		SetupRanderStates( theDevice , kRS_ALPHA );		// レンダリングステート設定
		for( i0 = 0 ; i0 < kNUM_OF_BILLBORDS ; i0++ )
		{
			if((g_Billbord[i0].LDflag != kLD_STANDBY)&&(g_Billbord[i0].TexType == kTEX_EXPLODE1 ))
			{  drawFVF( theDevice , &g_Billbord[i0] , &gEyePt ); }
		}

		// ビルボードを加算合成で描画
		// モデルもそのままで描画できる
		SetupRanderStates( theDevice , kRS_ADDBLEND );	// レンダリングステート設定
		for( i0 = 0 ; i0 < kNUM_OF_BILLBORDS ; i0++ )
		{
			if((g_Billbord[i0].LDflag != kLD_STANDBY)&&(g_Billbord[i0].TexType == kTEX_EXPLODE2 ))
			{  drawFVF( theDevice , &g_Billbord[i0] , &gEyePt ); }
		}

		
		//=================================================================情報レイヤの描画
		// Zバッファとバックバッファのクリア
		theDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

		SetupWorldMatrix( theDevice , &gBilEyePt , &gLookatPt , &gBillUpVec );// 情報レイヤ用の視点設定
		//SetupRanderStates( theDevice , kRS_ADDBLEND );
		SetupRanderStates( theDevice , kRS_ALPHA );

		for( i0 = 0 ; i0 < kNUM_OF_INFOBORDS ; i0++ )
		{
			if( g_Infobord[i0].TexType != kTEX_NONE ){ drawFVF( theDevice , &g_Infobord[i0] , &gBilEyePt ); }
		}
*/
		//終わりの定型処理
		// シーンの終了
		theDevice->EndScene();
	}

	// バックバッファを表画面に反映させる。
	HRESULT hr = theDevice->Present( NULL, NULL, NULL, NULL );
}
//終わりかどうかを確認
void RaceClass::ChkEnd(void)
{
	BOOL pHit;
	DWORD pFaceIndex;
	FLOAT pU;
	FLOAT pV;
	FLOAT pDist;
	LPD3DXBUFFER ppAllHits;
	DWORD pCountOfHits;

	SetupObjMatrix( g_pD3DDevice , &RaceMap->ReturnEnd() );

	D3DXVECTOR3 NowPosBuf = MyCar->ReturnPos();
	D3DXVECTOR3 BeforePosBuf = MyCar->ReturnBeforePos();
	theObject EndBuf = RaceMap->ReturnEnd();
	if(D3DXIntersect(g_ObjModels[EndBuf.objType]->ReturnMesh(),&BeforePosBuf,&(NowPosBuf-BeforePosBuf),&pHit,&pFaceIndex,&pU,&pV,&pDist,&ppAllHits,&pCountOfHits)==D3D_OK)
	{
		if((pHit==TRUE)&&(pDist<10.0f))
		{
			RaceInfo = kRACE_INFO_FINISH;
			gGameResultTime = timeGetTime() - StartTime;
			MyCar->Stop();
		}
	}
}
//カウントダウン
void RaceClass::RaceCount(void)
{
	if((CountDown <= 180)&&(CountDown >120))
	{
		HyouziCount = 3;
	}
	if((CountDown <= 120)&&(CountDown > 60))
	{
		HyouziCount = 2;
	}
	if((CountDown <= 60)&&(CountDown > 0))
	{
		HyouziCount = 1;
	}
	if(CountDown == 0)
	{
		HyouziCount = 0;
		AfterCount = TRUE;
		RaceInfo = kRACE_INFO_RACE;
		StartTime = timeGetTime();
	}
	CountDown--;
}
//終了
void RaceClass::FinishCount(void)
{
	if(finishCounter < 1)
	{
		g_GameMode = kMODE_END_RACE;
	}
	finishCounter--;
}
//壁と当たってるか判定
BOOL RaceClass::ChkHitWall(D3DXVECTOR3* StartPos,D3DXVECTOR3* EndPos,FLOAT* pDist,DWORD* pFaceIndex)
{
	theObject WallBuf = RaceMap->ReturnWall();

	BOOL pHit1,pHit2;
	FLOAT pU;
	FLOAT pV;
	FLOAT pDist1;
	LPD3DXBUFFER ppAllHits,ppAllHits1;
	DWORD pCountOfHits,pCountOfHits1;
	DWORD pFaceIndex1;
	if((D3DXIntersect(g_ObjModels[WallBuf.objType]->ReturnMesh(),EndPos,&(*StartPos-*EndPos),&pHit1,&pFaceIndex1,&pU,&pV,&pDist1,&ppAllHits1,&pCountOfHits1)==D3D_OK)
		&&(D3DXIntersect(g_ObjModels[WallBuf.objType]->ReturnMesh(),StartPos,&(*EndPos-*StartPos),&pHit2,pFaceIndex,&pU,&pV,pDist,&ppAllHits,&pCountOfHits)==D3D_OK))
	{
		if(pHit1 && pHit2)
		{
			int i;
			for(i=0;i<pCountOfHits1;i++)
			{
				if(static_cast<D3DXINTERSECTINFO*>(ppAllHits1->GetBufferPointer())[i].FaceIndex == *pFaceIndex)
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}