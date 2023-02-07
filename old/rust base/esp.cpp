#include "esp.h"
#include "../rust base/utils/memory.hpp"
#include "../rust base/game/offsets.hpp"
#include <cmath>
#include "Glob.hpp"
#include "aimbot.h"

#include <Windows.h>

namespace esp
{
	uintptr_t shader;
	void do_chams(rust::BasePlayer* player, float vischamr, float vischamg, float vischamb, float nonchamr, float nonchamg, float nonchamb)
	{
		if(!variables::asset_bundle)
			variables::asset_bundle = functions::load_bundle_file("C:\\shitcoder\\assets.saph");

		auto multiMesh = player->get_model()->get_multimesh();
		auto ourmesh = variables::local_player->get_model()->get_multimesh();

		if (!multiMesh)
			return;

		auto renderers = functions::get2renderers(multiMesh);
		auto ourrenders = functions::get2renderers(ourmesh);

		if (!renderers)
			return;

		for (int i = 0; i < renderers->get_size(); i++)
		{
			auto renderer = renderers->get_value(i);
			
			if (!renderer)
				return;

			const auto material = functions::get_material(renderer);
			if (!material)
				return;

			if (!shader)
				shader = functions::load_asset(variables::asset_bundle, "flatshader.shader", il2cpp::type_object("UnityEngine", "Shader")); // there are two flatshader.shader and shader.shader

			if (functions::get_shader(material) == shader)
				return;

			functions::set_shader(material, shader);
            
           

            functions::set_color(material, "ColorBehind", { nonchamr, nonchamg,nonchamb, 255});
			functions::set_color(material, "ColorVisible", { vischamr, vischamg, vischamb, 0.2});   
		}
	}
    void render_box(rust::BasePlayer* player) {
        //for (int i = 0; i < variables::player_list.size(); i++) {
          //  auto player = variables::player_list[i];

            if (player == variables::local_player)
                return;

            auto bounds = get_bounds(player, 2.f);
            float box_width = bounds.right - bounds.left;
            float box_height = bounds.bottom - bounds.top;
            auto half = (bounds.right - bounds.left) / 2;
            Vector2 HeadPos;
            Vector2 NeckPos;
            Vector2 PelvisPos;
            Vector2 LegLPos;
            Vector2 LegRPos;
          
            auto distancea = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());

           
            if (bounds.onscreen) {
                float resp, gesp, besp;
                resp = 0.0f;
                gesp = 1.0f;
                besp = 0.2f;

                if (player->get_model()->is_npc()) {
                    resp = 0.5f;
                    gesp = 0.2f;
                    besp = 0.0f;
                }
                if (player->is_knocked()) {
                    resp = 1.0f;
                    gesp = 0.0f;
                    besp = 0.0f;
                }

                if (player->is_sleeping()) {
                    resp = 0.4f;
                    gesp = 0.0f;
                    besp = 1.0f;
                }
                Vector3 vis = player->get_model()->get_position();
                if (player->is_visible(vis)) {
                    resp = 0.3f;
                    gesp = 0.2f;
                    besp = 0.9f;
                }
                functions::outline_box({
                  bounds.left - 1,
                  bounds.top - 1
                    }, {
                      box_width + 2,
                      box_height + 2
                    }, Vector4(resp, gesp, besp, 1), 1);
                functions::outline_box({
                  bounds.left,
                  bounds.top
                    }, {
                      box_width,
                      box_height
                    }, Vector4(1, 1, 1, 1), 1);

             
            }

            //	functions::set_timescale(12.0f);

        }
    void render_name(rust::BasePlayer* player) {
        //for (int i = 0; i < variables::player_list.size(); i++) {
          //  auto player = variables::player_list[i];

            if (player == variables::local_player)
                return;

            auto bounds = get_bounds(player, 2.f);
            float box_width = bounds.right - bounds.left;
            float box_height = bounds.bottom - bounds.top;
            auto half = (bounds.right - bounds.left) / 2;
            Vector2 HeadPos;
            Vector2 NeckPos;
            Vector2 PelvisPos;
            Vector2 LegLPos;
            Vector2 LegRPos;

            auto distancea = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());


            if (bounds.onscreen) {
                float resp, gesp, besp;
                resp = 0.0f;
                gesp = 1.0f;
                besp = 0.2f;

                if (player->get_model()->is_npc()) {
                    resp = 0.5f;
                    gesp = 0.2f;
                    besp = 0.0f;
                }
                if (player->is_knocked()) {
                    resp = 1.0f;
                    gesp = 0.0f;
                    besp = 0.0f;
                }

                if (player->is_sleeping()) {
                    resp = 0.4f;
                    gesp = 0.0f;
                    besp = 1.0f;
                }
                Vector3 vis = player->get_model()->get_position();
                if (player->is_visible(vis)) {
                    resp = 0.3f;
                    gesp = 0.2f;
                    besp = 0.9f;
                }

                auto name = player->get_username();
                std::string wcs = functions::ws2s(name);
                functions::outline(Vector4{
                  bounds.left - 80.f, bounds.bottom + 5, 79.f + half * 2 + 80.f, 13
                    }, wcs, Vector4(resp, gesp, besp, 1), true, 10);
                functions::label(Vector4{
                  bounds.left - 80.f, bounds.bottom + 5, 79.f + half * 2 + 80.f, 13
                    }, wcs, {
                      1,
                      1,
                      1,
                      1
                    }, true, 10);
               

            }

            //	functions::set_timescale(12.0f);

        }
    void render_distance(rust::BasePlayer* player) {
       
       // for (int i = 0; i < variables::player_list.size(); i++) {
        //    auto player = variables::player_list[i];
           
            //auto Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>(); //GetModuleHandleA(xorstr_("GameAssembly.dll"));

            //	functions::set_draw_color(Vector4(1, 1, 1, 1));

            //bool is_npc = false;
            //	if (model->is_npc())
            //	return;
            //	else
            //	is_npc = true;
            int espas;

            if (player == variables::local_player)
                return;

            auto bounds = get_bounds(player, 2.f);
            float box_width = bounds.right - bounds.left;
            float box_height = bounds.bottom - bounds.top;
            auto half = (bounds.right - bounds.left) / 2;
            Vector2 HeadPos;
            Vector2 NeckPos;
            Vector2 PelvisPos;
            Vector2 LegLPos;
            Vector2 LegRPos;

            auto distancea = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());

            if (bounds.onscreen) {

                std::string playerdistance = "[ " + std::to_string(int(distancea)) + "m ]";

                functions::outline(Vector4{
                  bounds.left - 80.f, bounds.bottom + 25, 79.f + half * 2 + 80.f, 13
                    }, playerdistance, Vector4(0.5, 0, 1.2, 1), true, 10);
                functions::label(Vector4{
                  bounds.left - 80.f, bounds.bottom + 25, 79.f + half * 2 + 80.f, 13
                    }, playerdistance, {
                      1,
                      1,
                      1,
                      1
                    }, true, 10);

            }


       // }
    }   
    void render_Health(rust::BasePlayer* player) {
        //for (int i = 0; i < variables::player_list.size(); i++) {
         //   auto player = variables::player_list[i];

            //auto Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>(); //GetModuleHandleA(xorstr_("GameAssembly.dll"));

            //	functions::set_draw_color(Vector4(1, 1, 1, 1));

            //bool is_npc = false;
            //	if (model->is_npc())
            //	return;
            //	else
            //	is_npc = true;
            int espas;

            if (player == variables::local_player)
                return;

            auto bounds = get_bounds(player, 2.f);
            float box_width = bounds.right - bounds.left;
            float box_height = bounds.bottom - bounds.top;
            auto half = (bounds.right - bounds.left) / 2;
            Vector2 HeadPos;
            Vector2 NeckPos;
            Vector2 PelvisPos;
            Vector2 LegLPos;
            Vector2 LegRPos;

            auto distancea = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());


            if (bounds.onscreen) {

                auto health = Vector4(bounds.left - 4, bounds.top + (box_height - box_height * (player->get_health() / 100)), 5, box_height * (player->get_health() / 100));
                const auto health_bg = Vector4(bounds.left - 5, bounds.top, 7, box_height);

                functions::fill_box(health_bg, Vector4(1, 0, 0, 1));

                functions::fill_box(health, Vector4(0.2, 0.8, 0, 1));
            }
       // }
    }
    void render_weapon(rust::BasePlayer* player) {
        //for (int i = 0; i < variables::player_list.size(); i++) {
         //   auto player = variables::player_list[i];

            auto bounds = get_bounds(player, 2.f);
            float box_width = bounds.right - bounds.left;
            float box_height = bounds.bottom - bounds.top;
            auto half = (bounds.right - bounds.left) / 2;
            Vector2 HeadPos;
            Vector2 NeckPos;
            Vector2 PelvisPos;
            Vector2 LegLPos;
            Vector2 LegRPos;

           

            if (bounds.onscreen) {

                std::string wName = functions::ws2s(player->get_held_item()->get_name());
                functions::outline(Vector4{
                  bounds.left - 80.f, bounds.top - 20, 79.f + half * 2 + 80.f, 13
                    }, wName, Vector4(0, 0.2, 1, 1), true, 10);
                functions::label(Vector4{
                  bounds.left - 80.f, bounds.top - 20, 79.f + half * 2 + 80.f, 13
                    }, wName, {
                      1,
                      1,
                      1,
                      1
                    }, true, 10);

            }

        }
    void render_aimbot(rust::BasePlayer* player, int bone) {
        //for (int i = 0; i < variables::player_list.size(); i++) {
          //  auto player = variables::player_list[i];

            //auto Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>(); //GetModuleHandleA(xorstr_("GameAssembly.dll"));

            //	functions::set_draw_color(Vector4(1, 1, 1, 1));

            //bool is_npc = false;
            //	if (model->is_npc())
            //	return;
            //	else
            //	is_npc = true;
        int espas;

        if (player == variables::local_player)
            return;

        //variables::local_player->get_movement()->fun();
        //variables::local_player->get_movement()->spiderman();

        auto bounds = get_bounds(player, 2.f);
        float box_width = bounds.right - bounds.left;
        float box_height = bounds.bottom - bounds.top;
        auto half = (bounds.right - bounds.left) / 2;
        Vector2 HeadPos;
        Vector2 NeckPos;
        Vector2 PelvisPos;
        Vector2 LegLPos;
        Vector2 LegRPos;
        //aimbot::getClosestBoneToCrosshair(player);
        //aimbot::getBestPlayerByFov();
        //aimbot::run();
        
        if (bounds.onscreen) {
            if (functions::get_key(KeyCode::Mouse3)) {
                aimbot::run(bone);
                espas = 1;
            }
            else {
                espas = 0;
            }

            if (espas == 1) {
                functions::outline(Vector4(variables::screen_width / 2, variables::screen_height / 2 - 10, 150, 20), xorstr_("Locking on"), Vector4(0, 0.5, 1, 1));
                functions::label(Vector4(variables::screen_width / 2, variables::screen_height / 2 - 10, 150, 20), xorstr_("Locking on"), Vector4(1, 1, 1, 1));
            }
        }
    }
    void SafeAdmin() { variables::local_player->add_flag(4); }
    void NoRecoil() { variables::local_player->get_held_item()->Norecoil(0.0f); }
    void nosway() {
        variables::local_player->get_held_item()->set_aim_cone(-1.0f);
        variables::local_player->get_held_item()->set_aim_cone_curve_scale(-1.0f);
        variables::local_player->get_held_item()->set_aim_cone_penalty_max(-1.0f);
        variables::local_player->get_held_item()->set_aim_cone_penalty_per_shot(-1.0f);
        variables::local_player->get_held_item()->set_aim_sway(-1.0f);
        variables::local_player->get_held_item()->set_aim_sway_speed(-1.0f);
    }
    void instaEoka() {
        variables::local_player->get_held_item()->set_always_eoka(100.0f);
    }
    void FullAuto() { variables::local_player->get_held_item()->set_automatic(true); }
    void MoonJump() { variables::local_player->get_movement()->fun(); }
    void ShootWhileInAir() { variables::local_player->get_movement()->ShootInAir(); }
    void WalkOnWalter() {
        functions::ignore_layer_collision(layer::PlayerMovement, layer::Water, 0);
            }
    void ExterndMelee() { variables::local_player->get_held_item()->ExtMelee(); }
    void fastbow() {
        if (functions::get_key(KeyCode::Mouse0))
        {
            functions::outline(Vector4(variables::screen_width / 2 + 5, variables::screen_height / 2, 150, 20), xorstr_("BING"), Vector4(0, 0, 0, 1));
            functions::label(Vector4(variables::screen_width / 2 + 5, variables::screen_height / 2, 150, 20), xorstr_("BING"), Vector4(1, 1, 1, 1));

            variables::local_player->get_lastSentTick()->SetTicks(0.99);
            variables::local_player->get_held_item()->instabow();
        }

    }
    void instacharge() { variables::local_player->get_held_item()->instacharge(); }
    void longeye() { variables::local_player->get_eyes()->longeyes(); }
    void Dysnic() {

        if (functions::get_key(KeyCode::Mouse2))
        {
            variables::local_player->get_lastSentTick()->SetTicks(0.99);

            std::string ServerTicks = " [ Ticks Per Second -> " + std::to_string(float(variables::local_player->get_lastSentTick()->TicksPerSec())) + " ]";
            functions::outline(Vector4(variables::screen_width / 2 + 5, variables::screen_height / 2, 150, 20), xorstr_("Dysnic"), Vector4(0, 0, 0, 1));
            functions::label(Vector4(variables::screen_width / 2 + 5, variables::screen_height / 2, 150, 20), xorstr_("Dysnic"), Vector4(1, 1, 1, 1));

        }
    }
    void fastswitch() { variables::local_player->get_held_item()->FastSwitch(); }
    void RaidFire() { variables::local_player->get_held_item()->FastFire(); }
    void spiderman() { variables::local_player->get_movement()->spiderman(); }
    void omean() { variables::local_player->get_movement()->Omean(); }
    void fatbullet() { variables::local_player->get_held_item()->fatbullet(); }
    void BrightNIght() {
       
    }
    void spinbot() {
        variables::local_player->Spinbot();
    }
    void fovchange(float fov)
    {
        uintptr_t Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>();
        auto addressOne = *reinterpret_cast<uintptr_t*>(Vbase + 48764840);
        if (!addressOne)
            return;
        auto addressTwo = *reinterpret_cast<uintptr_t*>(addressOne + 0xB8);
        if (!addressTwo)
            return;
        memory::write<float>(addressTwo + 0x18, fov);
    }
    void daytime()
    {
       
    }
    void shotgunnospread()
    {
        variables::local_player->get_held_item()->shotgun();
    }

    void nicesky()
    {
       // uintptr_t Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>();
    }
    void GroundFucker()
    {
      variables::local_player->get_movement()->ShootInAir();
    }
    void ladder()
    {
        variables::local_player->get_modelstate()->set_onLadder(1);
    }
    void omniSprint() {

    }
    void nocollision()
    {
        functions::ignore_layer_collision(layer::PlayerMovement, layer::Tree, 1);
        functions::ignore_layer_collision(layer::PlayerMovement, layer::AI, 1);

    }
    void instatRevive() {
        static auto ServerRPC = reinterpret_cast<void (*)(uintptr_t, std::string funcName)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("BaseEntity"), ("ServerRPC"), 1, ("funcName"), (""), 1)));
        auto target = aimbot::getBestPlayerByFov();
        if (target->get_health() <= 4 && target->is_same_team(variables::local_player) && Math::Calc3D_Dist(variables::local_position, target->get_model()->get_position()) <= 5)
            ServerRPC((uintptr_t)target, std::string("RPC_Assist"));
    }

    
    void DysnicBar()
    {
        auto loco = variables::local_player;
       
        float desyncTime = (functions::get_deltaTime() - loco->get_lastSentTick()->TicksPerSec()) - 0.03125 * 3;


        float red, green, blue;
        float Num = desyncTime;
        if (desyncTime < 0.1)
            Num = 0;

        if (Num < 0.5)
        {
            red = Num * 2.f * 255.f;
            green = 255.f;
            blue = 0.f;
        }
        else
        {
            red = 255.f;
            green = (2.f - 2.f * Num) * 255.f;
            blue = 0.f;
        }
        if (Num > 1.f)
            Num = 1.f;

             
            functions::outline_box(Vector2(variables::screen_width / 2 - 50, variables::screen_height / 2 - 105), Vector2(100, 8), Vector4(0, 0, 0, 1));
            functions::fill_box(Vector4(variables::screen_width / 2 - 50, variables::screen_height / 2 - 105, 100 * (desyncTime), 8), Vector4(red, green, blue, 1));

    }
    void fakeshot()
    {
        variables::local_player->get_held_item()->get_held_entity()->mounted() == false;
    
    }
    void Backtrack()
    {
        Vector3 OurPlayer = variables::local_position;
        Vector3 OurPenis = variables::local_player->get_bone_position(BoneList::l_knee);
        //RainbowLoop();
      
       // functions::ddraw_line(Vector3(OurPlayer.x, OurPlayer.y, OurPlayer.z), Vector3(OurPenis.x, OurPenis.y, OurPenis.z), Vector4(0.3, 0.8, 0, 1));
        functions::ddraw_arrow(Vector3(OurPlayer.x, OurPlayer.y, OurPlayer.z), Vector3(OurPenis.x, OurPenis.y, OurPenis.z), 0.1, Vector4(205.0f,100.0f, 0, 255));
    }
    Vector3 FindManipulationAngle()
    {
        auto loco = variables::local_player;
        rust::BasePlayer* target_ply = aimbot::getBestPlayerByFov();
        Vector3 re_p = loco->get_eyes()->get_position() + loco->get_transform()->up() * (variables::local_player->get_eyes()->get_eyeOffset().y + loco->get_eyes()->get_viewoffset().y);
        // real eye pos
        
        Vector3 choice = Vector3(0, 0, 0);
       if (target_ply->get_mpv_bone() == NULL || target_ply == nullptr) return Vector3(0, 0, 0);

       if (functions::is_visible(re_p, target_ply->get_bone_position(target_ply->get_mpv_bone()))) {
           return Vector3(0, 0, 0);
       }

        float desyncTime = (functions::realtimeSinceStartup() - loco->get_lastSentTick()->TicksPerSec()) - 0.03125 * 3;
        float mm_max_eye = ((0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * variables::local_player->MaxVelocity()) - 0.05f);
        float max_v_multicplier = 4.5f;
        float v_mm_max_eye = mm_max_eye / max_v_multicplier;

        auto right = loco->get_eyes()->get_bodyright();
        auto forward = loco->get_eyes()->get_bodyforward();

        std::array<Vector3, 11> arr = {
            Vector3(right.x * (mm_max_eye / 2), 0.f, right.z * (mm_max_eye / 2)), // small right
            Vector3(right.x * mm_max_eye, 0.f, right.z * mm_max_eye), // big right

            Vector3(-(right.x * (mm_max_eye / 2)), 0.f, -(right.z * (mm_max_eye / 2))), // small left
            Vector3(-(right.x * mm_max_eye), 0.f, -(right.z * mm_max_eye)), // big left

            Vector3(0.f, (v_mm_max_eye / 5), 0.f), // small up
            Vector3(0.f, v_mm_max_eye / 2, 0.f), // medium up
            Vector3(0.f, v_mm_max_eye, 0.f), // big up

            Vector3(forward.x * (mm_max_eye / 2), 0.f, forward.z * (mm_max_eye / 2)), // small forward
            Vector3(forward.x * mm_max_eye, 0.f, forward.z * mm_max_eye), // big forward

            Vector3(-(forward.x * (mm_max_eye / 2)), 0.f, -(forward.z * (mm_max_eye / 2))), // small backward
            Vector3(-(forward.x * mm_max_eye), 0.f, -(forward.z * mm_max_eye)), // big backward
        }; // restoring procedure: *= 1.428571428571429 for 0.7f
           //                      *= 0.7142857142857143 for 1.4f
        
        for (auto s : arr) {//
            auto b = Vector3(s.x, s.y, s.z);
            Vector3 point = re_p + b;

            if (!functions::is_visible(point, re_p))
                continue;

            if (!functions::is_visible(target_ply->get_bone_position(BoneList::head), point))
                continue;

            choice = b;
            break;
        }
        if (choice.IsZero()) {
            return Vector3(0, 0, 0);
        }

       return choice;
    }
}
   
    
  