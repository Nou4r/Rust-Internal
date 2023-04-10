#pragma once
#include "includes.hpp"
#include "game/classes.hpp"

namespace aimbot {
	rust::BasePlayer* getBestPlayerByFov();
	rust::BasePlayer* hotbar();
	void draw();


	int getClosestBoneToCrosshair(rust::BasePlayer* player, float height);
		void run(bool forceHead, float height);
}

