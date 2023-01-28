#include "MagicObj.h"
#include "Mario.h"

void CMagicObj::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (level == 0) { return; }
	if (e->obj) {
		if (dynamic_cast<CMario*>(e->obj)) {
			level = 0;
		}
	}
}