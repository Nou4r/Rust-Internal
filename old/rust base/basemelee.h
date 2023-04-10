#pragma once
#include "hinclude.h"

template <class t = uintptr_t>
inline t get_closest_object_of_class(std::string class_to_find, int max_dist)
{
	int best_distance = max_dist;
	uintptr_t best_object = NULL;
	auto c_ents = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);

	auto entity_realm = memory::read(c_ents + 0x10);
	if (!entity_realm)
	{
		return NULL;
	}

	auto buffer_list = memory::read(entity_realm + 0x28);
	if (!buffer_list)
	{
		return NULL;
	}

	uintptr_t object_list = memory::read(buffer_list + 0x18);
	if (!object_list)
	{
		return NULL;
	}

	auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);
	if (!object_list_size)
	{
		return NULL;
	}

	try
	{
		for (auto i = 0; i < object_list_size; i++)
		{
			////////std::cout << "current_object" << std::endl;
			auto current_object = *reinterpret_cast<rust::BaseEntity**>(object_list + (0x20 + (i * 8)));

			if (!current_object)
			{
				continue;
			}

			std::string class_name = current_object->get_class_name(); //hook_get_class_name(current_object);

			if (class_name == class_to_find)
			{
				////////std::cout << "Found: " << class_name << std::endl;
				auto unk1 = memory::read((uintptr_t)current_object + 0x10);
				if (!unk1)
				{
					continue;
				}

				auto unk2 = memory::read(unk1 + 0x30);
				if (!unk2)
				{
					continue;
				}

				auto unk3 = memory::read(unk2 + 0x30);
				if (!unk3)
				{
					continue;
				}

				if (!current_object)
				{
					continue;
				}

				auto player_visual = memory::read(unk3 + 0x8);

				if (!player_visual)
				{
					continue;
				}

				auto visual_state = memory::read(player_visual + 0x38);

				if (!visual_state)
				{
					continue;
				}

				Vector3 position = *reinterpret_cast<Vector3*>(visual_state + 0x90);

				auto distance = Math::Calc3D_Dist(variables::local_position, position);
				////////std::cout << "Distance: " << std::dec << (int)distance << std::endl;

				if ((int)distance < best_distance)
				{
					best_distance = distance;
					best_object = reinterpret_cast<uintptr_t>(current_object);
					
				}
			}
		}
	}

	catch (...)
	{
	}

	return reinterpret_cast<t>(best_object);
}


namespace basemelee
{
	uintptr_t process_attack_address;

	inline void process_attack_hook(rust::BaseEntity* melee, rust::HitTest* hittest)
	{
		auto orig_basemelee_process_attack = reinterpret_cast<void(*)(rust::BaseEntity*, rust::HitTest*)>(process_attack_address);

		//if () //change to a setting or smthin that the user can toggle
		{
			//Vector3 pos = hittest->get_hit_entity()->get_transform()->get_position();
			//functions::label(Vector4(pos.x, pos.y, 255, 50), "X", Vector4(1, 0, 0, 1));

			
			auto hit_entity = hittest->get_hit_entity();
			if (!hit_entity)
				return orig_basemelee_process_attack(melee, hittest);

			auto hit_entity_class = hit_entity->get_class_name();

			
		
			if (hit_entity_class == xorstr_("OreResourceEntity"))
			{

				rust::BaseEntity* closest_object = get_closest_object_of_class<rust::BaseEntity*>(xorstr_("OreHotSpot"), 10);

				

				if (!closest_object)
				{
					return orig_basemelee_process_attack(melee, hittest);
				}

				rust::Transform* hot_spot_transform = closest_object->get_transform();
				if (hot_spot_transform)
				{
					hittest->set_hit_entity(hit_entity);
					hittest->set_hit_transform(hot_spot_transform);
					auto hotspot_position = hot_spot_transform->get_position();
					auto closest_object_transform = closest_object->get_transform();
					auto inverse_point = closest_object_transform->inverse_tranform_point(hotspot_position);
					hittest->set_hit_hitpoint(inverse_point);
				}
			}
			else if (hit_entity_class == xorstr_("TreeEntity"))
			{
				auto hit_entity_transform = hit_entity->get_transform();
				auto treePos = hit_entity_transform->get_position();
				auto inverse_point = hit_entity_transform->inverse_tranform_point(treePos + Vector3(0.f, 1.f, 0.f));
				auto local = variables::local_player->get_closest_point(treePos);
				auto dist = Math::Calc3D_Dist(local, treePos);
				rust::BaseEntity* closest_object = get_closest_object_of_class<rust::BaseEntity*>(xorstr_("TreeMarker"), 10);

				if (!closest_object)
				{
					hittest->set_hit_distance(dist);
					hittest->set_hit_hitpoint(inverse_point);
					return orig_basemelee_process_attack(melee, hittest);
				}
				else
				{
					auto pos3 = closest_object->get_transform()->get_position();
					auto pos3_inverse_point = hit_entity_transform->inverse_tranform_point(pos3);
					hittest->set_hit_distance(dist);
					hittest->set_hit_hitpoint(pos3_inverse_point);
					hit_entity->set_hit_direction(Vector3(treePos.x, 0, treePos.z) - Vector3(pos3.x, 0, pos3.z));
					return orig_basemelee_process_attack(melee, hittest);
				}
			}
		}
		return orig_basemelee_process_attack(melee, hittest);



	}
}

