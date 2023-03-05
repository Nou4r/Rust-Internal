#pragma once
#include "game/classes.hpp"
#include "hinclude.h"

namespace cumhook {
	uintptr_t BodyLeanOffset;
	inline Vector3 BodyLeanOffset_hk() {
		/*try {
			AddVectoredExceptionHandler(0, ExHandler);
			if (!LocalPlayer::Entity()->HasPlayerFlag(PlayerFlags::Connected)) return a1->BodyLeanOffset();
			if (plusminus::ui::get_bool(xorstr_("manipulator")) && get_key(plusminus::ui::get_keybind(xorstr_("manipulator bind")))) {
				if (target_ply != nullptr) {

					if (variables::manipulator::m_manipulate.empty() || !LocalPlayer::Entity()->GetHeldEntity())
						return a1->BodyLeanOffset();

					return variables::manipulator::m_manipulate;
				}
			}
		}
		catch (...) { return a1->BodyLeanOffset(); };*/
		std::cout << "called\n";
		auto org_bodyleanoffset = reinterpret_cast<Vector3(*)()>(BodyLeanOffset);
		return SpoofReturn(org_bodyleanoffset);
		//return a1->BodyLeanOffset();
		//return Vector3(0, 0, 0);
	}
}