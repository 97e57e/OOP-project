////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <math.h>

IDirect3DDevice9* Device = NULL;

// window size
const int Width = 1024;
const int Height = 768;

// There are four balls
// initialize the position (coordinate) of each ball (ball0 ~ ball15)
const float spherePos[22][2] = { { +1.6f,0 } ,{ +1.95f,+0.35f } ,{ +1.95f,-0.35f } ,{ +2.3f,+0.7f } ,{ +2.3f,0 } ,{ +2.3f,-0.7f } ,{ +2.65f,+1.05f } ,{ +2.65f,+0.35f } ,
{ +2.65f,-0.35f } ,{ +2.65f,-1.05f } ,{ +3.0f,+1.4f } ,{ +3.0f,+0.7f } ,{ +3.0f,0 } ,{ +3.0f,-0.7f } ,{ +3.0f,-1.4f } ,{ -2.5f,0 } ,
{ -4.45f, 2.95f },{ 0, 3.0f },{ +4.45f, 2.95f },{ 4.45f, -2.95f },{ 0, -3.0f },{ -4.45f, -2.95f } };
// initialize the color of each ball (ball0 ~ ball15)
const D3DXCOLOR sphereColor[22] = { d3d::YELLOW, d3d::YELLOW , d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::BLACK, d3d::GREEN, d3d::GREEN, d3d::GREEN, d3d::GREEN, d3d::GREEN, d3d::GREEN, d3d::GREEN, d3d::WHITE ,
d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK };

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982

// -----------------------------------------------------------------------------
// CSphere class definition 
// -----------------------------------------------------------------------------

class CSphere {
private:
	float               center_x, center_y, center_z;
	float                   m_radius;
	float               m_velocity_x;
	float               m_velocity_z;
	bool               checkFoul = false;

public:
	CSphere(void)
	{
		D3DXMatrixIdentity(&m_mLocal); //D3DXMatrixIdentity : 단위행렬로 초기화
		ZeroMemory(&m_mtrl, sizeof(m_mtrl)); //zeromemory : 문자열을 NULL로 초기화
		m_radius = 0;
		m_velocity_x = 0;
		m_velocity_z = 0;
		m_pSphereMesh = NULL;
	}
	~CSphere(void) {}

public:
	//create함수 : CSphere 객체의 색을 초기화하고 생성
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
			return false;
		return true;
	}

	void destroy(void)
	{
		if (m_pSphereMesh != NULL) {
			m_pSphereMesh->Release();
			m_pSphereMesh = NULL;
		}
	}

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pSphereMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		double dist = (center_x - ball.center_x) * (center_x - ball.center_x) + (center_z - ball.center_z) * (center_z - ball.center_z);
		dist = sqrt(dist);

		if (dist <= 2 * getRadius())
			return true;

		return false;
	}

	bool hitBy(CSphere& ball)
	{
		bool i;
		i = this->hasIntersected(ball);
		if (i == true)
		{
			D3DXVECTOR3 ball_cord = ball.getCenter();

			double distance = sqrt(((center_x - ball_cord.x) * (center_x - ball_cord.x)) + ((center_z - ball_cord.z) * (center_z - ball_cord.z)));

			double vax = this->m_velocity_x;
			double vaz = this->m_velocity_z;
			double vbx = ball.m_velocity_x;
			double vbz = ball.m_velocity_z;

			double cos = (center_x - ball_cord.x) / distance;
			double sin = (center_z - ball_cord.z) / distance;

			double vaxp = vbx * cos + vbz * sin;
			double vbxp = vax * cos + vaz * sin;
			double vazp = vaz * cos - vax * sin;
			double vbzp = vbz * cos - vbx * sin;

			this->setPower(vaxp * cos - vazp * sin, vaxp * sin + vazp * cos);
			ball.setPower(vbxp * cos - vbzp * sin, vbxp * sin + vbzp * cos);

			if (this->center_x > ball.getCenter().x && this->center_z > ball.getCenter().z) {
				this->setCenter(this->center_x + EPSILON, this->center_y, this->center_z + EPSILON);
				ball.setCenter(ball.getCenter().x - EPSILON, ball.getCenter().y, ball.getCenter().z - EPSILON);
			}
			if (this->center_x > ball.getCenter().x && this->center_z < ball.getCenter().z) {
				this->setCenter(this->center_x + EPSILON, this->center_y, this->center_z - EPSILON);
				ball.setCenter(ball.getCenter().x - EPSILON, ball.getCenter().y, ball.getCenter().z + EPSILON);
			}
			if (this->center_x < ball.getCenter().x && this->center_z > ball.getCenter().z) {
				this->setCenter(this->center_x - EPSILON, this->center_y, this->center_z + EPSILON);
				ball.setCenter(ball.getCenter().x + EPSILON, ball.getCenter().y, ball.getCenter().z - EPSILON);
			}
			if (this->center_x < ball.getCenter().x && this->center_z < ball.getCenter().z) {
				this->setCenter(this->center_x - EPSILON, this->center_y, this->center_z - EPSILON);
				ball.setCenter(ball.getCenter().x + EPSILON, ball.getCenter().y, ball.getCenter().z + EPSILON);
			}

			return true;
		}
		return false;
	}


	bool hasIntersectedToHole(CSphere& ball)
	{
		double dist = (center_x - ball.center_x) * (center_x - ball.center_x) + (center_z - ball.center_z) * (center_z - ball.center_z);
		dist = sqrt(dist);

		if (dist <= 1.5 * getRadius())
			return true;

		return false;
	}

	bool hitByHole(CSphere& ball)
	{
		D3DXVECTOR3 ball_center = ball.getCenter();
		if (hasIntersectedToHole(ball) == true) {
			ball.setCenter(10000, 10000, 10000);
			ball.checkFoul = true;

			return true;
		}
		return false;
	}

	bool getFoul() {
		return checkFoul;
	}
	void setFoul() {
		checkFoul = false;
	}

	void ballUpdate(float timeDiff)
	{
		const float TIME_SCALE = 3.3;
		D3DXVECTOR3 cord = this->getCenter();
		double vx = abs(this->getVelocity_X());
		double vz = abs(this->getVelocity_Z());

		if (vx > 0.01 || vz > 0.01)
		{
			float tX = cord.x + TIME_SCALE*timeDiff*m_velocity_x;
			float tZ = cord.z + TIME_SCALE*timeDiff*m_velocity_z;

			//correction of position of ball
			// Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
			/*if(tX >= (4.5 - M_RADIUS))
			tX = 4.5 - M_RADIUS;
			else if(tX <=(-4.5 + M_RADIUS))
			tX = -4.5 + M_RADIUS;
			else if(tZ <= (-3 + M_RADIUS))
			tZ = -3 + M_RADIUS;
			else if(tZ >= (3 - M_RADIUS))
			tZ = 3 - M_RADIUS;*/

			this->setCenter(tX, cord.y, tZ);
		}
		else { this->setPower(0, 0); }
		//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
		double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
		if (rate < 0)
			rate = 0;
		this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
	}

	double getVelocity_X() { return this->m_velocity_x; }
	double getVelocity_Z() { return this->m_velocity_z; }

	void setPower(double vx, double vz)
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}

	void setCenter(float x, float y, float z)
	{
		D3DXMATRIX m;
		center_x = x;   center_y = y;   center_z = z;
		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getRadius(void)  const { return (float)(M_RADIUS); }
	const D3DXMATRIX& getLocalTransform(void) const { return m_mLocal; }
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	D3DXVECTOR3 getCenter(void) const
	{
		D3DXVECTOR3 org(center_x, center_y, center_z);
		return org;
	}

private:
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh*              m_pSphereMesh;

};

// -----------------------------------------------------------------------------
// CPlayer class definition
// -----------------------------------------------------------------------------

class CPlayer
{
private:
	bool   MyTurn;
	char   MyBallType;
	int    LeftBall;
	bool   result;

public:

	void setMyTurn(bool A) { MyTurn = A; }
	bool getMyTurn() { return MyTurn; }

	void setMyBallType(char A) { MyBallType = A; }
	char getMyBallType() { return MyBallType; }

	void setLeftBall(int A) { LeftBall = A; }
	int getLeftBall() { return LeftBall; }

	void setresult(int A) { result = A; }
	bool getresult() { return result; }

	void PutInMyBall() { LeftBall--; }
};


// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------

class CWall {

private:

	float               m_x;
	float               m_z;
	float                   m_width;
	float                   m_depth;
	float               m_height;

public:
	CWall(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_width = 0;
		m_depth = 0;
		m_pBoundMesh = NULL;
	}
	~CWall(void) {}
public:
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		m_width = iwidth;
		m_depth = idepth;

		if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
			return false;
		return true;
	}
	void destroy(void)
	{
		if (m_pBoundMesh != NULL) {
			m_pBoundMesh->Release();
			m_pBoundMesh = NULL;
		}
	}
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pBoundMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		D3DXVECTOR3 cord = ball.getCenter();
		if (cord.x + M_RADIUS >= 4.5f && ball.getVelocity_X() > 0)
			return true;
		if (cord.x - M_RADIUS <= -4.5f  && ball.getVelocity_X() < 0)
			return true;
		if (cord.z + M_RADIUS >= 3.0f  && ball.getVelocity_Z() > 0)
			return true;
		if (cord.z - M_RADIUS <= -3.0f  && ball.getVelocity_Z() < 0)
			return true;
		return false;

	}

	void hitBy(CSphere& ball)
	{
		D3DXVECTOR3 cord = ball.getCenter();
		bool i = hasIntersected(ball);
		if (i == true)
		{
			if (cord.x + M_RADIUS >= 4.5f - (this->m_width / 2) && ball.getVelocity_X() > 0)
			{
				ball.setPower((-1)*ball.getVelocity_X(), ball.getVelocity_Z());
			}
			if (cord.x - M_RADIUS <= -4.5f + (this->m_width / 2) && ball.getVelocity_X() < 0)
			{
				ball.setPower((-1)*ball.getVelocity_X(), ball.getVelocity_Z());
			}
			if (cord.z + M_RADIUS >= 3.0f - (this->m_width / 2) && ball.getVelocity_Z() > 0)
			{
				ball.setPower(ball.getVelocity_X(), (-1)*ball.getVelocity_Z());
			}
			if (cord.z - M_RADIUS <= -3.0f + (this->m_width / 2) && ball.getVelocity_Z() < 0)
			{
				ball.setPower(ball.getVelocity_X(), (-1)*ball.getVelocity_Z());
			}

		}
	}

	void setPosition(float x, float y, float z)
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getHeight(void) const { return M_HEIGHT; }



private:
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }

	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh*              m_pBoundMesh;
};

// -----------------------------------------------------------------------------
// CLight class definition
// -----------------------------------------------------------------------------

class CLight {
public:
	CLight(void)
	{
		static DWORD i = 0;
		m_index = i++;
		D3DXMatrixIdentity(&m_mLocal);
		::ZeroMemory(&m_lit, sizeof(m_lit));
		m_pMesh = NULL;
		m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_bound._radius = 0.0f;
	}
	~CLight(void) {}
public:
	bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f)
	{
		if (NULL == pDevice)
			return false;
		if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))
			return false;

		m_bound._center = lit.Position;
		m_bound._radius = radius;

		m_lit.Type = lit.Type;
		m_lit.Diffuse = lit.Diffuse;
		m_lit.Specular = lit.Specular;
		m_lit.Ambient = lit.Ambient;
		m_lit.Position = lit.Position;
		m_lit.Direction = lit.Direction;
		m_lit.Range = lit.Range;
		m_lit.Falloff = lit.Falloff;
		m_lit.Attenuation0 = lit.Attenuation0;
		m_lit.Attenuation1 = lit.Attenuation1;
		m_lit.Attenuation2 = lit.Attenuation2;
		m_lit.Theta = lit.Theta;
		m_lit.Phi = lit.Phi;
		return true;
	}
	void destroy(void)
	{
		if (m_pMesh != NULL) {
			m_pMesh->Release();
			m_pMesh = NULL;
		}
	}
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return false;

		D3DXVECTOR3 pos(m_bound._center);
		D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
		D3DXVec3TransformCoord(&pos, &pos, &mWorld);
		m_lit.Position = pos;

		pDevice->SetLight(m_index, &m_lit);
		pDevice->LightEnable(m_index, TRUE);
		return true;
	}

	void draw(IDirect3DDevice9* pDevice)
	{
		if (NULL == pDevice)
			return;
		D3DXMATRIX m;
		D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
		pDevice->SetTransform(D3DTS_WORLD, &m);
		pDevice->SetMaterial(&d3d::WHITE_MTRL);
		m_pMesh->DrawSubset(0);
	}

	D3DXVECTOR3 getPosition(void) const { return D3DXVECTOR3(m_lit.Position); }

private:
	DWORD               m_index;
	D3DXMATRIX          m_mLocal;
	D3DLIGHT9           m_lit;
	ID3DXMesh*          m_pMesh;
	d3d::BoundingSphere m_bound;
};


// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall   g_legoPlane;
CWall   g_legowall[4];
CSphere   g_sphere[22];
CPlayer g_player[2];
CSphere   g_target_blueball;
CLight   g_light;
bool STOP = true;
bool FREEBALL = false;
bool Block = true;
bool changeTurn = false;
int first = 100;
ID3DXFont*   Font;
ID3DXSprite*  Sprite;

double g_camera_pos[3] = { 0.0, 5.0,-8.0 };

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
}

// initialization
bool Setup()
{
	int i;

	D3DXMatrixIdentity(&g_mWorld);
	D3DXMatrixIdentity(&g_mView);
	D3DXMatrixIdentity(&g_mProj);

	// create plane and set the position
	if (false == g_legoPlane.create(Device, -1, -1, 9, 0.03f, 6, d3d::GREEN)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);

	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	if (false == g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);
	if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
	g_legowall[2].setPosition(4.56f, 0.12f, 0.0f);
	if (false == g_legowall[3].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
	g_legowall[3].setPosition(-4.56f, 0.12f, 0.0f);

	// create 16 balls and set the position
	for (i = 0; i < 22; i++) {
		if (false == g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS, spherePos[i][1]);
		g_sphere[i].setPower(0, 0);
	}

	g_player[0].setMyTurn(true);
	g_player[1].setMyTurn(false);

	g_player[0].setMyBallType('N');
	g_player[1].setMyBallType('N');

	g_player[0].setLeftBall(7);
	g_player[1].setLeftBall(7);

	g_player[0].setresult(false);
	g_player[1].setresult(false);

	D3DXCreateFont(Device, 25, 10, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "굴림체", &Font);
	D3DXCreateSprite(Device, &Sprite);

	// create blue ball for set direction
	if (false == g_target_blueball.create(Device, d3d::BLUE)) return false;
	g_target_blueball.setCenter(.0f, (float)M_RADIUS, .0f);

	// light setting 
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));
	lit.Type = D3DLIGHT_POINT;
	lit.Diffuse = d3d::WHITE;
	lit.Specular = d3d::WHITE * 0.9f;
	lit.Ambient = d3d::WHITE * 0.9f;
	lit.Position = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	lit.Range = 100.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.9f;
	lit.Attenuation2 = 0.0f;
	if (false == g_light.create(Device, lit))
		return false;

	// Position and aim the camera.
	D3DXVECTOR3 pos(0.0f, 5.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);

	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
		(float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
	g_legoPlane.destroy();
	for (int i = 0; i < 4; i++) {
		g_legowall[i].destroy();
	}
	destroyAllLegoBlock();
	g_light.destroy();
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i = 0;
	int j = 0;
	int stopBall = 0;
	int putCount = 0;
	int notMyBall;
	int MyBall;
	int buffer = 7;
	int nowPlayer;

	if (Device)
	{
		for (i = 0; i < 2; i++)
		{
			if (g_player[i].getMyTurn() == true)
			{
				nowPlayer = i;
				if (g_player[i].getMyBallType() == 'C')
				{
					notMyBall = 8;
					buffer = 7;
				}
				else if (g_player[i].getMyBallType() == 'S')
				{
					notMyBall = 0;
					buffer = 7;
				}
				else
				{
					notMyBall = 24;
					buffer = 15;
				}
			}
		}

		//notMyBall = 0~6 -> MyBall = 8~14 / notMyBall = 8~14 -> MyBall = 0~6
		MyBall = (notMyBall + 8) % 16;

		// update the position of each ball. during update, check whether each ball hit by walls.
		for (i = 0; i < 16; i++) {
			g_sphere[i].ballUpdate(timeDelta);
			for (j = 0; j < 16; j++) { g_legowall[i].hitBy(g_sphere[j]); }
		}

		// check whether any two balls hit together and update the direction of balls
		for (i = 0; i < 15; i++)
		{
			if (g_sphere[15].hasIntersected(g_sphere[i]) && Block == true)
			{
				first = i;
				Block = false;
				break;
			}
		}

		//아무것도 안 친 경우
		if (i == 15 && g_sphere[15].getVelocity_X() != 0 && g_sphere[15].getVelocity_Z() != 0 && Block == true)
		{
			changeTurn = true;
			FREEBALL = true;
			//::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		}
		//자기 공을 먼저 친 경우 턴이 안 바뀜
		if (first >= MyBall && first < (MyBall + buffer) && first != 7)
		{
			changeTurn = false;
			FREEBALL = false;
		}
		//남의 공을 먼저 친 경우 턴이 바뀜
		if ((first >= notMyBall && first < (notMyBall + buffer)) || first == 7)
		{
			changeTurn = true;
			FREEBALL = true;
		}

		for (i = 0; i < 16; i++) {
			for (j = 0; j < 16; j++) {
				if (i >= j) { continue; }
				g_sphere[i].hitBy(g_sphere[j]);
			}
		}

		for (i = 16; i < 22; i++)
		{
			//자기 공이 들어가는 경우
			for (j = MyBall; j < MyBall + buffer && j < 15; j++)
			{
				if (g_sphere[i].hitByHole(g_sphere[j]))
				{
					if (buffer == 15 && j < 7)
					{
						g_player[nowPlayer].setMyBallType('C');
						g_player[(nowPlayer + 1) % 2].setMyBallType('S');
						notMyBall = 8;
						MyBall = 0;
						buffer = 7;
					}
					else if (buffer == 15 && j < 15)
					{
						g_player[nowPlayer].setMyBallType('S');
						g_player[(nowPlayer + 1) % 2].setMyBallType('C');
						notMyBall = 0;
						MyBall = 8;
						buffer = 7;
					}
					g_player[nowPlayer].PutInMyBall();
					changeTurn = false;
					FREEBALL = false;
					putCount++;
				}
			}

			// 남의 공의 넣었을때, 턴 바꾸고 파울
			for (j = notMyBall; j < notMyBall + buffer && j < 15; j++)
			{
				if (g_sphere[i].hitByHole(g_sphere[j]))
				{
					g_player[(nowPlayer + 1) % 2].PutInMyBall();
					changeTurn = true;
					FREEBALL = true;
				}
			}

			//흰공이 들어갔을때, 턴을 바꾸고 프리볼
			if (g_sphere[i].hitByHole(g_sphere[15]))
			{
				changeTurn = true;
				FREEBALL = true;
			}
			//검은공이 들어갔을때
			if (g_sphere[i].hitByHole(g_sphere[7]))
			{
				//자기 공을 모두 넣었을때, 검은 공을 마지막으로 넣으면 승리
				if (g_player[nowPlayer].getLeftBall() == 0)
				{
					g_player[nowPlayer].setresult(true); //현재 플레이어 승리
					g_player[(nowPlayer + 1) % 2].setresult(false); //다른 플레이어 패배
					::MessageBox(0, "You Win!", 0, 0);
				}
				//자기 공이 남았는데, 검은 공을 넣으면 패배
				else
				{
					g_player[nowPlayer].setresult(false); //현재 플레이어 승리
					g_player[(nowPlayer + 1) % 2].setresult(true); //다른 플레이어 패배
					::MessageBox(0, "You Lose...", 0, 0);
				}
			}
		}

		if (putCount == 0 && first <= MyBall && first > (MyBall + buffer))
			changeTurn = true;

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld);
		for (i = 0; i<4; i++)
			g_legowall[i].draw(Device, g_mWorld);
		for (i = 0; i<22; i++)
			g_sphere[i].draw(Device, g_mWorld);

		g_target_blueball.draw(Device, g_mWorld);
		g_light.draw(Device);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);

		for (i = 0; i < 16; i++)
		{
			if ((g_sphere[i].getVelocity_X() == 0.0) && (g_sphere[i].getVelocity_Z() == 0.0))
				stopBall++;
		}
		if (stopBall == 16)
			STOP = true;
		else
			STOP = false;
	}

	if (Sprite)
		Sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	RECT rt = { 100,100,0,0 };
	Font->DrawText(Sprite, "Player 1", -1, &rt, DT_NOCLIP, D3DCOLOR_XRGB(100, 0, 0));

	rt.top = 130;

	Font->DrawText(Sprite, "Player 2", -1, &rt, DT_NOCLIP, D3DCOLOR_XRGB(100, 0, 0));
	Sprite->End();

	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
	Device->BeginScene();
	return true;
}



LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
	static int old_x = 0;
	static int old_y = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;
	int i;

	switch (msg) {
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_ESCAPE:
			::DestroyWindow(hwnd);
			break;
		case VK_RETURN:
			if (NULL != Device) {
				wire = !wire;
				Device->SetRenderState(D3DRS_FILLMODE,
					(wire ? D3DFILL_WIREFRAME : D3DFILL_SOLID));
			}
			break;
		case VK_SPACE:
			if (STOP == true || STOP == false)
			{
				if (g_sphere[15].getFoul() || FREEBALL == true) {
					D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
					g_sphere[15].setCenter(targetpos.x, targetpos.y, targetpos.z);
					g_sphere[15].setPower(0.0f, 0.0f);
					g_sphere[15].setFoul();
				}
				else {
					D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
					D3DXVECTOR3   whitepos = g_sphere[15].getCenter();
					double theta = acos(sqrt(pow(targetpos.x - whitepos.x, 2)) / sqrt(pow(targetpos.x - whitepos.x, 2) +
						pow(targetpos.z - whitepos.z, 2)));      // 기본 1 사분면
					if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x >= 0) { theta = -theta; }   //4 사분면
					if (targetpos.z - whitepos.z >= 0 && targetpos.x - whitepos.x <= 0) { theta = PI - theta; } //2 사분면
					if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x <= 0) { theta = PI + theta; } // 3 사분면
					double distance = sqrt(pow(targetpos.x - whitepos.x, 2) + pow(targetpos.z - whitepos.z, 2));
					g_sphere[15].setPower(distance * cos(theta), distance * sin(theta));

				}
				STOP = false;
				Block = true;
				FREEBALL = false;
				first = 100;


				//change turn
				if (changeTurn)
				{
					bool opposite = g_player[0].getMyTurn();
					g_player[0].setMyTurn(g_player[1].getMyTurn());
					g_player[1].setMyTurn(opposite);
				}
				changeTurn = false;

			}

		}
		break;
	}

	case WM_MOUSEMOVE:
	{
		int new_x = LOWORD(lParam);
		int new_y = HIWORD(lParam);
		float dx;
		float dy;

		if (LOWORD(wParam) & MK_LBUTTON) {

			if (isReset) {
				isReset = false;
			}
			else {
				D3DXVECTOR3 vDist;
				D3DXVECTOR3 vTrans;
				D3DXMATRIX mTrans;
				D3DXMATRIX mX;
				D3DXMATRIX mY;

				switch (move) {
				case WORLD_MOVE:
					dx = (old_x - new_x) * 0.01f;
					dy = (old_y - new_y) * 0.01f;
					D3DXMatrixRotationY(&mX, dx);
					D3DXMatrixRotationX(&mY, dy);
					g_mWorld = g_mWorld * mX * mY;

					break;
				}
			}

			old_x = new_x;
			old_y = new_y;

		}
		else {
			isReset = true;

			if (LOWORD(wParam) & MK_RBUTTON) {
				dx = (old_x - new_x);// * 0.01f;
				dy = (old_y - new_y);// * 0.01f;

				D3DXVECTOR3 coord3d = g_target_blueball.getCenter();
				g_target_blueball.setCenter(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dy*0.007f);
			}
			old_x = new_x;
			old_y = new_y;

			move = WORLD_MOVE;
		}
		break;
	}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	srand(static_cast<unsigned int>(time(NULL)));

	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}