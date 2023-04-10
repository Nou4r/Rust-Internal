#pragma once
#include "hinclude.h"
#include "aimbot.h"

namespace baseplayer
{
	uintptr_t client_input_address;
	inline void client_input_hook(rust::BasePlayer* player, uintptr_t state)
	{
		variables::screen_width = functions::get_screen_width();
		variables::screen_height = functions::get_screen_height();
		variables::camera_list = functions::scan_for_camera_list();
		variables::client_entities = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
		
		functions::ddraw_get();
	

		auto orig_baseplayer_client_input = reinterpret_cast<void(*)(rust::BasePlayer*, uintptr_t)>(client_input_address);
		SpoofReturn(orig_baseplayer_client_input, player, state);
	}
}



namespace BaseAttack
{
	uintptr_t CanAttackA;
	inline void CanAttack(rust::BaseProjectile* player, uintptr_t CanAttack)
	{
		variables::client_entities = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);

		auto orig_baseplayer_client_input = reinterpret_cast<void(*)(rust::BaseProjectile*, uintptr_t)>(CanAttackA);
		SpoofReturn(orig_baseplayer_client_input, player, CanAttack);
	}


}
