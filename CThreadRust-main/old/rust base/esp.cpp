#include "esp.h"
#include "../rust base/utils/memory.hpp"
#include "../rust base/game/offsets.hpp"
#include <cmath>
#include "Glob.hpp"
#include "aimbot.h"

#include <Windows.h>

std::chrono::time_point<std::chrono::steady_clock> m_StartTime;



    void Start()
    {
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

    float GetDuration()
    {
        std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_StartTime;
        return duration.count();
    }

    void RotatePoint(float* pointToRotate, float* centerPoint, float angle, float* ReturnTo)
    {
        angle = (float)(angle * (M_PI / (float)180));

        float cosTheta = (float)cos(angle);
        float sinTheta = (float)sin(angle);

        ReturnTo[0] = cosTheta * (pointToRotate[0] - centerPoint[0]) - sinTheta * (pointToRotate[1] - centerPoint[1]);
        ReturnTo[1] = sinTheta * (pointToRotate[0] - centerPoint[0]) + cosTheta * (pointToRotate[1] - centerPoint[1]);

        ReturnTo[0] += centerPoint[0];
        ReturnTo[1] += centerPoint[1];
    }


namespace esp
{

    void radar()
    {
        //if (options::radar)
        {
            float x = 200, y = 200;
            float resx = 200, resy = 200;
            float screenPos[2];
            float RotatedPoint[2];
            float centerx = x + (resx / 2), centery = 300;
            float centerWindowPos[2] = { centerx, centery };

            functions::fill_box({ x, y, resx, resy }, { 0.2f, 0.2f, 0.2f, 0.2f });

            for (int i = 0; i < variables::player_list.size(); i++)
            {
                Vector4 color = { 0,0,1,1 };
                rust::BasePlayer* player = variables::player_list.at(i);

                if (player->is_knocked())
                    color = { 1,0,0,1 };
                if (player->get_model()->is_npc())
                    color = { 0.9, 0, 1, 1 };
                if (player->is_same_team(variables::local_player))
                    color = { 0, 1, 0, 1 };
                if (player->is_sleeping())
                    color = { 0, 0, 1, 1 };

                screenPos[0] = variables::local_position.x - player->get_bone_position(BoneList::spine1).x;
                screenPos[1] = variables::local_position.z - player->get_bone_position(BoneList::spine1).z;
                screenPos[0] *= 1;
                screenPos[1] *= 1;
                screenPos[0] *= -1;
                screenPos[0] += centerx;
                screenPos[1] += centery;

                RotatePoint(screenPos, centerWindowPos, 360.0f - variables::local_player->get_input()->get_view_angles().y, RotatedPoint);

                if (RotatedPoint[0] < x) continue;
                if (RotatedPoint[1] < y) continue;
                if (RotatedPoint[0] > x + resx) continue;
                if (RotatedPoint[1] > y + resy) continue;

                functions::fill_box({ RotatedPoint[0] - 1, RotatedPoint[1] - 1, 4, 4 }, { 0,0,0,1 });
                functions::fill_box({ RotatedPoint[0], RotatedPoint[1], 2, 2 }, color);
            }
            functions::fill_box({ centerx, centery, 2, 2 }, { 1,0.5,0,1 });
        }
    }
	uintptr_t shader;
    void do_handchams()
    {
        if (!variables::asset_bundle)
			variables::asset_bundle = functions::load_bundle_file("C:\\asd.asset");

		auto ourmesh = variables::local_player->get_held_item()->Model();

        if (!ourmesh)
            return;

        auto ourrenders = functions::get2renderers(ourmesh);

        if (!ourrenders)
            return;

        for (int i = 0; i < ourrenders->get_size(); i++)
        {
            auto renderer = ourrenders->get_value(i);

            if (!renderer)
                return;

            const auto material = functions::get_material(renderer);
            if (!material)
                return;


            if (!shader)
                shader = functions::load_asset(variables::asset_bundle, "shader.shader", il2cpp::type_object("UnityEngine", "Shader")); // there are two flatshader.shader and shader.shader
            if (functions::get_shader(material) == shader)
                return;

            functions::set_shader(material, shader);


            functions::set_color(material, "ColorBehind", { 0, 255,0, 0.01 });
            functions::set_color(material, "ColorVisible", { 255, 0, 0, 0.01 });
        }
    }


    void do_chams(rust::BasePlayer* player, float vischamr, float vischamg, float vischamb, float nonchamr, float nonchamg, float nonchamb)
    {

       // do_handchams();
        if (!variables::asset_bundle)
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



			functions::set_color(material, "ColorBehind", { 255.0f, 0,0, 0.2 });
			functions::set_color(material, "ColorVisible", { 0, 255, 0, 0.2 });
        }
    }

	void pickup()
	{
		
	}

    void render_hotbar(rust::BasePlayer* player)
    {
            auto belt = aimbot::getBestPlayerByFov()->aget_hotbar_items();
        if (!belt.empty())
        {
            float info_y = 0;
            for (int i = 0; i < 6; i++)
            {
                float mat = 500 + info_y;
                functions::fill_box({ mat, 300, 75, 75 }, { 0, 0, 0, 0.09 });

                if (i > belt.size() - 1)
                    continue;
                auto item = belt.at(i);

                if (item != NULL)
                {
                    auto weapon_name = functions::ws2s(item->get_name());

                    if (!weapon_name.empty())
                    {
                        auto sprite_the_drink = item->get_sprite();
                        if (!sprite_the_drink)
                            continue;

                        auto texture = functions::get_texture(sprite_the_drink);
                        if (!texture)
                            continue;


                        functions::set_draw_color({ 1, 1, 1, 1 });
                        functions::draw_texture({ mat, 300, 75, 75 }, texture);
                        info_y += 80;
                    }
                }

            }
            info_y = 0;
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
                    }, Vector4(0, 0, 0, 1), 1);
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
                    }, wcs, Vector4(0, 0, 0, 1), true, 10);
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
                    }, playerdistance, Vector4(0, 0, 0, 0), true, 10);
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
    void fakelag()
    {
        variables::local_player->get_movement()->set_targetmovement(Vector3(100, 0, 25));
    }

    void ammobar()
    {
        float ammo = variables::local_player->get_held_item()->ammocount();
        if (ammo == 1)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
        }
        if (ammo == 2)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
        }
        if (ammo == 3)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
        }
        if (ammo == 4)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }
        if (ammo == 5)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
        }
        if (ammo == 6)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }

        if (ammo == 7)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }

        if (ammo == 8)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }
        if (ammo == 9)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }
        if (ammo == 10)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }
        if (ammo == 11)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }
        if (ammo == 12)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }

        if (ammo == 13)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }

        if (ammo == 14)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }
        if (ammo == 15)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }
        if (ammo == 16)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }

        if (ammo == 17)
        {   
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }
        if (ammo == 18)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }

        if (ammo == 19)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));


        }
        if (ammo == 20)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }

        if (ammo == 21)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }

        if (ammo == 22)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }


        if (ammo == 23)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }

        if (ammo == 24)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }
        if (ammo == 25)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 32, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }
        if (ammo == 26)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 32, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 30, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }

        if (ammo == 27)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 32, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 30, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 28, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }

        if (ammo == 27)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 32, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 30, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 28, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 26, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }
        if (ammo == 28)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 32, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 30, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 28, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 26, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 24, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }
        if (ammo == 29)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 32, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 30, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 28, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 26, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 24, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 22, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }
        if (ammo == 30)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 32, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 30, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 28, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 26, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 24, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 22, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 20, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }
        if (ammo == 31)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 32, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 30, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 28, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 26, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 24, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 22, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 20, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 18, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }
        if (ammo == 32)
        {
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 80, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 78, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 76, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 74, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 72, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 70, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 68, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 66, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 64, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 62, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 60, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 58, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 56, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 54, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 52, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 50, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 48, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 46, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 44, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 42, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 40, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 38, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 36, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 34, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 32, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 30, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 28, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 26, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 24, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 22, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 20, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 18, 10, 3), Vector4(0, 0.4, 0.8, 0.4));
            functions::fill_box(Vector4(functions::get_screen_width() / 2 - 8, functions::get_screen_height() / 2 - 16, 10, 3), Vector4(0, 0.4, 0.8, 0.4));

        }
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

                auto health = Vector4(bounds.left - 4, bounds.top + (box_height - box_height * (player->get_health() / 100)), 3, box_height * (player->get_health() / 100));
                const auto health_bg = Vector4(bounds.left - 5, bounds.top, 5, box_height);
                float rra, gga, bba;
                if (player->get_health() > 75.0f)
                {
                    rra = 0;
                    gga = 1;
                    bba = 0;
                }
                if (player->get_health() < 75.0f)
                {
                    if (player->get_health() > 10.0f)
                    {
                        rra = 1;
                        gga = 0.2;
                        bba = 0;

                    }
                }
                if (player->get_health() < 10.0f)
                {
                    rra = 1;
                    gga = 0;
                    bba = 0;
                }

                functions::fill_box(health_bg, Vector4(0, 0, 0, 1));

                functions::fill_box(health, Vector4(rra, gga, bba, 1));
            }
            if (distancea < 50)
            {
                Vector3 eye = player->get_eyes()->get_bodyforward();
                Vector3 Reye = player->get_bone_position(BoneList::r_eye);
                functions::ddraw_arrow(Vector3(eye.x, eye.y, eye.z), Vector3(eye.x - 4, eye.y, eye.z), 0.01f, Vector4(0, 255, 144, 0.1));
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
                    }, wName, Vector4(0, 0, 0, 1), true, 10);
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
    void render_aimbot(rust::BasePlayer* player, int bone, float height) {
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

        Vector3 Neck = player->get_bone_position(BoneList::neck);
        Vector3 Chest = player->get_bone_position(BoneList::BoobCensor);
        Vector3 Penis = player->get_bone_position(BoneList::penis);
        Vector3 Lm = player->get_bone_position(BoneList::l_middle1);
        Vector3 Lmm = player->get_bone_position(BoneList::l_middle2);
        Vector3 Lmmm = player->get_bone_position(BoneList::l_middle3);

        //aimbot::getClosestBoneToCrosshair(player);
        //aimbot::getBestPlayerByFov();
        //aimbot::run();
        functions::outline_box(Vector2((functions::get_screen_width() / 2) - (250 / 2), (functions::get_screen_height() / 2) - (250 /2)), Vector2(250, 250), Vector4(155.0f, 0.0f, 255.0f, 0.7));

        if (bounds.onscreen) {
            if (functions::get_key(KeyCode::L)) {
                aimbot::run(bone, height);
               // esp::render_hotbar(aimbot::getBestPlayerByFov());
            }
            else {
                espas = 0;
            }

          
        }
    }
    void SafeAdmin() { variables::local_player->add_flag(4); }
    void NoRecoil(float recoil) {       
                variables::local_player->get_held_item()->Norecoil(recoil);
  
    }
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

        }
    }
    void fastswitch() { variables::local_player->get_held_item()->FastSwitch(); }
    void RaidFire() { variables::local_player->get_held_item()->FastFire(); }
    void spiderman() { variables::local_player->get_movement()->spiderman(); }
    void omean() { variables::local_player->get_movement()->Omean(); }
    void fatbullet() { variables::local_player->get_held_item()->fatbullet(); }
    void autoreload()
    {
        if (functions::get_key(KeyCode::J))
        {
            variables::local_player->get_held_item()->AutoReload();
        }
    }
    void BrightNIght() {
       

    }
    void spinbot() {
        variables::local_player->canwelditem();

    }
    void fovchange(float fov)
    {
        uintptr_t Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>();
        auto addressOne = *reinterpret_cast<uintptr_t*>(Vbase + 49161488);
        if (!addressOne)
            return;
        auto addressTwo = *reinterpret_cast<uintptr_t*>(addressOne + 0xB8);
        if (!addressTwo)
            return;
        memory::write<float>(addressTwo + 0x18, fov);
       // variables::local_player->get_held_item()->fakefire();

    }
    void daytime()    
    {
    }
    void shotgunnospread()
    {
        variables::local_player->get_input()->set_recoil_angles(Vector2(0, 0));
    }

    void nicesky(float x, float y)
    {
        

       
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
        functions::ignore_layer_collision(layer::Game_Trace, layer::TransparentFX, 1);
        functions::ignore_layer_collision(layer::Game_Trace, layer::Transparent, 1);

    }

    void fakeshot()
    {
        variables::local_player->get_held_item()->fakefire(); 
    }
    void instatRevive() {
        rust::BasePlayer* target_ply = aimbot::getBestPlayerByFov();
        auto distancea = Math::Calc3D_Dist(variables::local_position, target_ply->get_bone_position(BoneList::BoobCensor));
        if (distancea < 4.0f)
        {
            if (target_ply->is_knocked())
            {
                variables::local_player->server_rpc((uintptr_t)target_ply, ("RPC_Assist"));
            }
        }
    }

    void TeamInstantRevive()
    {
        rust::BasePlayer* target_ply = aimbot::getBestPlayerByFov();
        auto distancea = Math::Calc3D_Dist(variables::local_position, target_ply->get_bone_position(BoneList::BoobCensor));
        if (target_ply->is_same_team(variables::local_player))
        {
            if (distancea < 4.0f)
            {
                if (target_ply->is_knocked())
                {
                    variables::local_player->server_rpc((uintptr_t)target_ply, ("RPC_Assist"));
                }
            }
        }
        
    }
    

    
    void DysnicBar()
    {
        auto loco = variables::local_player;
        float desyncTime = (functions::get_time() - loco->get_lastSentTick()->TicksPerSec() - 0.03125 * 3);


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
        if (desyncTime > 0) {
            functions::outline_box(Vector2(variables::screen_width - 50, variables::screen_height - 105), Vector2(100, 8), Vector4(0, 0, 0, 1));
            functions::fill_box(Vector4(variables::screen_width - 50, variables::screen_height - 105, 100 * (desyncTime), 8), Vector4(red, green, blue, 1));
        }

    }
    
    void fakeshotaa()
    {
        rust::BasePlayer* target_ply = aimbot::getBestPlayerByFov();

        Vector3 P = variables::local_player->get_bone_position(BoneList::l_foot);
        Vector3 E = target_ply->get_bone_position(BoneList::l_foot);

        Vector3 EnPos = target_ply->get_bone_position(BoneList::penis);
        Vector3 ourpos = variables::local_player->get_bone_position(BoneList::penis);

        Vector3 Direction = (E - P).Normalize() * 4.4;
        Vector3 Directiona = (E - P).Normalize();


        if (functions::get_key(KeyCode::M)) {
            variables::local_player->get_eyes()->setEyes(Directiona);
            variables::local_player->get_movement()->set_targetmovement(Direction);
            functions::ddraw_arrow(Vector3(ourpos.x, ourpos.y, ourpos.z), Vector3(EnPos.x, EnPos.y, EnPos.z), 0.1f, Vector4(0, 255, 144, 0.1));

        }
          

         //   auto distancea = Math::Calc3D_Dist(variables::local_position, target_ply->get_model()->get_position());

           
    }

   
    void hotbar()
    {
        rust::BasePlayer* target_ply = aimbot::getBestPlayerByFov();
        if (!target_ply)
            return;
        if (target_ply->is_sleeping())
            return;
        if (target_ply->is_same_team(variables::local_player))
            return;
                           
        std::string a = functions::ws2s(target_ply->get_inventory()->get_belt()->get_item(0)->get_name());

        std::string a1 = functions::ws2s(target_ply->get_inventory()->get_belt()->get_item(1)->get_name());
        std::string a2 = functions::ws2s(target_ply->get_inventory()->get_belt()->get_item(2)->get_name());
        std::string a3 = functions::ws2s(target_ply->get_inventory()->get_belt()->get_item(3)->get_name());
        std::string a4 = functions::ws2s(target_ply->get_inventory()->get_belt()->get_item(4)->get_name());
        std::string a5 = functions::ws2s(target_ply->get_inventory()->get_belt()->get_item(5)->get_name());
        if (!a.c_str())
            return;
        if (!a1.c_str())
            return;
        if (!a2.c_str())
            return;
        if (!a3.c_str())
            return;
        if (!a4.c_str())
            return;
        if (!a5.c_str())
            return;

        functions::fill_box(Vector4(8, 258, 110, 120), Vector4(0, 0, 0, 0.7));

        functions::outline(Vector4(12, 260, 150, 20), a, Vector4(0, 0, 0, 1));
        functions::label(Vector4(12, 260, 150, 20), a, Vector4(1, 1, 1, 1));

        functions::outline(Vector4(12, 280, 150, 20), a1, Vector4(0, 0, 0, 1));
        functions::label(Vector4(12, 280, 150, 20), a1, Vector4(1, 1, 1, 1));

        functions::outline(Vector4(12, 300, 150, 20), a2, Vector4(0, 0, 0, 1));
        functions::label(Vector4(12, 300, 150, 20), a2, Vector4(1, 1, 1, 1));

        functions::outline(Vector4(12, 320, 150, 20), a3, Vector4(0, 0, 0, 1));
        functions::label(Vector4(12, 320, 150, 20), a3, Vector4(1, 1, 1, 1));

        functions::outline(Vector4(12, 340, 150, 20), a4, Vector4(0, 0, 0, 1));
        functions::label(Vector4(12, 340, 150, 20), a4, Vector4(1, 1, 1, 1));

        functions::outline(Vector4(12, 360, 150, 20), a5, Vector4(0, 0, 0, 1));
        functions::label(Vector4(12, 360, 150, 20), a5, Vector4(1, 1, 1, 1));


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
   
    
  