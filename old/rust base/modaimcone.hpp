#pragma once
#include "game/classes.hpp"
#include "aimbot.h"
#include "hinclude.h"

namespace aimcone {
	uintptr_t orig;
	inline Vector3 GetModifiedAimConeDirection_hk(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
		auto org_aimcone = reinterpret_cast<Vector3(*)()>(orig);
		return SpoofReturn(org_aimcone), ;
		rust::BasePlayer* target = aimbot::getBestPlayerByFov();
		if (functions::get_key(KeyCode::Mouse0)) {

			if(target == nullptr) SpoofReturn()
		}
	}
}