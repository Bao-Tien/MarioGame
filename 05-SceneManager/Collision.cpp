#include "Collision.h"
#include "GameObject.h"
#include "Mario.h"

#include "debug.h"
#include "TailMario.h"
#include "BrickGolden.h"
#include "Enemy.h"
#include "FireFlower.h"

#define BLOCK_PUSH_FACTOR 0.4f

CCollision* CCollision::__instance = NULL;

CCollision* CCollision::GetInstance()
{
	if (__instance == NULL) __instance = new CCollision();
	return __instance;
}

/*
	SweptAABB 
*/
void CCollision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -9999999.0f;
		tx_exit = 99999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999.0f;
		ty_exit = 99999999999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}


// Returns true if two rectangles (l1, r1) and (l2, r2)
// overlap https://www.geeksforgeeks.org/find-two-rectangles-overlap/ 
bool doOverlap(D3DXVECTOR2 l1, D3DXVECTOR2 r1, D3DXVECTOR2 l2, D3DXVECTOR2 r2)
{
	// if rectangle has area 0, no overlap
	if (l1.x == r1.x || l1.y == r1.y || r2.x == l2.x || l2.y == r2.y)
		return false;

	// If one rectangle is on left side of other
	if (l1.x > r2.x || l2.x > r1.x)
		return false;

	// If one rectangle is above other
	if (r1.y < l2.y || r2.y < l1.y)
		return false; 

	return true;
}
/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	float mvx, mvy;
	objSrc->GetSpeed(mvx, mvy);
	float mdx = mvx * dt;
	float mdy = mvy * dt;

	float svx, svy;
	objDest->GetSpeed(svx, svy);
	float sdx = svx * dt;
	float sdy = svy * dt;

	//
	// NOTE: new m speed = original m speed - collide object speed
	// 
	float dx = mdx - sdx;
	float dy = mdy - sdy;

	objSrc->GetBoundingBox(ml, mt, mr, mb);
	objDest->GetBoundingBox(sl, st, sr, sb);
	
	if (dynamic_cast<CFireFlower*>(objDest) && dynamic_cast<CMario*>(objSrc)) {
		int a = 0;
	}

	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);
	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, dx, dy, objDest, objSrc);
	if (doOverlap(D3DXVECTOR2(ml, mt), D3DXVECTOR2(mr, mb), D3DXVECTOR2(sl, st), D3DXVECTOR2(sr, sb))) {// 2 hcn tu dau ko overlap nhau thi ms sweptAABB
		CCollisionEvent* e1 = new CCollisionEvent(0, 0, 0, 0, 0, objDest, objSrc);
		objSrc->OnOverlapWith(e1);
		CCollisionEvent* e2 = new CCollisionEvent(0, 0, 0, 0, 0, objSrc, objDest);
		objDest->OnOverlapWith(e2);
	}

	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < objDests->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDests->at(i));

		if (e->WasCollided()==1)
			coEvents.push_back(e);
		else
			delete e;
	}

	

	//std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CCollision::Filter( LPGAMEOBJECT objSrc,
	vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT &colX,
	LPCOLLISIONEVENT &colY,
	int filterBlock = 1,		// 1 = only filter block collisions, 0 = filter all collisions 
	int filterX = 1,			// 1 = process events on X-axis, 0 = skip events on X 
	int filterY = 1)			// 1 = process events on Y-axis, 0 = skip events on Y
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		if (c->isDeleted) continue;
		if (c->obj->IsDeleted()) continue; 

		// ignore collision event with object having IsBlocking = 0 (like coin, mushroom, etc)
		/*if (filterBlock == 1 && c->obj->IsBlocking() == 0) 
		{
			continue;
		}*/

		if (c->obj->IsBlockingLeft() == 0 && c->nx < 0) {
			continue;
		}

		if (c->obj->IsBlockingTop() == 0 && c->ny < 0) {
			continue;
		}

		if (c->obj->IsBlockingRight() == 0 && c->nx > 0) {
			continue;
		}

		if (c->obj->IsBlockingBottom() == 0 && c->ny > 0) {
			continue;
		}

		if (c->t < min_tx && c->nx != 0 && filterX == 1) {
			min_tx = c->t; min_ix = i;
		}
		
		if (c->t < min_ty && c->ny != 0 && filterY == 1) {
			min_ty = c->t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

void CCollision::CallCollisionEventOnDestObject(LPGAMEOBJECT objSrc, LPCOLLISIONEVENT colEvent) {
	// OnCollsionWith nguoc lai
	LPCOLLISIONEVENT _colEvent = new CCollisionEvent(colEvent->t, (colEvent->nx) * -1, (colEvent->ny) * -1, colEvent->dx, colEvent->dy,
		objSrc, colEvent->obj);
	colEvent->obj->OnCollisionWith(_colEvent);
}

/*
*  Simple/Sample collision framework 
*  NOTE: Student might need to improve this based on game logic 
*/
void CCollision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL; 
	LPCOLLISIONEVENT colY = NULL;

	coEvents.clear();

	if (objSrc->IsCollidable())
	{
		Scan(objSrc, dt, coObjects, coEvents);
	}

	// No collision detected
	if (coEvents.size() == 0)
	{
		D3DXVECTOR2 currentPosition = objSrc->GetPosition();
		D3DXVECTOR2 currentSpeed = objSrc->GetSpeed();
		objSrc->SetPosition(currentPosition.x + currentSpeed.x * dt, currentPosition.y + currentSpeed.y * dt);
		objSrc->OnNoCollision(dt);
	}
	else
	{
		Filter(objSrc, coEvents, colX, colY);
		
		float x, y, vx, vy, dx, dy;
		objSrc->GetPosition(x, y);
		objSrc->GetSpeed(vx, vy);
		dx = vx * dt;
		dy = vy * dt;

		if (colX != NULL && colY != NULL) 
		{
			
			if (colY->t < colX->t)	// was collision on Y first ?
			{
				if (colY->obj->IsBlocking()) {
					y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
				}
				else {
					y += dy;
				}
				
				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colY);
				//call OnCollisionWith nguoc lai
				CallCollisionEventOnDestObject(objSrc, colY);
				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//
				colX->isDeleted = true;		// remove current collision event on X

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABB(objSrc, dt, colX->obj));

				// re-filter on X only
				Filter(objSrc, coEvents, colX_other, colY, /*filterBlock = */ 1, 1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					if (colX_other->obj->IsBlocking()) {
						x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
					}
					else {
						x += dx;
					}
					
					objSrc->OnCollisionWith(colX_other);
					//call OnCollisionWith nguoc lai
					CallCollisionEventOnDestObject(objSrc, colX_other);
				}
				else
				{
					x += dx;
				}
			}
			else // collision on X first
			{
				if (colX->obj->IsBlocking()) {
					x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				}
				else {
					x += dx;
				}
				
				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colX);
				//call OnCollisionWith nguoc lai
				CallCollisionEventOnDestObject(objSrc, colX);

				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				LPCOLLISIONEVENT colY_other = NULL;

				//
				// check again if there is true collision on Y
				//
				colY->isDeleted = true;		// remove current collision event on Y

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABB(objSrc, dt, colY->obj));

				// re-filter on Y only
				Filter(objSrc, coEvents, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					if (colY_other->obj->IsBlocking()) {
						y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
					}
					else {
						y += dy;
					}
					
					objSrc->OnCollisionWith(colY_other);
					//call OnCollisionWith nguoc lai
					CallCollisionEventOnDestObject(objSrc, colY_other);
				}
				else
				{
					y += dy;
				}
			}
		}
		else {
			if (colX != NULL)
			{
				if (colX->obj->IsBlocking()) {
					x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				}
				else {
					x += dx;
				}
				
				y += dy;
				objSrc->OnCollisionWith(colX);
				//call OnCollisionWith nguoc lai
				CallCollisionEventOnDestObject(objSrc, colX);
			}
			else {
				if (colY != NULL)
				{
					x += dx;
					if (colY->obj->IsBlocking()) {
						y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
					}
					else {
						y += dy;
					}
					
					objSrc->OnCollisionWith(colY);
					//call OnCollisionWith nguoc lai
					CallCollisionEventOnDestObject(objSrc, colY);
				}
				else // both colX & colY are NULL 
				{
					x += dx;
					y += dy;
				}
			}
		}

		objSrc->SetPosition(x, y);
	}

	// Mario (y=50, vy=5, dt=6) -> y=64
	// Nen (y=65)
	// Dan (y=55->y=75)
	// Dan3 (y=85)

	//
	// Scan all non-blocking collisions for further collision logic
	//
	
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		
		LPCOLLISIONEVENT e = coEvents[i];
		if (e->isDeleted)
		{
			continue;
		}
		if (e->obj->IsBlocking())
		{
			continue;  // blocking collisions were handled already, skip them
		}

		//dk bo qua
		bool condition1 = colX && colX->nx < 0 && coEvents[i]->nx < 0;
		bool condition2 = colX && colX->nx > 0 && coEvents[i]->nx > 0;
		bool condition3 = colY && colY->ny < 0 && coEvents[i]->ny < 0;
		bool condition4 = colY && colY->ny > 0 && coEvents[i]->ny > 0;
		
		if (!(condition1 || condition2 || condition3 || condition4)) {
			objSrc->OnCollisionWith(e);
			//call OnCollisionWith nguoc lai
			CallCollisionEventOnDestObject(objSrc, e);
		}
	}


	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
