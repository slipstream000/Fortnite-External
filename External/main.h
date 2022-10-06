//used for logging/cycling through values
bool logger = true;
int countnum = 1;
char szString[64];

#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = nullptr; } }

//==========================================================================================================================

//get dir
using namespace std;
#include <fstream>
char dlldir[320];
char *GetDirectoryFile(char *filename)
{
	static char path[320];
	strcpy_s(path, dlldir);
	strcat_s(path, filename);
	return path;
}

//log

//==========================================================================================================================

//generate shader func
HRESULT GenerateShader(ID3D11Device* pD3DDevice, ID3D11PixelShader** pShader, float r, float g, float b)
{
	char szCast[] = "struct VS_OUT"

	ID3D10Blob* pBlob;
	char szPixelShader[1000];

	sprintf_s(szPixelShader, szCast, r, g, b);

	ID3DBlob* d3dErrorMsgBlob;

	HRESULT hr = D3DCompile(szPixelShader, sizeof(szPixelShader), "shader", NULL, NULL, "main", "ps_4_0", NULL, NULL, &pBlob, &d3dErrorMsgBlob);

	if (FAILED(hr))
		return hr;

	hr = pD3DDevice->CreatePixelShader((DWORD*)pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pShader);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Vec4
{
	float x, y, z, w;
};



void MapBuffer(ID3D11Buffer* pStageBuffer, void** ppData, UINT* pByteWidth)
{
	D3D11_MAPPED_SUBRESOURCE subRes;
	HRESULT res = pContext->Map(pStageBuffer, 0, D3D11_MAP_READ, 0, &subRes);

	D3D11_BUFFER_DESC desc;
	pStageBuffer->GetDesc(&desc);

	if (FAILED(res))
	{
		Log("Map stage buffer failed {%d} {%d} {%d} {%d} {%d}", (void*)pStageBuffer, desc.ByteWidth, desc.BindFlags, desc.CPUAccessFlags, desc.Usage);
	}

	*ppData = subRes.pData;

	if (pByteWidth)
		*pByteWidth = desc.ByteWidth;
}

void UnmapBuffer(ID3D11Buffer* pStageBuffer)
{
	pContext->Unmap(pStageBuffer, 0);
}

ID3D11Buffer* CopyBufferToCpu(ID3D11Buffer* pBuffer)
{
	D3D11_BUFFER_DESC CBDesc;
	pBuffer->GetDesc(&CBDesc);

	ID3D11Buffer* pStageBuffer = NULL;
	{ // create shadow buffer.
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = 0;
		desc.ByteWidth = CBDesc.ByteWidth;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_STAGING;

		if (FAILED(pDevice->CreateBuffer(&desc, NULL, &pStageBuffer)))
		{
			Log("CreateBuffer failed when CopyBufferToCpu {%d}", CBDesc.ByteWidth);
		}
	}

	if (pStageBuffer != NULL)
		pContext->CopyResource(pStageBuffer, pBuffer);

	return pStageBuffer;
}


//w2s
int WorldViewCBnum = 2;
int ProjCBnum = 1;
int matProjnum = 16;
//Game			WorldViewCBnum		ProjCBnum		matProjnum
//UT4 Alpha		2					1				16		
//Fortnite		2					1				16
//Outlast 		0					1				0 and 16
//Warframe		0					0				0 or 4
ID3D11Buffer* pWorldViewCB = nullptr;
ID3D11Buffer* pProjCB = nullptr;
ID3D11Buffer* m_pCurWorldViewCB = nullptr;
ID3D11Buffer* m_pCurProjCB = nullptr;
void AddModel(ID3D11DeviceContext* pContext)
{
	//Warning, this is NOT optimized:

	pContext->VSGetConstantBuffers(WorldViewCBnum, 1, &pWorldViewCB);//WorldViewCBnum

	pContext->VSGetConstantBuffers(ProjCBnum, 1, &pProjCB);//ProjCBnum

	if (pWorldViewCB == NULL)
	{
		SAFE_RELEASE(pWorldViewCB)
		//return; here only if a game is crashing
	}

	if (pProjCB == NULL)
	{
		SAFE_RELEASE(pProjCB)
		//return; here only if a game is crashing
	}

	//WORLDVIEW
	if (pWorldViewCB != NULL)
		m_pCurWorldViewCB = CopyBufferToCpu(pWorldViewCB);
	SAFE_RELEASE(pWorldViewCB);

	float matWorldView[4][4];
	{
		float* WorldViewCB;
		MapBuffer(m_pCurWorldViewCB, (void**)&WorldViewCB, NULL);
		memcpy(matWorldView, &WorldViewCB[0], sizeof(matWorldView));
		matWorldView[3][2] = matWorldView[3][2] + (aimheight * 20);		//aimheight can be done here for body parts
		UnmapBuffer(m_pCurWorldViewCB);
		SAFE_RELEASE(m_pCurWorldViewCB);
	}
	Vec3 v;
	Vec4 vWorldView = Vec3MulMat4x4(v, matWorldView);


	//PROJECTION
	if (pProjCB != NULL)
		m_pCurProjCB = CopyBufferToCpu(pProjCB);
	SAFE_RELEASE(pProjCB);

	float matProj[4][4];
	{

	}
	Vec4 vWorldViewProj = Vec4MulMat4x4(vWorldView, matProj);


	Vec2 o;
	o.x = ScreenCenterX + ScreenCenterX * vWorldViewProj.x / vWorldViewProj.w;
	o.y = ScreenCenterY + ScreenCenterY * -vWorldViewProj.y / vWorldViewProj.w;
}

//==========================================================================================================================
