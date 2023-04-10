#include "../rust base/hinclude.h"
#include <string>
#include <string.h>
#include "basemelee.h"
struct rust_str
{
	char zpad[128];
};
namespace itemesp
{

	void animals(float Distance)
	{
		rust::BasePlayer* a1 = variables::local_player;
		if (!(a1))
			return;

		auto c_ents = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
		auto entity_realm = memory::read(c_ents + 0x10);
		if (!entity_realm)
		{
			return;
		}

		auto buffer_list = memory::read(entity_realm + 0x28);
		if (!buffer_list)
		{
			return;
		}

		uintptr_t object_list = memory::read(buffer_list + 0x18);
		if (!object_list)
		{
			return;
		}

		auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);
		if (!object_list_size)
		{
			return;
		}

		for (auto i = 0; i < object_list_size; i++)
		{
			////////std::cout << "current_object" << std::endl;
			auto current_object = *reinterpret_cast<rust::BaseEntity**>(object_list + (0x20 + (i * 8)));

			if (!current_object)
			{
				continue;
			}

			std::string class_name = current_object->get_class_name(); //hook_get_class_name(current_object);


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
			Vector2 pos;
			rust_str name = *reinterpret_cast<rust_str*>(current_object + 0x60);
			if (distance > Distance) continue;

			//std::cout << functions::ws2s(current_object->get_name())	;
			//std::cout << current_object->get_prefab_namehash();
			if (functions::get_key(KeyCode::P))
			{
				std::cout << current_object->get_class_name() << "\n";

			}
			if (current_object->get_class_name() == "Boar")
			{
				std::string raid = "[Boar : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}

			if (current_object->get_class_name() == "Chicken")
			{
				std::string raid = "[Chicken : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
			}

		
		
			if (current_object->get_class_name() == "Stag")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[Deer : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "Wolf")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[Wolf : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
		

		
			if (current_object->get_class_name() == "Bear")
			{
				std::string raid = "Bear : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "Polarbear")
			{
				std::string raid = "[Polar Bear : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			

			if (current_object->get_class_name() == "RidableHorse")
			{
				std::string raid = "[Ridable Horse : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}



		


		}
	}
	void Items( float Distance)
	{
		rust::BasePlayer* a1 = variables::local_player;
		if (!(a1))
			return;

		auto c_ents = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
		auto entity_realm = memory::read(c_ents + 0x10);
		if (!entity_realm)
		{
			return;
		}

		auto buffer_list = memory::read(entity_realm + 0x28);
		if (!buffer_list)
		{
			return;
		}

		uintptr_t object_list = memory::read(buffer_list + 0x18);
		if (!object_list)
		{
			return;
		}

		auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);
		if (!object_list_size)
		{
			return;
		}

		for (auto i = 0; i < object_list_size; i++)
		{
			////////std::cout << "current_object" << std::endl;
			auto current_object = *reinterpret_cast<rust::BaseEntity**>(object_list + (0x20 + (i * 8)));

			if (!current_object)
			{
				continue;
			}

			std::string class_name = current_object->get_class_name(); //hook_get_class_name(current_object);


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
			Vector2 pos;
			rust_str name = *reinterpret_cast<rust_str*>(current_object + 0x60);
			if (distance > Distance) continue;

			//std::cout << functions::ws2s(current_object->get_name())	;
			//std::cout << current_object->get_prefab_namehash();
			if (functions::get_key(KeyCode::P))
			{
				std::cout << current_object->get_class_name() << "\n";

			}
			
			

				if (current_object->get_class_name() == "PlayerCorpse")
				{
					std::cout << functions::ws2s(current_object->get_name());

					std::string raid = "[Player Corpse : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

				}
				
					if (current_object->get_class_name() == "FreeableLootContainer")
					{
						std::cout << functions::ws2s(current_object->get_name());

						std::string raid = "[Freeable Container : " + std::to_string(int(distance)) + "M ]";

						functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
						functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
						functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

					}
					//connect      91.229.23.249:28080
				if (current_object->get_class_name() == "JunkPileWater")
				{
					std::cout << functions::ws2s(current_object->get_name());
					
					std::string raid = "[Junk Pile : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

				}
					if (current_object->get_class_name() == "DroppedItem")
					{
						std::cout << functions::ws2s(current_object->get_name());

						std::string raid = "[Dropped Item : " + std::to_string(int(distance)) + "M ]";

						functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
						functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
						functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

					}
			if (current_object->get_class_name() == "Boar")
			{
				std::string raid = "[Boar : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "LootContainer")
			{
				std::string raid = "[loot Container : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "HackableLockedCrate")
			{
				std::string raid = "[Hackable Create : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
			}
			
			if (current_object->get_class_name() == "Chicken")
			{
				std::string raid = "[Chicken : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
			}
			
				if (current_object->get_class_name() == "GrowableEntity")
				{
					std::string raid = "[Plant : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
				}
				if (current_object->get_class_name() == "BaseOven")
				{
					std::string raid = "[Furnace : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
				}
				if (current_object->get_class_name() == "Drone")
				{
					std::string raid = "[Drone : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
				}
				if (current_object->get_class_name() == "VendingMachine")
				{
					std::string raid = "[Vending Machine : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
				}
				if (current_object->get_class_name() == "BoxStorage")
				{
					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					std::string raid = "[BoxStorage : " + std::to_string(int(distance)) + "M ]";
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

				}
				if (current_object->get_class_name() == "DudTimedExplosive")
				{
					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					std::string raid = "[Raid : Satchel : " + std::to_string(int(distance)) + "M ]";
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

				}
			if (current_object->get_class_name() == "TimedExplosive")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[Raid : C4 : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid , Vector4(0, 0, 0, 1));

			}

			
				if (current_object->get_class_name() == "Stag")
				{
					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					std::string raid = "[Deerw : " + std::to_string(int(distance)) + "M ]";
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

				}
				if (current_object->get_class_name() == "Wolf")
				{
					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					std::string raid = "[Wolf : " + std::to_string(int(distance)) + "M ]";
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

				}
			if (current_object->get_class_name() == "StashContainer")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[Stash : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "Landmine")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[Land Mine : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			
			if (current_object->get_class_name() == "MotorRowboat")
			{
				std::string raid = "[Row Boat : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "Bear")
			{
				std::string raid = "Bear : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "Polarbear")
			{
				std::string raid = "[Polar Bear : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "SupplyDrop")
			{
				std::string raid = "[Supply Drop : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			
			if (current_object->get_class_name() == "NPCAutoTurret")
			{
				std::string raid = "[NPC Auto Turret : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "MobileMapMarker")
			{
				std::string raid = "[Marker : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			
				
				if (current_object->get_class_name() == "Recycler")
				{
					std::string raid = "[Recycler : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
				}
			if (current_object->get_class_name() == "MiniCopter")
			{
				std::string raid = "[Mini Copter : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			
				if (current_object->get_class_name() == "RidableHorse")
				{
					std::string raid = "[Ridable Horse : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

				}
				
			if (current_object->get_class_name() == "SupplySignal")
			{
				std::string raid = "[Supply Signal : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			
				if (current_object->get_class_name() == "SleepingBag")
				{
					std::string raid = "[Sleeping Bag : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

				}
				
					if (current_object->get_class_name() == "FlameTurret")
					{
						std::string raid = "[Flame Turret : " + std::to_string(int(distance)) + "M ]";

						functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
						functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
						functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

					}
					
						if (current_object->get_class_name() == "TeslaCoil")
						{
							std::string raid = "[Tesla Coil : " + std::to_string(int(distance)) + "M ]";

							functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
							functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
							functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

						}
					
				if (current_object->get_class_name() == "AutoTurret")
				{
					std::string raid = "[Auto Turret : " + std::to_string(int(distance)) + "M ]";

					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
					functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

				}
			if (current_object->get_class_name() == "GunTrap")
			{
				std::string raid = "[Shotgun Trap : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
			}



			//std::cout << current_object->get_prefab_name();
			
		
		
			/*
			
			
			if (current_object->get_class_name() == "OreResourceEntity")
			{

				if (current_object->get_prefab_name() == "stone-ore.prefab")
				{
					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), "Stone", Vector4(0, 0, 1, 1));
				}

				if (current_object->get_prefab_name() == "metal-ore.prefab")
				{
					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), "metal", Vector4(0.8, 0.2, 0, 1));
				}

			}



			

				*/
				

			
			/*
			
			if (current_object->get_class_name() == "TreeEntity")
			{
				if (current_object->get_prefab_name() == "TreeEntity")
				{
					functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
					functions::outline(Vector4(pos.x, pos.y, 150, 20), "Tree", Vector4(1, 0, 0, 1));
				}
			}

			if (current_object->get_prefab_name() =="hemp-collectable")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), "Hemp", Vector4(0, 1, 0, 1));
			}
			if (current_object->get_class_name() == "hemp-collectable")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), "Hemp1", Vector4(0, 0, 1, 1));
			}
		//	std::cout << name;
			if (current_object->get_class_name() == "OreResourceEntity") {

				if (current_object->get_prefab_name() == "sulfur-ore")
				{
					if (current_object->get_prefab_name() == "sulfur-ore")
					{
						functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
						functions::outline(Vector4(pos.x, pos.y, 150, 20), "sulf", Vector4(1, 0, 0, 1));
					}

				}

				if (current_object->get_class_name() == "metal-ore")
				{
					if (current_object->get_prefab_name() == "metal-ore")
					{
						functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
						functions::outline(Vector4(pos.x, pos.y, 150, 20), "metal", Vector4(1, 0, 0, 1));
					}

				}

				}
				*/

		

			////////std::cout << "Distance: " << std::dec << (int)distance << std::endl;


		}
	}
	void Raid(float Distance)
	{
		rust::BasePlayer* a1 = variables::local_player;
		if (!(a1))
			return;

		auto c_ents = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
		auto entity_realm = memory::read(c_ents + 0x10);
		if (!entity_realm)
		{
			return;
		}

		auto buffer_list = memory::read(entity_realm + 0x28);
		if (!buffer_list)
		{
			return;
		}

		uintptr_t object_list = memory::read(buffer_list + 0x18);
		if (!object_list)
		{
			return;
		}

		auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);
		if (!object_list_size)
		{
			return;
		}

		for (auto i = 0; i < object_list_size; i++)
		{
			////////std::cout << "current_object" << std::endl;
			auto current_object = *reinterpret_cast<rust::BaseEntity**>(object_list + (0x20 + (i * 8)));

			if (!current_object)
			{
				continue;
			}

			std::string class_name = current_object->get_class_name(); //hook_get_class_name(current_object);


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
			Vector2 pos;
			rust_str name = *reinterpret_cast<rust_str*>(current_object + 0x60);
			if (distance > Distance) continue;

			//std::cout << functions::ws2s(current_object->get_name())	;
			//std::cout << current_object->get_prefab_namehash();
			if (functions::get_key(KeyCode::P))
			{
				std::cout << current_object->get_class_name() << "\n";

			}



			
			if (current_object->get_class_name() == "DudTimedExplosive")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[Raid : Satchel : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "TimedExplosive")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[Raid : C4 : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}


		}
	}

	void Traps(float Distance)
	{
		rust::BasePlayer* a1 = variables::local_player;
		if (!(a1))
			return;

		auto c_ents = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
		auto entity_realm = memory::read(c_ents + 0x10);
		if (!entity_realm)
		{
			return;
		}

		auto buffer_list = memory::read(entity_realm + 0x28);
		if (!buffer_list)
		{
			return;
		}

		uintptr_t object_list = memory::read(buffer_list + 0x18);
		if (!object_list)
		{
			return;
		}

		auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);
		if (!object_list_size)
		{
			return;
		}

		for (auto i = 0; i < object_list_size; i++)
		{
			////////std::cout << "current_object" << std::endl;
			auto current_object = *reinterpret_cast<rust::BaseEntity**>(object_list + (0x20 + (i * 8)));

			if (!current_object)
			{
				continue;
			}

			std::string class_name = current_object->get_class_name(); //hook_get_class_name(current_object);


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
			Vector2 pos;
			rust_str name = *reinterpret_cast<rust_str*>(current_object + 0x60);
			if (distance > Distance) continue;

			//std::cout << functions::ws2s(current_object->get_name())	;
			//std::cout << current_object->get_prefab_namehash();
			if (functions::get_key(KeyCode::P))
			{
				std::cout << current_object->get_class_name() << "\n";

			}



			
			if (current_object->get_class_name() == "Landmine")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[Land Mine : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}

		
			if (current_object->get_class_name() == "NPCAutoTurret")
			{
				std::string raid = "[NPC Auto Turret : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			
			if (current_object->get_class_name() == "FlameTurret")
			{
				std::string raid = "[Flame Turret : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}

			if (current_object->get_class_name() == "TeslaCoil")
			{
				std::string raid = "[Tesla Coil : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}

			if (current_object->get_class_name() == "AutoTurret")
			{
				std::string raid = "[Auto Turret : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			if (current_object->get_class_name() == "GunTrap")
			{
				std::string raid = "[Shotgun Trap : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
			}


		}

	}

	void Storage(float Distance)
	{
		rust::BasePlayer* a1 = variables::local_player;
		if (!(a1))
			return;

		auto c_ents = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
		auto entity_realm = memory::read(c_ents + 0x10);
		if (!entity_realm)
		{
			return;
		}

		auto buffer_list = memory::read(entity_realm + 0x28);
		if (!buffer_list)
		{
			return;
		}

		uintptr_t object_list = memory::read(buffer_list + 0x18);
		if (!object_list)
		{
			return;
		}

		auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);
		if (!object_list_size)
		{
			return;
		}

		for (auto i = 0; i < object_list_size; i++)
		{
			////////std::cout << "current_object" << std::endl;
			auto current_object = *reinterpret_cast<rust::BaseEntity**>(object_list + (0x20 + (i * 8)));

			if (!current_object)
			{
				continue;
			}

			std::string class_name = current_object->get_class_name(); //hook_get_class_name(current_object);


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
			Vector2 pos;
			rust_str name = *reinterpret_cast<rust_str*>(current_object + 0x60);
			if (distance > Distance) continue;

			//std::cout << functions::ws2s(current_object->get_name())	;
			//std::cout << current_object->get_prefab_namehash();
			if (functions::get_key(KeyCode::P))
			{
				std::cout << current_object->get_class_name() << "\n";

			}



			
			
			if (current_object->get_class_name() == "BaseOven")
			{
				std::string raid = "[Furnace : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
			}
			
			if (current_object->get_class_name() == "VendingMachine")
			{
				std::string raid = "[Vending Machine : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));
			}
			if (current_object->get_class_name() == "BoxStorage")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[BoxStorage : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			
			

			
		}
	}

	void Vecicals(float Distance)
	{
		rust::BasePlayer* a1 = variables::local_player;
		if (!(a1))
			return;

		auto c_ents = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
		auto entity_realm = memory::read(c_ents + 0x10);
		if (!entity_realm)
		{
			return;
		}

		auto buffer_list = memory::read(entity_realm + 0x28);
		if (!buffer_list)
		{
			return;
		}

		uintptr_t object_list = memory::read(buffer_list + 0x18);
		if (!object_list)
		{
			return;
		}

		auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);
		if (!object_list_size)
		{
			return;
		}

		for (auto i = 0; i < object_list_size; i++)
		{
			////////std::cout << "current_object" << std::endl;
			auto current_object = *reinterpret_cast<rust::BaseEntity**>(object_list + (0x20 + (i * 8)));

			if (!current_object)
			{
				continue;
			}

			std::string class_name = current_object->get_class_name(); //hook_get_class_name(current_object);


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
			Vector2 pos;
			rust_str name = *reinterpret_cast<rust_str*>(current_object + 0x60);
			if (distance > Distance) continue;

			//std::cout << functions::ws2s(current_object->get_name())	;
			//std::cout << current_object->get_prefab_namehash();
			if (functions::get_key(KeyCode::P))
			{
				std::cout << current_object->get_class_name() << "\n";

			}



			if (current_object->get_class_name() == "MiniCopter")
			{
				std::string raid = "[Mini Copter : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}

			if (current_object->get_class_name() == "RidableHorse")
			{
				std::string raid = "[Ridable Horse : " + std::to_string(int(distance)) + "M ]";

				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}

		
			

		}
	}
	

	void Stash(float Distance)
	{
		rust::BasePlayer* a1 = variables::local_player;
		if (!(a1))
			return;

		auto c_ents = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
		auto entity_realm = memory::read(c_ents + 0x10);
		if (!entity_realm)
		{
			return;
		}

		auto buffer_list = memory::read(entity_realm + 0x28);
		if (!buffer_list)
		{
			return;
		}

		uintptr_t object_list = memory::read(buffer_list + 0x18);
		if (!object_list)
		{
			return;
		}

		auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);
		if (!object_list_size)
		{
			return;
		}

		for (auto i = 0; i < object_list_size; i++)
		{
			////////std::cout << "current_object" << std::endl;
			auto current_object = *reinterpret_cast<rust::BaseEntity**>(object_list + (0x20 + (i * 8)));

			if (!current_object)
			{
				continue;
			}

			std::string class_name = current_object->get_class_name(); //hook_get_class_name(current_object);


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
			Vector2 pos;
			rust_str name = *reinterpret_cast<rust_str*>(current_object + 0x60);
			if (distance > Distance) continue;

			//std::cout << functions::ws2s(current_object->get_name())	;
			//std::cout << current_object->get_prefab_namehash();
			if (functions::get_key(KeyCode::P))
			{
				std::cout << current_object->get_class_name() << "\n";

			}


			if (current_object->get_class_name() == "StashContainer")
			{
				functions::WorldToScreen(current_object->get_transform()->get_position(), pos);
				std::string raid = "[Stash : " + std::to_string(int(distance)) + "M ]";
				functions::outline(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(1, 1, 1, 1));
				functions::label(Vector4(pos.x, pos.y, 150, 20), raid, Vector4(0, 0, 0, 1));

			}
			
		}
	}
}
			
