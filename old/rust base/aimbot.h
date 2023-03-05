#pragma once
#include "includes.hpp"
#include "game/classes.hpp"

namespace aimbot {
	Vector3 sim_prediction(const Vector3& from, rust::BasePlayer* who, unsigned int bone, float initial_velocity, float gravity_modifier, float drag);
	rust::BasePlayer* getBestPlayerByFov();
	int getClosestBoneToCrosshair(rust::BasePlayer* player);
	void run(bool forceHead);

}

