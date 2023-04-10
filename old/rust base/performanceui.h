#pragma once
#include "hinclude.h"
#include "esp.h"
#include "game/offsets.hpp"
#include "Glob.hpp"
#include "sky.hpp"
#include "Render_ItemEsp.hpp"
#include "Misc.h"
#include "Keybind.hpp"

bool b = false;

float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}
float BOG_TO_GRD(float BOG) {
	return (180 / M_PI) * BOG;
}
namespace options {
	bool chams;
	bool coolbow;
	float xx;
	float yy;
	float zz;
	float height;
	bool box;
	bool name;
	bool radar;
	bool helditemname;
	bool Health;
	bool Distance;
	bool hotbar;
	bool w2p;
	bool item;
	bool test;
	bool fakeleg;
	bool farm;
	bool ammo;
	bool norecoil;
	float PlayerDistance;
	bool Stash;
	float StashDistance;
	bool admin;
	bool animal;
	float animaldistance;
	bool trap;
	float trapdistance;
	bool Storage;
	float StorageDistance;
	bool fakefire;
	bool vehicle;
	float vehicledistance;
	bool raid;
	float raiddistance;
	bool nosway;
	bool instaeoka;
	float itemdistance;
	bool Water;
	bool nice;
	bool nocol;
	bool fovchang;
	float fovamout = 100.0f;
	bool fullauto;
	bool fatbullet;
	bool aimbot;
	bool speed;
	bool aimbotb;
	bool bones;
	bool moonJump;
	bool Combatfov;
	float Combatfovvalue;
	bool Shitaim;
	bool shootinair;
	bool omean;
	bool AllDr;
	float x;
	float y;
	bool spinbot;
	bool shnospreadl;
	bool hotbarsad;
	bool extendedmelee;
	bool instabow;
	bool longeyes;
	bool dysnicfromserver;
	bool instaswap;
	bool instafire;
	bool spiderman;
	bool backtrace; 
	bool dysnicbar;
	bool fakeshot;
	bool night;
	bool day;
	bool groundfucker;
	bool onladder;
	bool noami;
	bool instaRevive;
	bool instareviveTeam;
	bool omniSprint;
	bool alwaysShoot;
	bool cleanday;
	bool manip = false;
	bool flybar;
}
float vr, vg, vb, nr, ng, nb;




void sky_loop()
{
	uintptr_t todsky;

	if (!todsky)
	{
		std::cout << "Not Working";
	}
	auto instantsky = *reinterpret_cast<uintptr_t*>(todsky + offsets::tod_sky_get_instance);

	if (!instantsky)
	{
		std::cout << "Instant not working";
	}
	auto tod_atmosphereparameters = *reinterpret_cast<uintptr_t*>(todsky + offsets::tod_atmosphereparameters);
	if (!tod_atmosphereparameters)
	{
		std::cout << "tod_atmosphereparameters Not Working";
	}
	
	auto tod_dayparameters = *reinterpret_cast<uintptr_t*>(todsky + offsets::tod_dayparameters);
	if (!tod_dayparameters)
	{
		std::cout << "tod_dayparameters Not Working";
	}
	

	auto tod_dayparameters_ambientmultiplier = *reinterpret_cast<uintptr_t*>(tod_dayparameters + offsets::tod_dayparameters_ambientmultiplier);
	if (!tod_dayparameters_ambientmultiplier)
	{
		std::cout << "tod_dayparameters_ambientmultiplier Not Working";
	}
	auto tod_nightparameters = *reinterpret_cast<uintptr_t*>(todsky + offsets::tod_nightparameters);
	if (!tod_nightparameters)
	{
		std::cout << "tod_nightparameters Not Working";
	}
		
	auto tod_nightparameters_ambientmultiplier = *reinterpret_cast<uintptr_t*>(tod_nightparameters + offsets::tod_nightparameters_ambientmultiplier);
	if (!tod_nightparameters_ambientmultiplier)
	{
		std::cout << "tod_nightparameters_ambientmultiplier Not Working";
	}

	
}
void entity_loop()
{
	variables::client_entities = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);

	if (!variables::client_entities)
	{
		rust::LocalPlayer::ConsoleMessage(xorstr_("!client_entities"));
		return;
	}

	auto entity_realm = *reinterpret_cast<uintptr_t*>(variables::client_entities + 0x10);
	if (!entity_realm)
	{
		return;
	}

	auto buffer_list = *reinterpret_cast<uintptr_t*>(entity_realm + 0x28);
	if (!buffer_list)
	{
		return;
	}

	auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);

	auto object_list = *reinterpret_cast<uintptr_t*>(buffer_list + 0x18);
	if (!object_list)
	{
		return;
	}

	std::vector<rust::BasePlayer*> tempList;



	for (int idx = 0; idx < object_list_size; idx++)
	{
		auto current_object = *reinterpret_cast<uintptr_t*>(object_list + (0x20 + (idx * sizeof(uint64_t))));
		if (!current_object)
		{
			continue;
		}

		auto object_unk = *reinterpret_cast<uintptr_t*>(current_object);
		if (!object_unk)
		{
			continue;
		}

		auto classname_ptr = *reinterpret_cast<uintptr_t*>(object_unk + 0x10);
		if (!classname_ptr)
		{
			continue;
		}

		auto base_object = *reinterpret_cast<uintptr_t*>(current_object + 0x10);
		if (!base_object)
		{
			continue;
		}

		auto object = *reinterpret_cast<uintptr_t*>(base_object + 0x30);
		if (!object)
		{
			continue;
		}

		if (idx == 0)
		{
			auto local_player_object_class = *reinterpret_cast<uintptr_t*>(object + 0x30);

			if (!local_player_object_class)
			{
				continue;
			}

			auto local_entity = *reinterpret_cast<uintptr_t*>(local_player_object_class + 0x18);

			if (!local_entity)
			{
				continue;
			}

			variables::local_player = *reinterpret_cast<rust::BasePlayer**>(local_entity + 0x28);

			if (variables::local_player->get_life_state())
			{
				continue;
			}

			variables::local_position = variables::local_player->get_visual_position(local_player_object_class);
			continue;
		}

		functions::update_view_matrix();

		const auto tag = *reinterpret_cast<std::uint16_t*>(object + 0x54);
		if (tag == 6)
		{
			auto object_class = *reinterpret_cast<uintptr_t*>(object + 0x30);
			if (!object_class)
				return;

			auto entity = *reinterpret_cast<uintptr_t*>(object_class + 0x18);
			if (!entity)
				return;

			auto player = *reinterpret_cast<rust::BasePlayer**>(entity + 0x28);
			if (!reinterpret_cast<uintptr_t>(player))
				return;


			tempList.push_back(player);

			
		}
		else
		{
			std::hash<std::string_view> char_hash;
			const char* class_name = reinterpret_cast<const char*>(classname_ptr);
			auto class_crc = char_hash(class_name);

			auto object_name = xorstr_("OreResourceEntity");
			auto object_crc = char_hash(std::string_view(object_name, functions::my_strlen(object_name)));
			

		}
	}
	variables::player_list = tempList;

	return;
}

namespace PerformanceUI
{
	bool left;
	bool right;
	float l, y;
	bool sleeper;
	bool knocked;
	int comboLeftSelected;
	int comboRightSelected;

	float sliderFloatLeft;
	float sliderFloatRight;

	int sliderIntLeft;
	int sliderIntRight;

	std::vector<std::string> Vis = { "Red", "Green", "Blue", "Purple" };
	std::vector<std::string> nonoVis = { "Red", "Green", "Blue", "Purple" };
	std::vector<std::string> BoneSelection = { "Head", "Neck", "Pelvis", "Dick" };
	int Bones;
	int nonvis;
	int vis;
	int Aimbotbone; 

	inline void ongui_hook(void* instance)
	{

		
		
		
		if (Bones == 0)
		{
			Aimbotbone == BoneList::head;
		}
		if (Bones == 1)
		{
			Aimbotbone == BoneList::neck;
		}
		if (Bones == 2)
		{
			Aimbotbone == BoneList::pelvis;
		}
		if (Bones == 3)
		{
			Aimbotbone == BoneList::penis;
		}
		if (vis == 0)
		{
			vr = 200.0f;
			vg = 0.0f;
			vb = 55.0f;
		}

		if (vis == 1)
		{
			vr = 0.0f;
			vg = 230.0f;
			vb = 75.0f;
		}

		if (vis == 2)
		{
			vr = 0.0f;
			vg = 100.0f;
			vb = 200.0f;
		}

		if (vis == 3)
		{
			vr = 100.0f;
			vg = 0.0f;
			vb = 255.0f;
		}


		if (nonvis == 0)
		{
			nr = 255.0f;
			ng = 0.0f;
			ng = 0.0f;
		}

		if (nonvis == 1)
		{
			nr = 0.0f;
			ng = 255.0f;
			nb = 0.0f;
		}

		if (nonvis == 2)
		{
			nr = 0.0f;
			ng = 100.0f;
			nb = 200.0f;
		}

		if (nonvis == 3)
		{
			nr = 100.0f;
			ng = 0.0f;
			nb = 255.0f;
		}
		auto skin = functions::get_draw_skin();
		variables::draw_label = *reinterpret_cast<uintptr_t*>(skin + 0x38);
		functions::set_draw_alignment(variables::draw_label, 0x0);
		functions::set_draw_font_size(variables::draw_label, 40);
		functions::set_draw_color(Vector4(1, 1, 1, 1));

		//esp::render_esp();

		/*
				mid_framework::begin(xorstr_("EAC PRIVATE"));

	
		//menu
		mid_framework::begin(xorstr_("EAC PRIVATE"));
		mid_framework::add_tab("[ Visuals ]", 0, 1);
		mid_framework::add_tab("[ Attack ]", 1, 2);
		mid_framework::add_tab("[ Weapon ]", 2, 3);
		mid_framework::add_tab("[ Movement ]", 3, 4);
		mid_framework::add_tab("[ Exploits ]", 4, 5);
		mid_framework::add_tab("[ View Model ]", 5, 6);

		



		if (mid_framework::selected == 0)
		{
			mid_framework::add_child(1);

			//checkmarks
			mid_framework::add_checkbox("Player Box", &options::box, 1);

			mid_framework::add_checkbox("Player Name", &options::name, 1);
			mid_framework::add_checkbox("Player Health", &options::Health, 1);
			mid_framework::add_checkbox("Player Held Item", &options::helditemname, 1);
			mid_framework::add_checkbox("Player Distance", &options::Distance, 1);
		//	mid_framework::add_checkbox("Player Hot Bar", &options::hotbarsad, 1);
			mid_framework::add_checkbox("Draw Sleepers", &sleeper, 1);
			mid_framework::add_checkbox("Draw Knocked", &knocked, 1);
			//mid_framework::add_checkbox("Back Track", &options::backtrace, 1);
			mid_framework::add_checkbox("Radar", &options::radar, 1);


			mid_framework::add_checkbox("Player Chams", &options::chams, 1);
			//mid_framework::add_label("Vis", 1);
			//mid_framework::add_combo(Vis, &vis, 1);
			//mid_framework::add_label("Non", 1);
			//mid_framework::add_combo(nonoVis, &nonvis, 1);
			mid_framework::add_slider_float("Render Distance", &options::PlayerDistance, 0.0f, 1000.0f, 1);
	

		//	mid_framework::add_slider_float("Render Distance", &sliderFloatLeft, 50.0f, 1000.0f, 1);

			


		}
		else if (mid_framework::selected == 1)
		{
			mid_framework::add_child(1);
			mid_framework::add_checkbox("Toggle Aimbot (Mouse 3)", &options::aimbot, 1);
			//mid_framework::add_checkbox("Manipulation", &options::manip, 1);
			//mid_framework::add_combo(BoneSelection, &Bones, 1);


		}
		else if (mid_framework::selected == 2)
		{
			mid_framework::add_child(1);

			mid_framework::add_label("Weapon Misc", 1);
			mid_framework::add_checkbox("No Recoil", &options::norecoil, 1);
			mid_framework::add_slider_float("%", &options::x, 0.0f, 100.0f, 1);

			mid_framework::add_checkbox("No Sway", &options::nosway, 1);
			mid_framework::add_checkbox("Insta Eoka", &options::instaeoka,1);
			mid_framework::add_checkbox("Full Auto", &options::fullauto, 1);
		//	mid_framework::add_checkbox("No Shotgun Spread", &options::shnospreadl, 1);
		//	mid_framework::add_checkbox("No Spread", &options::fullauto, 1);
		//	mid_framework::add_checkbox("Rapid Fire ", &options::instafire, 1);
		//	mid_framework::add_checkbox("Auto Reload ", &options::fullauto, 1);
		//	mid_framework::add_checkbox("Ammo Bar", &options::fullauto, 1);
		//	mid_framework::add_checkbox("Reload Bar", &options::fullauto, 1);
		//	mid_framework::add_checkbox("Insta Switch", &options::instaswap, 1);
			mid_framework::add_checkbox("Extended Melee", &options::extendedmelee, 1);
		//	mid_framework::add_checkbox("Auto Melee Attack", &options::fullauto, 1);
			mid_framework::add_checkbox("Run 2 Ppl (Key M)", &options::fakeshot, 1);





		}
		else if (mid_framework::selected == 3)
		{
			mid_framework::add_child(1);
			mid_framework::add_label("Movment Misc", 1);
			mid_framework::add_checkbox("Moon Jump", &options::moonJump, 1);
			mid_framework::add_checkbox("Spiderman", &options::spiderman, 1);
			//mid_framework::add_checkbox("Omean", &options::omean, 1);
			//mid_framework::add_checkbox("No Movment Block", &options::moonJump, 1);
			mid_framework::add_checkbox("No Collusion", &options::nocol, 1);
			mid_framework::add_checkbox("Unlimited Jump", &options::moonJump, 1);
			mid_framework::add_checkbox("Shoot In Any State", &options::shootinair, 1);
			//mid_framework::add_checkbox("Unlock Angles", &options::moonJump, 1);
			mid_framework::add_checkbox("Water Walk", &options::Water, 1);


			


		}
		else if (mid_framework::selected == 4)
		{
			mid_framework::add_child(1);

			mid_framework::add_checkbox("Fat Gun", &options::instabow, 1);
			//mid_framework::add_checkbox("God Mode Exploit ", &left, 1);
			mid_framework::add_checkbox("Longer Eyes (Arrow Keys)", &options::longeyes, 1);
		//	mid_framework::add_checkbox("Fly Hack", &left, 1);
			//mid_framework::add_checkbox("Fly Hack Bar", &left, 1);
		//	mid_framework::add_checkbox("Air Stuck Exploit", &left, 1);
		//	mid_framework::add_checkbox("Glide", &left, 1);
			//mid_framework::add_checkbox("Run on air Exploit", &left, 1);
			mid_framework::add_checkbox("Dysnic (Middle Mouse)", &options::dysnicfromserver, 1);
			mid_framework::add_checkbox("Safe Admin", &options::admin, 1);
		//	mid_framework::add_checkbox("Hit Spoof", &options::fullauto, 1);
		//	mid_framework::add_checkbox("Fast Bullets", &options::fullauto, 1);
		//	mid_framework::add_checkbox("Insta Hit", &options::fullauto, 1);
		//	mid_framework::add_checkbox("Fat Bullts", &options::fatbullet, 1);
			mid_framework::add_checkbox("Insta Revive", &options::instaRevive, 1);
			mid_framework::add_checkbox("Farm Assist", &options::farm, 1);

		//	mid_framework::add_label("Player Troll Exploits", 1);
			mid_framework::add_checkbox("Ground Fucker", &options::groundfucker, 1);
			mid_framework::add_checkbox("On Ladder", &options::onladder, 1);
		}
		else if (mid_framework::selected == 5)
		{
			mid_framework::add_child(1);
			mid_framework::add_checkbox("Fov Changer", &options::fovchang, 1);
			mid_framework::add_slider_float("Fov", &options::fovamout, 0.0f, 180.0f, 1);
		//	mid_framework::add_checkbox("Nice Night", &options::night, 1);
		//	mid_framework::add_checkbox("Always Day", &options::day, 1);
		//	mid_framework::add_checkbox("Nice Sky", &options::nice, 1);
		//	mid_framework::add_checkbox("Dysnic Bar", &options::dysnicbar, 1);
			}
		mid_framework::end();
			*/

		mid_framework::begin(xorstr_("EAC PRIVATE"));
		mid_framework::add_tab("[ Visuals ]", 0, 1);
		mid_framework::add_tab("[ Attack ]", 1, 2);
		mid_framework::add_tab("[ Weapon ]", 2, 3);
		mid_framework::add_tab("[ Movement ]", 3, 4);
		mid_framework::add_tab("[ View Model ]", 4, 5);



		if (mid_framework::selected == 0)
		{

			mid_framework::add_child(2);

			mid_framework::add_checkbox("Visual.Box", &options::box, 1);
			mid_framework::add_checkbox("Visual.Health", &options::Health, 1);
			mid_framework::add_checkbox("Visual.Name", &options::name, 1);
			mid_framework::add_checkbox("Visual.Skeleton", &options::bones, 1);
			mid_framework::add_slider_float("X", &options::xx, 0.0f, 1000.0f, 1);		
			mid_framework::add_slider_float("Y", &options::yy, 0.0f, 1000.0f, 1);
			mid_framework::add_slider_float("X", &options::zz, 0.0f, 1000.0f, 1);


			mid_framework::add_checkbox("Visual.HeldItem", &options::helditemname, 1);
			mid_framework::add_checkbox("Visual.Hotbar", &options::hotbarsad, 1);

			mid_framework::add_checkbox("Visual.NoAn", &options::noami, 1);


			mid_framework::add_checkbox("Visual.Player.Distance", &options::Distance, 1);
			mid_framework::add_checkbox("Visual.Player.Chams", &options::chams, 1);
			mid_framework::add_checkbox("Visual.Radar", &options::radar, 1);


			mid_framework::add_checkbox("Visual.Animals", &options::animal, 1); // Not Done
			mid_framework::add_checkbox("Visual.Traps", &options::trap, 1); // Not Done
			mid_framework::add_checkbox("Visual.Stash", &options::Stash, 1); // Not Done
			mid_framework::add_checkbox("Visual.Storage", &options::Storage, 1); // Not Done
			mid_framework::add_checkbox("Visual.Vehicle", &options::vehicle, 1); // Not Done
			mid_framework::add_checkbox("Visual.Raid", &options::raid, 1); // Not Done

			//mid_framework::add_checkbox("Visual.Food", &options::test, 1); // Not Done
		//	mid_framework::add_checkbox("Visual.Loot", &options::test, 1); // Not Done
			//mid_framework::add_checkbox("Render.Teamates", &options::box, 1);
			mid_framework::add_checkbox("Render.Sleepers", &sleeper, 1);
			mid_framework::add_checkbox("Render.Knocked", &knocked, 1);
			mid_framework::add_checkbox("Render.NPC", &options::box, 1);
			mid_framework::add_slider_float("Player Render Distance", &options::PlayerDistance, 0.0f, 500.0f, 2);
			mid_framework::add_slider_float("Animal Distance", &options::animaldistance, 0.0f, 500.0f, 2);
			mid_framework::add_slider_float("Traps Render Distance", &options::trapdistance, 0.0f, 500.0f, 2);
			mid_framework::add_slider_float("Stash Render Distance", &options::StashDistance, 0.0f, 500.0f, 2);
			mid_framework::add_slider_float("Storage Render Distance", &options::StorageDistance, 0.0f, 500.0f, 2);
			mid_framework::add_slider_float("Vechicle Render Distance", &options::vehicledistance, 0.0f, 500.0f, 2);
			mid_framework::add_slider_float("Raid Render Distance", &options::raiddistance, 0.0f, 500.0f, 2);




		}

		else if (mid_framework::selected == 1)
		{
			mid_framework::add_child(1);
			

			mid_framework::add_label("Press Mouse 0, and keybind", 1);
			mid_framework::add_checkbox("Combat.Memory Mouse 3", &options::aimbot, 1); // Not Done
			mid_framework::add_checkbox("Combat.Minip Mouse 3", &options::manip, 1); // Not Done

			//mid_framework::add_checkbox("Combat.Heli.Psilent", &options::instabow, 1);// Not Done
			//mid_framework::add_checkbox("Combat.Bullet.Minipulation", &options::instabow, 1);// Not Done
			//mid_framework::add_checkbox("Combat.Magic.Bullet", &options::test, 1);// Not Done
			//mid_framework::add_checkbox("Combat.Instant.Bullet.Minipulation", &options::test, 1);// Not Done
			//mid_framework::add_checkbox("Combat.Full.Auto.Magic.Bullet", &options::test, 1);// Not Done
			//mid_framework::add_checkbox("Combat.Bust.Magic.Bullet", &options::test, 1);// Not Done
			//mid_framework::add_checkbox("Combat.Pierce.Objects", &options::test, 1);// Not Done
			mid_framework::add_checkbox("Combat.Field.Of.Fiew", &options::Combatfov, 1);// Not Done
			mid_framework::add_slider_float("FOV", &options::Combatfovvalue, 0.0f, 900.0f, 1);

		}
	
		else if (mid_framework::selected == 2)
		{
			mid_framework::add_child(1);

			mid_framework::add_label("Weapon Misc", 1);
			mid_framework::add_checkbox("No Recoil", &options::norecoil, 1);
			mid_framework::add_slider_float("%", &options::x, 0.0f, 100.0f, 1);
			mid_framework::add_checkbox("No Sway", &options::nosway, 1);
			mid_framework::add_checkbox("Insta Eoka", &options::instaeoka, 1);
			mid_framework::add_checkbox("Full Auto", &options::fullauto, 1);
			mid_framework::add_checkbox("Fast Fire", &options::instabow, 1);
			mid_framework::add_checkbox("Fat Bullet", &options::fatbullet, 1);


			//mid_framework::add_checkbox("No Shotgun Spread", &options::shnospreadl, 1);
			//mid_framework::add_checkbox("No Spread", &options::fullauto, 1);
			///mid_framework::add_checkbox("Auto Reload ", &options::fullauto, 1);
			//mid_framework::add_checkbox("Ammo Bar", &options::fullauto, 1);
			//mid_framework::add_checkbox("Reload Bar", &options::fullauto, 1);
			mid_framework::add_checkbox("Insta Switch", &options::instaswap, 1);
			mid_framework::add_checkbox("Extended Melee", &options::extendedmelee, 1);
			//mid_framework::add_checkbox("Auto Melee Attack", &options::extendedmelee, 1);
			mid_framework::add_checkbox("Fake Fire", &options::fakeshot, 1);
		}

		else if (mid_framework::selected == 3)
		{
			mid_framework::add_child(1);


			mid_framework::add_label("Movement Selection", 1);
			mid_framework::add_checkbox("Movement.SpiderMan", &options::spiderman, 1);
			mid_framework::add_checkbox("Movement.No.Collision", &options::nocol, 1);
			mid_framework::add_checkbox("Movement.All.Direction", &options::AllDr, 1);// Not Done
			mid_framework::add_checkbox("Movement.Shoot.in.Any.State", &options::shootinair, 1);// Not Done
			mid_framework::add_checkbox("Movement.Jump.Increase", &options::moonJump, 1);
			mid_framework::add_checkbox("Movement.Walk.On.Water", &options::Water, 1);
			//mid_framework::add_checkbox("Movement.Speed", &options::speed, 1);// Not Done
			//mid_framework::add_checkbox("Movement.Fly.Bypass", &options::box, 1);// Not Done
			//mid_framework::add_checkbox("Movement.Spin.Bot", &options::spinbot, 1);// Not Done
			mid_framework::add_checkbox("Movement.Fake.Lag", &options::fakeleg, 1);// Not Done
			//mid_framework::add_checkbox("Movement.Auto.Farm", &options::box, 1);
			mid_framework::add_checkbox("Movement.Auto.Walk.To.Player (M)", &options::w2p, 1);

		}
		else if (mid_framework::selected == 4)
		{
			mid_framework::add_child(1);



			mid_framework::add_checkbox("Player.Field.Of.View.Changer", &options::fovchang, 1);
			mid_framework::add_slider_float("FOV", &options::fovamout, 0.0f, 180.0f, 1);
			mid_framework::add_checkbox("Player.Instant.Rev", &options::instaRevive, 1);// Not Done
			mid_framework::add_checkbox("Player.Instant.Rev.Teamates", &options::instareviveTeam, 1);// Not Done
			//mid_framework::add_checkbox("Player.Clean.Day", &options::cleanday, 1);// Not Done
		//	mid_framework::add_checkbox("Player.Desync", &options::dysnicfromserver, 1);// Not Done
			mid_framework::add_checkbox("Player.Admin.Access", &options::admin, 1);



			//mid_framework::add_label("Bar Selection", 1);
			//mid_framework::add_checkbox("Bar.Reload", &options::ammo, 1);
			//mid_framework::add_checkbox("Bar.Ammo", &options::test, 1);
			mid_framework::add_checkbox("Bar.Fly.Hack", &options::flybar, 1);
			mid_framework::add_checkbox("Bar.Desync", &options::dysnicbar, 1);
			//mid_framework::add_checkbox("Bar.Ban.Chance", &options::box, 1);



		}

	

		mid_framework::end();



		float screen_centerx = functions::get_screen_width() / 2;
		float screen_centery = functions::get_screen_height() / 2;
		float a = screen_centery / 30.0f;






		

	entity_loop();

	functions::outline(Vector4(3, 10, 150, 20), " CThread.Rust ", Vector4(0, 0, 0, 1));
		functions::label(Vector4(3, 10, 150, 20), " Cthread.Rust  ", Vector4(1, 1, 1, 1));
		int xc = functions::get_screen_width() / 2;
		int yc = functions::get_screen_height() / 2;
		functions::line(Vector2(xc,  yc - 10 ), Vector2( xc, yc + 10 ), Vector4(255, 0, 0, 255));
		functions::line(Vector2( xc - 10,  yc ), Vector2( xc + 10, yc ), Vector4(255, 0, 0, 255));



		KeyCode input;

		if (functions::get_key(KeyCode::DownArrow))
		{
			if (functions::get_key(KeyCode::A))
			{
				input = KeyCode::A;
			}
			if (functions::get_key(KeyCode::S))
			{
				input = KeyCode::S;
			}
			if (functions::get_key(KeyCode::D))
			{
				input = KeyCode::D;
			}
			if (functions::get_key(KeyCode::F))
			{
				input = KeyCode::F;
			}
			if (functions::get_key(KeyCode::G))
			{
				input = KeyCode::G;
			}
			if (functions::get_key(KeyCode::H))
			{
				input = KeyCode::H;
			}
			if (functions::get_key(KeyCode::J))
			{
				input = KeyCode::J;
			}
			if (functions::get_key(KeyCode::K))
			{
				input = KeyCode::K;
			}
			if (functions::get_key(KeyCode::L))
			{
				input = KeyCode::L;
			}
			if (functions::get_key(KeyCode::Z))
			{
				input = KeyCode::Z;
			}
			if (functions::get_key(KeyCode::X))
			{
				input = KeyCode::X;
			}
			if (functions::get_key(KeyCode::C))
			{
				input = KeyCode::C;
			}
			if (functions::get_key(KeyCode::V))
			{
				input = KeyCode::V;
			}
			if (functions::get_key(KeyCode::B))
			{
				input = KeyCode::B;
			}
			if (functions::get_key(KeyCode::N))
			{
				input = KeyCode::N;
			}
			if (functions::get_key(KeyCode::M))
			{
				input = KeyCode::M;
			}

			if (functions::get_key(KeyCode::Q))
			{
				input = KeyCode::Q;
			}
			if (functions::get_key(KeyCode::W))
			{
				input = KeyCode::W;
			}
			if (functions::get_key(KeyCode::E))
			{
				input = KeyCode::E;
			}
			if (functions::get_key(KeyCode::R))
			{
				input = KeyCode::R;
			}
			if (functions::get_key(KeyCode::T))
			{
				input = KeyCode::T;
			}
			if (functions::get_key(KeyCode::Y))
			{
				input = KeyCode::Y;
			}
			if (functions::get_key(KeyCode::U))
			{
				input = KeyCode::U;
			}
			if (functions::get_key(KeyCode::I))
			{
				input = KeyCode::I;
			}
			if (functions::get_key(KeyCode::O))
			{
				input = KeyCode::O;
			}
			if (functions::get_key(KeyCode::P))
			{
				input = KeyCode::P;
			}
		}

		for (int i = 0; i < variables::player_list.size(); i++)
		{
			auto player = variables::player_list[i];
			if (!knocked)
			{
				if (player->is_knocked())
					continue;
			}
			if (!sleeper)
			{
				if (player->is_sleeping())
					continue;

			}
			if (player->is_same_team(player))
				continue;
			auto distancea = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());

			if (distancea > options::PlayerDistance)
				continue;



			if (functions::get_key(KeyCode::F9))
			{
				{ esp::render_box(player); }
			}
			if (options::box) { esp::render_box(player); }
			if (options::chams) { esp::do_chams(player, vr, vg, vb, nr, ng, nb); }
			if (options::name) { esp::render_name(player); }
			
			if (options::helditemname) { esp::render_weapon(player); }
			if (options::Health) { esp::render_Health(player); }
			if (options::Distance) { esp::render_distance(player); }
			if (options::radar) { esp::radar(); }
			if (options::bones) { esp::bullettraceshitty( player, options::xx, options::yy, options::zz); }
			
			if (options::aimbot) { 
				KeyCode Apple;
				if (functions::get_key(KeyCode::DownArrow))
				{
					Apple = Onput();
				}
				esp::render_aimbot(player, Aimbotbone, 100, Apple); 
			
			}
			if (options::aimbotb) { esp::aimbot(player, Aimbotbone, options::Combatfovvalue); }
			if (options::hotbarsad) { esp::render_hotbar(player,y,l); }
				
			//if (options::Shitaim) { esp::render_shitaim(player, Aimbotbone); }
		}
	
		if (options::norecoil) { esp::NoRecoil(options::x); }
		if (options::fullauto) { esp::FullAuto(); }
		if (options::ammo) { esp::ammobar(); }
		if (options::spinbot) { esp::spinbot(); }
		if (options::nosway) { esp::nosway(); }
		if (options::noami) { esp::noanimation(); }
		if (options::fakeleg) { esp::fakelag(); }
		if (options::item) { itemesp::Items(options::itemdistance); }
		if (options::instareviveTeam) { esp::TeamInstantRevive(); }
		if (options::instaeoka) { esp::instaEoka(); }
		if (options::Combatfov) { esp::FovCircle(options::Combatfovvalue); }
		if (options::Stash) { itemesp::Stash(options::StashDistance); }

		if (options::vehicle) { itemesp::Vecicals(options::vehicledistance); }
		if (options::raid) { itemesp::Raid(options::raiddistance); }
		if (options::Storage) { itemesp::Storage(options::StorageDistance); }
		if (options::trap) { itemesp::Traps(options::StashDistance); }
		if (options::animal) { itemesp::animals(options::animaldistance); }

		if (options::instaRevive) { esp::instatRevive(); }
		if (options::flybar) { esp::flyhackbar(); }
		if (options::admin) { esp::SafeAdmin(); }
		if (options::moonJump) { esp::MoonJump(); }
		if (options::speed) { esp::speed(); }
		if (options::Water) { esp::WalkOnWalter(); }
		if (options::extendedmelee) { esp::ExterndMelee(); }
		if (options::instabow) { esp::fastbow(); }
		if (options::longeyes) { esp::longeye(); }
		if (options::dysnicfromserver) { esp::Dysnic(); }
		if (options::AllDr) { esp::alldr(); }
		if (options::coolbow) { esp::coolbow(); }
		if (options::nocol) { esp::nocollision(); }
		if (options::shnospreadl) { esp::shotgunnospread(); }
		if (options::spiderman) { esp::spiderman(); }
		if (options::fatbullet) { esp::fatbullet(); }
		if (options::night) { esp::autoreload(); }
		if (options::w2p) { esp::fakeshotaa(); }
		if (options::fakefire) { esp::fakeshot(); }
		if (options::cleanday) { esp::daytime(); }
		if (options::day) { esp::daytime(); }
		if (options::fovchang) { esp::fovchange(options::fovamout); }
		if (options::spinbot) { esp::spinbot(); }
		if (options::groundfucker) { esp::GroundFucker(); }
		if (options::onladder) { esp::ladder(); }
		if (options::instaRevive) { esp::instatRevive(); }
		if (options::manip) { esp::FindManipulationAngle(); }
		if (options::fakeshot) { esp::fakeshot(); }
		if (options::dysnicbar) { esp::DysnicBar(); }
		if (options::backtrace) { esp::Backtrack(); }
		
		if (options::manip && functions::get_key(KeyCode::X)) { 
			Vector3 angle = esp::FindManipulationAngle(); 
			if (!angle.IsZero()) {
				variables::local_player->get_eyes()->longeyes(angle);
				functions::outline(Vector4(variables::screen_width / 2, variables::screen_height / 2 + 5, 150, 20), xorstr_("Found Angle"), Vector4(0, 0, 0, 1));
				functions::label(Vector4(variables::screen_width / 2, variables::screen_height / 2 + 5, 150, 20), xorstr_("Found Angle"), Vector4(1, 1, 1, 1));
			}
			else {
				functions::outline(Vector4(variables::screen_width / 2, variables::screen_height / 2 + 5, 150, 20), xorstr_("Empty"), Vector4(0, 0, 0, 1));
				functions::label(Vector4(variables::screen_width / 2, variables::screen_height / 2 + 5, 150, 20), xorstr_("Empty"), Vector4(1, 1, 1, 1));
			}
		}
		
		return;
	}

	static bool wakeup{ true };
	uintptr_t performaceui_hook;
	void hk_performance_ui_update(void* instance)
	{
		if (wakeup)
		{
			auto game_object = il2cpp::methods::object_new(il2cpp::init_class(xorstr_("GameObject"), xorstr_("UnityEngine")));
			functions::create(game_object, xorstr_(""));
			functions::add_component(game_object, il2cpp::type_object(xorstr_(""), xorstr_("DevControls")));
			functions::dont_destroy_on_load(game_object);
			wakeup = false;
		}

		auto static update_performance_ui = reinterpret_cast<void(*)(void* instance)>(performaceui_hook);
		SpoofReturn(update_performance_ui, instance);
	}
}