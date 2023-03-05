#pragma once
#include "game/classes.hpp"
#include "hinclude.h"

namespace eyeposhk {
	uintptr_t EyePositionForPlayer;

	inline Vector3 EyePositionForPlayer_hk(rust::BasePlayer* player, Quaternion lookRot) noexcept {
		/*if (!mount || !player) return Vector3::Zero();
		try {
			AddVectoredExceptionHandler(0, ExHandler);
			if (!LocalPlayer::Entity()->HasPlayerFlag(PlayerFlags::Connected)) return mount->EyePositionForPlayer(player, lookRot);
			if (player->userID() == LocalPlayer::Entity()->userID()) {
				if (plusminus::ui::get_bool(xorstr_("manipulator")) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind")))) {
					return mount->EyePositionForPlayer(player, lookRot) + variables::manipulator::m_manipulate;
				}
			}
		}
		catch (...) { return mount->EyePositionForPlayer(player, lookRot); };
		return mount->EyePositionForPlayer(player, lookRot);*/
		std::cout << "called eyepos\n";

		auto org_EyePositionForPlayer = reinterpret_cast<Vector3(*)(rust::BasePlayer*, Quaternion)>(EyePositionForPlayer);
		return SpoofReturn(org_EyePositionForPlayer, player, lookRot);
	}
}