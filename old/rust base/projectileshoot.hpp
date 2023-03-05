#pragma once
#include "aimbot.h"


inline uintptr_t find_pattern(uintptr_t base, const PBYTE Pattern, const char* Mask) {
	PBYTE Start = (PBYTE)base;
	PIMAGE_NT_HEADERS NTHead = (PIMAGE_NT_HEADERS)(Start + ((PIMAGE_DOS_HEADER)Start)->e_lfanew);
	DWORD Len = NTHead->OptionalHeader.SizeOfImage;

	for (PBYTE region_it = Start; region_it < (Start + Len); ++region_it) {
		if (*region_it == *Pattern) {
			bool found = true;
			const unsigned char* pattern_it = Pattern, * mask_it = (const PBYTE)Mask, * memory_it = region_it;
			for (; *mask_it && (memory_it < (Start + Len)); ++mask_it, ++pattern_it, ++memory_it) {
				if (*mask_it != 'x') continue;
				if (*memory_it != *pattern_it) {
					found = false;
					break;
				}
			}

			if (found)
				return (uintptr_t)region_it;
		}
	}

	return 0;
}

namespace serverrpc
{
	inline uintptr_t serverrpc_projectileshoot;
	extern Vector3 rpc_position;
	static bool init = true;

	inline void hk_serverrpc_projectileshoot(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t projectile_shoot, uintptr_t arg5) {
		while (true)
		{
			if (!variables::local_player)
				break;

			if (!variables::client_entities)
				break;

			auto held_weapon = variables::local_player->get_held_item();
			if (!held_weapon)
				break;

			auto held_entity = held_weapon->get_held_entity();
			if (!held_entity)
				break;

			auto object_unk = *reinterpret_cast<uintptr_t*>(held_entity);
			if (!object_unk)
				break;

			auto classname_ptr = *reinterpret_cast<uintptr_t*>(object_unk + 0x10);
			if (!classname_ptr)
				break;

			auto class_name = functions::read_ascii_alt(classname_ptr);

			uintptr_t created_projectiles = *reinterpret_cast<uintptr_t*>((uintptr_t)(held_entity)+0x398);
			if (!created_projectiles)
				break;

			uintptr_t projectile_list = *reinterpret_cast<uintptr_t*>(created_projectiles + 0x10);
			if (!projectile_list)
				break;

			Vector3 aim_velocity;

			if (true)//options aimbot
			{

				if (!functions::update_view_matrix())
					break;

				auto target = aimbot::getBestPlayerByFov();
				if (!target)
					break;

				auto weapon_data = held_weapon->get_weapon_data();

				uintptr_t projectiles = *reinterpret_cast<uintptr_t*>(projectile_shoot + 0x18);
				if (!projectiles)
					break;

				uintptr_t shoot_list = *reinterpret_cast<uintptr_t*>(projectiles + 0x10);

				int shoot_list_size = *reinterpret_cast<int*>(projectiles + 0x18);

				Vector3 aim_angle;
				Vector3 rpc_position;

				for (int i = 0; i < shoot_list_size; i++)
				{
					auto projectile = *(uintptr_t*)(shoot_list + 0x20 + i * sizeof(uint64_t));
					if (projectile < 0x100000)
						continue;

					rpc_position = *reinterpret_cast<Vector3*>(projectile + 0x18);
					Vector3 original_velocity = *reinterpret_cast<Vector3*>(projectile + 0x24);
					Vector3 target_pos;
					target_pos = target->get_bone_position(eyeTranform);
					auto target_velocity = target->get_model()->get_velocity();
					auto bone = BoneList::eyeTranform;
					float real_initial_velocity = original_velocity.Length();
					float our_intitial_velocity = weapon_data.initial_velocity;
					target_pos = aimbot::sim_prediction(rpc_position, target, (int)bone, original_velocity.Length(), weapon_data.gravity_modifier, weapon_data.drag);
					aim_angle = target_pos - rpc_position;
					aim_velocity = (aim_angle).Normalize() * original_velocity.Length();
					*reinterpret_cast<Vector3*>(projectile + 0x24) = aim_velocity;
				}
			}

			int projectile_list_size = *reinterpret_cast<uint32_t*>(projectile_list + 0x18);

			for (int i = 0; i < projectile_list_size; i++)
			{
				auto projectile = *reinterpret_cast<rust::Projectile**>(projectile_list + (0x20 + (i * sizeof(uint64_t))));
				if (!projectile)
					continue;

				if (true)//option aimbot
				{
					projectile->set_current_velocity(aim_velocity);
				}
			}

			break;
		}

		auto projectile_shoot_func = reinterpret_cast<void(*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t)>(serverrpc_projectileshoot);
		projectile_shoot_func(arg1, arg2, arg3, projectile_shoot, arg5);
	}
	
	inline void init_shoot_projectile() {
		if (!init) return;
		static auto initialize_method = find_pattern(LI_MODULE("GameAssembly.dll").get<uintptr_t>(), (PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8D\x15", "xxxxxxxxxxxx");
		static auto il2cpp_codegen_initialize_method = reinterpret_cast<void (*)(unsigned int)>(initialize_method);

		for (size_t idx{ 0 }; idx < 56229; idx++)
		{
			il2cpp_codegen_initialize_method(idx);
		}
		il2cpp_codegen_initialize_method(*reinterpret_cast<uintptr_t*>(LI_MODULE("GameAssembly.dll").get<uintptr_t>() + 49465088));
		//static auto projectile_shoot_sig = find_pattern(LI_MODULE("GameAssembly.dll").get<uintptr_t>(), (PBYTE)"\x40\x55\x57\x48\x8D\x6C\x24\xB1\x48\x81\xEC\xD8", "xxxxxxxxxxxx");
		static auto init_two = find_pattern(LI_MODULE("GameAssembly.dll").get<uintptr_t>(), (PBYTE)"\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x60\x48\x8B\xD9", "xxxxxxxxxxxxxxxxxx");
		static auto init_func = reinterpret_cast<void (*)(unsigned int)>(init_two);
		init_func(*reinterpret_cast<uintptr_t*>(LI_MODULE("GameAssembly.dll").get<uintptr_t>() + 49465088));

		auto& RPC_ProjectileShoot = *(__int64*)(**(__int64**)(*(__int64*)(LI_MODULE("GameAssembly.dll").get<uintptr_t>() + 49465088) + 0x38) + 8);
		serverrpc_projectileshoot = RPC_ProjectileShoot;
		RPC_ProjectileShoot = (uintptr_t)&hk_serverrpc_projectileshoot;

		init = false;
	}
}
  