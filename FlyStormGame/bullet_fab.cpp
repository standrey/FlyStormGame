#include "bullet_fab.h"


struct bulletVertex
{
	D3DXVECTOR3 position;
	float tu, tv;

	bulletVertex(const D3DXVECTOR3 & pos, float u, float v)
	{
		position = pos;
		tu = u;
		tv = v;
	}
};


BulletFabric::BulletFabric() {}

BulletFabric::BulletFabric(IDirect3DDevice9 * d3ddev) {
	
	D3DVERTEXELEMENT9 rawBulletVertex[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	D3DXQuaternionIdentity(&localRotation);

	l_d3ddev = d3ddev;

	HRESULT hr;
	
	//hr = D3DXCreateTextureFromFile(d3ddev, L"gameresources/bullet.jpg", &texture);

	hr = D3DXCreateTextureFromFileExA(l_d3ddev, //device
                             "gameresources/bullet.jpg",        //file name
                             D3DX_DEFAULT,      //width
                             D3DX_DEFAULT,      //height
                             D3DX_DEFAULT,      //mip levels
                             NULL,              //usage
                             D3DFMT_UNKNOWN,    //texture color format
                             D3DPOOL_MANAGED,   //memory class
                             D3DX_DEFAULT,      //filter
                             D3DX_DEFAULT,      //mip filter
                             0xff000000,        //color key
                             NULL,              //source info
                             NULL,              //pallette
                             &texture);    //texture object

	//hr = D3DXCreateTextureFromFile(d3ddev, L"gameresources/radiation_box.tga", &texture);

	if (FAILED(hr)) {
		MessageBox(NULL, L"D3DXCreateTexture", L"Error", MB_ICONERROR);
		return;
	}
	

	hr = d3ddev->CreateVertexDeclaration(rawBulletVertex, &bulletVertexDecl);
	if (FAILED(hr)) {
		MessageBox(NULL, L"CreateVertexDeclaration", L"Error", MB_ICONERROR);
		return;
	}

	bulletVertex rawVertices[] = {
		bulletVertex(D3DXVECTOR3( 0.1f,  0.1f, 0), (float)0, (float)0),
		bulletVertex(D3DXVECTOR3(-0.1f,  0.1f, 0), (float)1, (float)0),
		bulletVertex(D3DXVECTOR3( 0.1f, -0.1f, 0), (float)0, (float)1),
		bulletVertex(D3DXVECTOR3(-0.1f, -0.1f, 0), (float)1, (float)1)
	};

	void * lock = NULL;
	int vbSize = sizeof(rawVertices);
	hr = d3ddev->CreateVertexBuffer(vbSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &squareBullet, 0);
	if (FAILED(hr))
		return;
	squareBullet->Lock(0, vbSize, &lock, 0);
	memcpy(lock, rawVertices, vbSize);
	squareBullet->Unlock();
}

HRESULT BulletFabric::Draw()
{
	l_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	l_d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	l_d3ddev->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	l_d3ddev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) ;

	l_d3ddev->SetTexture(0, texture);
	l_d3ddev->SetVertexDeclaration(bulletVertexDecl);

	D3DXMATRIX m_Rot;
	D3DXMatrixRotationQuaternion(&m_Rot, &localRotation);

	for(std::deque<Bullet *>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++)  {
		D3DXMATRIX matWorld;
		D3DXMatrixTranslation( &matWorld, (*iter)->worldPosition.x, (*iter)->worldPosition.y, (*iter)->worldPosition.z);
		D3DXMatrixMultiply(&matWorld, &m_Rot, &matWorld);

		l_d3ddev->SetStreamSource( 0, squareBullet, 0, sizeof(float) * 5 );
		l_d3ddev->SetTransform( D3DTS_WORLD, &matWorld );
		l_d3ddev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	}
	l_d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	l_d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
	return S_OK;
}

void BulletFabric::AddBullet(D3DXQUATERNION startRotation, D3DXVECTOR3 startPos) {
	bulletList.push_back(new Bullet(startPos, startRotation));
}

void BulletFabric::setRotationBullets(D3DXQUATERNION rotQuat) {
	localRotation = rotQuat;
}

void BulletFabric::RemoveFirstBullet() {
	if (bulletList.empty())
		return;
	Bullet * firstBullet = *(bulletList.begin());
	delete firstBullet;
	bulletList.pop_front();
}

void BulletFabric::moveForward(float speed) {
	for(std::deque<Bullet *>::iterator iter = bulletList.begin(); 
		iter != bulletList.end(); 
		iter++)
		(*iter)->worldPosition += (*iter)->moveDir * speed;

}

BulletFabric::~BulletFabric()
{
	//разворачиваем пули в нужном направлении
	for(std::deque<Bullet *>::iterator iter = bulletList.begin();
		iter != bulletList.end();
		iter++)
		delete *iter;
	
	if (texture)
		texture->Release();

	if (squareBullet) {
		squareBullet->Release();
		squareBullet = NULL;
	}
	if (bulletVertexDecl) {
		bulletVertexDecl->Release();
		bulletVertexDecl = NULL;
	}
}