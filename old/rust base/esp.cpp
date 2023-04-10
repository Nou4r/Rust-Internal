#include "esp.h"
#include "../rust base/utils/memory.hpp"
#include "../rust base/game/offsets.hpp"
#include <cmath>
#include "Glob.hpp"
#include "aimbot.h"

#include <Windows.h>

std::chrono::time_point<std::chrono::steady_clock> m_StartTime;




void circle(const float x, const float y, const float r, const float s, const Vector4 color) {
    const float step = 3.14159265359 * 2.0 / s;
    for (float a = 0; a < 3.14159265359 * 2.0; a += step) {
        const auto x1 = r * std::cos(a) + x;
        const auto y1 = r * std::sin(a) + y;
        const auto x2 = r * std::cos(a + step) + x;
        const auto y2 = r * std::sin(a + step) + y;


        const auto x3 = r * std::cos(a) - x;
        const auto y3 = r * std::sin(a) * y;
        const auto x4 = r * std::cos(a - step) * x;
        const auto y4 = r * std::sin(a - step) * y;


        const auto x5 = r * std::sin(a) - x;
        const auto y5 = r * std::cos(a) * y;
        const auto x6 = r * std::sin(a - step) * x;
        const auto y6 = r * std::cos(a - step) * y;


        functions::line(Vector2(x1, y1), Vector2(x2, y2), color);
        functions::line(Vector2(x3, y3), Vector2(x4, y4), color);
        functions::line(Vector2(x5, y5), Vector2(x6, y6), color);

        functions::line(Vector2(x2, y2), Vector2(x1, y1), color);
        functions::line(Vector2(x4, y4), Vector2(x2, y2), color);
        functions::line(Vector2(x6, y6), Vector2(x5, y5), color);


    }
}
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
                    rust::BasePlayer* player = variables::player_list.at(i);

                   // if (player->is_knocked())
                     //   color = { 1,0,0,1 };
                    //if (player->get_model()->is_npc())
                      //  color = { 0.9, 0, 1, 1 };
                    //if (player->is_same_team(variables::local_player))
                      //  color = { 0, 1, 0, 1 };
                    //if (player->is_sleeping())
                      //      
                    static int cases = 0;
                    static float r = 1.00f, g = 0.00f, b = 1.00f;
                    switch (cases)
                    {
                    case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
                    case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
                    case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
                    case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
                    default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
                    }
                    Vector4 color = { 1,0,0,1 };

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
                variables::asset_bundle = functions::load_bundle_file("C:\\shitcoder\\assets.saph");


            auto ourmesh = variables::local_player->View();
            //variables::local_player->get_modelstate()->Setfly(1);
            

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
                    shader = functions::load_asset(variables::asset_bundle, "flatshader.shader", il2cpp::type_object("UnityEngine", "Shader")); // there are two flatshader.shader and shader.shader
                if (functions::get_shader(material) == shader)
                    return;

                functions::set_shader(material, shader);
                

                functions::set_color(material, "ColorBehind", { 0, 255,0, 0.01 });
                functions::set_color(material, "ColorVisible", { 255, 0, 0, 0.01 });
            }
        }


        void do_chams(rust::BasePlayer* player, float vischamr, float vischamg, float vischamb, float nonchamr, float nonchamg, float nonchamb)
        {
           // variables::local_player->get_modelstate()->add_flag(16);
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
                static int cases = 0;
                static float r = 1.00f, g = 0.00f, b = 1.00f;
                switch (cases)
                {
                case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
                case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
                case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
                case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
                default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
                }


                functions::set_color(material, "ColorBehind", { 255.0f, 0,0, 0.2 });
                functions::set_color(material, "ColorVisible", { r, g, b, 0.2 });
            }
        }

        void pickup()
        {

        }

       
    
    

    void render_hotbar(rust::BasePlayer* player, float height, float length)
    {

        static int cases = 0;
        static float r = 1.00f, g = 0.00f, b = 1.00f;
        switch (cases)
        {
        case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
        case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
        case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
        case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
        default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
        }
        auto playera = aimbot::hotbar();
        auto belt = aimbot::hotbar()->aget_hotbar_items();
        auto distancea = Math::Calc3D_Dist(variables::local_position, playera->get_model()->get_position());

        if (belt.empty())
            return;
        if (!belt.empty())
        {
            float info_y = 0;
            for (int i = 0; i < 6; i++)
            {
                std::string playername = "[ " + functions::ws2s(playera->get_username()) + " HotBar ] " + " [ Health : " + std::to_string(int(playera->get_health())) + "HP ]" + " [ Distance : " + std::to_string(int(distancea)) + "M ]";
                functions::fill_box({ 1177, 143, 530, 30 }, { 0.1, 0.1, 0.1, 0.8 });
                functions::label(Vector4(1190, 150,1000 , 20), playername, Vector4(1, 1, 1, 0.8));
               
                functions::fill_box({ 1177 + info_y, 180, 75, 75 }, { 0.1, 0.1, 0.1, 0.2 });
                functions::outline_box(Vector2( 1177 + info_y, 180), Vector2(75, 75), Vector4(r, g, b, 0.2));

                if (i > belt.size() - 1)
                {
                    info_y += 95;
                    continue;
                }

                auto item = belt.at(i);
                if (!item)
                    return;

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
                        functions::draw_texture({ 1177 + info_y, 180, 75, 75 }, texture);
                        info_y += 90;
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

            //	functions::set_
            
            
            (12.0f);

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

                functions::conerbox(bounds.left, bounds.right, box_width, box_height, Vector4(1, 0, 0, 1));

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

            auto health = Vector4(bounds.left - 5, bounds.top + (box_height - box_height * (player->get_health() / 100)), 3, box_height * (player->get_health() / 100));
            const auto health_bg = Vector4(bounds.left - 6, bounds.top, 5, box_height);

            auto health2 = Vector4(bounds.left, bounds.bottom, box_width * (player->get_health() / 100), 3);
            const auto health_bg2 = Vector4(bounds.left, bounds.bottom, box_width, 5);

            //functions::fill_box(health_bg, Vector4(0, 0, 0, 1));
            functions::fill_box(health_bg2, Vector4(0, 0, 0, 1));
            //functions::fill_box(health, Vector4(0.2, 0.8, 0, 1));
            functions::fill_box(health2, Vector4(0.2, 0.8, 0, 1));
        }
    }
    void bones(rust::BasePlayer* player)
    {
        Vector2 BoneNeck;
        Vector3 Neck = player->get_bone_position(BoneList::neck);
        Vector3 Dick = player->get_bone_position(BoneList::pelvis);
        Vector3 Chest = player->get_bone_position(BoneList::BoobCensor);
        Vector2 Chesta;
        Vector3 LAr = player->get_bone_position(BoneList::l_upperarm);
        Vector3 LLAr = player->get_bone_position(BoneList::l_forearm);
        Vector2 LArm;
        Vector2 LLArm;
        Vector2 BoneDicl;
        Vector3 LegR = player->get_bone_position(BoneList::r_hip);
        Vector3 LegLR = player->get_bone_position(BoneList::r_knee);
        Vector2 LegLL;
        Vector2 RLeg;
        functions::WorldToScreen(Neck, BoneNeck);
        functions::WorldToScreen(LegLR, LegLL);
        functions::WorldToScreen(Chest, Chesta);
        functions::WorldToScreen(LAr, LLArm);
        functions::WorldToScreen(LLAr, LArm);



        functions::WorldToScreen(Dick, BoneDicl);
        functions::WorldToScreen(LegR, RLeg);
        functions::line(Vector2(functions::get_screen_width() / 2, functions::get_screen_height()  / 2), Vector2(Neck.x, Neck.y), Vector4(1, 0, 0, 1));
 
        functions::line(Vector2(functions::get_screen_width() / 2, functions::get_screen_height() / 2), Vector2(BoneNeck.x, BoneNeck.y), Vector4(0, 1, 0, 1));


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
    void bullettraceshitty(rust::BasePlayer* ent, float x, float y, float z)
    {

       Vector3 Eyes = ent->get_bone_position(BoneList::eyeTranform);
        rust::BasePlayer* Player = variables::local_player;
       Vector3 Pos =  Player->get_bone_position(BoneList::eyeTranform);
       
       if (variables::local_player->get_movement()->ducking())
       {
           auto ducking = il2cpp::il2cppstring::New("[ Your Ducking ]");
           functions::ddraw_screentext(ducking, x, Vector4(1, 0, 0, 255));
       }
       if (fastbow && functions::get_key(KeyCode::Mouse0))
       {
           std::string duck = " [ Invalid Hit At X." + std::to_string(float(Eyes.x)) + " Y." + std::to_string(float(Eyes.y)) + " Z." + std::to_string(float(Eyes.z)) + " ]";
           auto ducking = il2cpp::il2cppstring::New(duck.c_str());
       }
    }

   
    void render_aimbot(rust::BasePlayer* player, int bone, float height, KeyCode input) {
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
        //rust::BaseProjectile a = variables::local_player->get_held_item();
        if (bounds.onscreen) {
            auto a = aimbot::getBestPlayerByFov();
         //   aimbot::draw();

          //  functions::line(Vector2(functions::get_screen_width() / 2, functions::get_screen_height() / 2 ), Vector2( a->get_bone_position(BoneList::neck).x, a->get_bone_position(BoneList::neck).y), Vector4(1, 1, 1, 1));

            if (functions::get_key(KeyCode::Mouse3)) {
                aimbot::run(bone, height);
               // esp::render_hotbar(aimbot::getBestPlayerByFov());
            }
            else {
                espas = 0;
            }

          
        }
    }

    void aimbot(rust::BasePlayer* player, int bone, float height) {
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
        aimbot::draw();
        if (bounds.onscreen) {
            if (functions::get_key(KeyCode::Mouse1)) {
                aimbot::run(bone, height);
                // esp::render_hotbar(aimbot::getBestPlayerByFov());
            }
            else {
                espas = 0;
            }


        }
    }

    void FovCircle(float amount)
    {
        static int cases = 0;
        static float r = 1.00f, g = 0.00f, b = 1.00f;
        switch (cases)
        {
        case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
        case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
        case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
        case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
        default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
        }

        circle(functions::get_screen_width() / 2, functions::get_screen_height() / 2, amount, 500, Vector4(r, g, b, 255));

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
    void FullAuto() { variables::local_player->get_held_item()->set_automatic(true); 
    }
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
    void longeye() {  }
    void speed() {
        if (functions::get_key(KeyCode::Mouse4))
        {
            functions::set_timescale(2.5);
        }
        else
        {
            functions::set_timescale(1);
        }
    }
    void alldr()
    {
        variables::local_player->get_movement()->runar();
    }
    void Dysnic() {
        fovchange(1);
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
    }

    void noanimation()
    {
        variables::local_player->noanim();
    }
    void fovchange(float fov)
    {
       

        uintptr_t Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>();
        auto addressOne = *reinterpret_cast<uintptr_t*>(Vbase + 49260672);
        if (!addressOne)
            return;
        auto addressTwo = *reinterpret_cast<uintptr_t*>(addressOne + 0xb8);
        if (!addressTwo)
            return;
        // auto ass = *reinterpret_cast<uintptr_t*>(addressOne + addressTwo);
        memory::write<float>(addressOne + 0x18, fov);

       // variables::local_player->get_held_item()->fakefire();

    }
    void coolbow()
    {
        variables::local_player->get_held_item()->coolbow();
    }
    void daytime()    
    {
        uintptr_t Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>();
        auto addressOne = *reinterpret_cast<uintptr_t*>(Vbase + 49457088);
        if (!addressOne)
            return;
        auto addressTwo = *reinterpret_cast<uintptr_t*>(addressOne + 0xB8);
        if (!addressTwo)
            return;
       // auto ass = *reinterpret_cast<uintptr_t*>(addressOne + addressTwo);

        memory::write<float>(addressTwo + 0xC, 8.0);
        memory::write<float>(addressOne + 0x4, 0);
        memory::write<float>(addressOne + 0x0, 8.4f);
        memory::write<bool>(addressOne + 0x11, 0.0f);

        // variables::local_player->get_held_item()->fakefire();
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
        
           // variables::local_player->get_modelstate()->InteractiveDebug(true);
        
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
       // int mammo = variables::local_player->get_held_item()->maxammo();

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
        float asas;
        rust::BasePlayer* target_ply = aimbot::hotbar();
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
    

    void AmmoBarBetter()
    {
        int cammo = variables::local_player->get_held_item()->ammocount();
        int mammo = variables::local_player->get_held_item()->maxammo();


        int newcammo = (cammo / mammo) * (250);
        functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 400, 250, 10), Vector4(0, 0, 0, 0.9));
        functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 400, (newcammo), 10), Vector4(1, 0, 0, 1));


    }
    void DysnicBar()
    {

        int cammo = variables::local_player->get_held_item()->ammocount();




        float newcammo = (cammo / 30) * (250);

        functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 400, 250, 10), Vector4(0, 0, 0, 0.9));
        functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 400, (newcammo), 10), Vector4(1, 0, 0, 1));

        auto loco = variables::local_player;
        float desyncTime = (functions::get_time() - loco->get_lastSentTick()->TicksPerSec() - 0.03125 * 3);

       // std::cout << desyncTime;
        float red, green, blue;
        float Num = desyncTime;
      

      //  int newcammo = (cammo / mammo) * (250);
        //functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 400, 250, 10), Vector4(0, 0, 0, 0.9));
       // functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 400, (newcammo), 10), Vector4(1, 0, 0, 1));

       

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
            functions::outline_box(Vector2((variables::screen_width / 2) - 50,( variables::screen_height / 2) - 105), Vector2(100, 8), Vector4(0, 0, 0, 1));
            functions::fill_box(Vector4((variables::screen_width / 2) - 50,( variables::screen_height / 2) - 105,  (desyncTime), 8), Vector4(red, green, blue, 1));
        }

    }

    
    void fakeshotaa()
    {
        float asdd;
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
        float asd;
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
        Vector3 re_p = loco->get_eyes()->get_position() + loco->get_transform()->up() * (loco->get_eyes()->get_eyeOffset().y + loco->get_eyes()->get_viewoffset().y);
        // real eye pos

        Vector3 choice = Vector3(0, 0, 0);
        if (target_ply->get_mpv_bone() == NULL || target_ply == nullptr) return Vector3(0, 0, 0);

        if (functions::is_visible(re_p, target_ply->get_bone_position(target_ply->get_mpv_bone()))) {
            return Vector3(0, 0, 0);
        }

        float desyncTime = (functions::realtimeSinceStartup() - loco->get_lastSentTick()->TicksPerSec()) - 0.03125 * 3;
        float mm_max_eye = ((0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * loco->MaxVelocity()) - 0.05f);
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
        if (choice.IsZero())
            return Vector3(0, 0, 0);


        return choice;
    
    }
    float flyhack_forgiveness_interia = 10.0f;
    float flyhack_forgiveness = 1.5f;
    float flyhackDistanceVertical = 110.f;
    float flyhack_extrusion = 1.85f;
    float flyhackDistanceHorizontal = 110.f;
    float flyhackPauseTime{};


    float VFlyhack = 0.0f;
    float HFlyhack = 0.0f;
    float VMaxFlyhack = 0.0f;
    float HMaxFlyhack = 0.0f;


    void flyhackbar()
    {
        rust::BasePlayer* plly = variables::local_player;
        float flyhack_forgiveness_interia = 10.0f;
        float flyhack_forgiveness = 1.5f;
        float flyhack_extrusion = 1.85f;
        bool flag = false;
        bool isInAir = false;
        Vector3 vector = (plly->get_lastSentTick()->get_position() + plly->get_transform()->get_position()) * 0.5f;
        flyhackPauseTime = functions::mathf_max(0.f, flyhackPauseTime - functions::get_deltaTime());

        if (plly) {
            float radius = plly->get_radius();
            float height = plly->get_height(false);

            Vector3 vector2 = vector + Vector3(0.0f, radius - flyhack_extrusion, 0.0f);
            Vector3 vector3 = vector + Vector3(0.0f, height - radius, 0.0f);
            float radius2 = radius - 0.05f;

            isInAir = plly->get_movement()->flying();

            if (isInAir)
            {
                Vector3 vector4 = (plly->get_lastSentTick()->get_position() - plly->get_transform()->get_position());

                float num2 = functions::mathf_abs(vector4.y);
                float num3  = sqrt((vector4.x * vector4.x) + (vector4.y * vector4.y) + (vector4.z + vector4.z));

              if (vector4.y >= 0.0f)
                {
                    flyhackDistanceVertical += vector4.y;
                    flag = true;
                }
                if (num2 < num3)
                {
                    flyhackDistanceHorizontal += num3;
                    flag = true;
                }
                if (flag)
                {
                    float num5 = functions::mathf_max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
                    float num6 = (plly->get_jumpheight() + num5);
                    if (flyhackDistanceVertical > num6) {
                        //std::cout << OBFUSCATE_STR("Got Flyhacked") << std::endl;
                    }
                    float num7 = functions::mathf_max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
                    float num8 = (5.f + num7);
                    if (flyhackDistanceHorizontal > num8) {
                        //std::cout << OBFUSCATE_STR("Got Flyhacked") << std::endl;
                    }
                }
            }




            else {
                flyhackDistanceHorizontal = 0.f;
                flyhackDistanceVertical = 0.f;
            }
            float num5 = functions::mathf_max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
            float num6 = ((plly->get_jumpheight() + num5) * 3);
            //num6 = 13.0f;
            VMaxFlyhack = num6;//(num6 * 2) - 2;
            if (flyhackDistanceVertical <= (num6)) {
                VFlyhack = flyhackDistanceVertical;
            }
            if (VFlyhack >= VMaxFlyhack)
                VFlyhack = VMaxFlyhack;
            float num7 = functions::mathf_max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
            float num8 = ((5.f + num7) * 3);
            //num8 = 26.0f;
            HMaxFlyhack = num8;//(num8 * 2) - 2; //- (VFlyhack * 2);//(num6 * 2);
            if (flyhackDistanceHorizontal <= (num8)) {
                HFlyhack = flyhackDistanceHorizontal;// +(VFlyhack * 2);
            }
            if (HFlyhack >= HMaxFlyhack)
                HFlyhack = HMaxFlyhack;

        }
        else {
            flyhackDistanceHorizontal = 0.f;
            flyhackDistanceVertical = 0.f;
        }
        if (!isInAir)
        {
            flyhackDistanceHorizontal = 0.f;
            flyhackDistanceVertical = 0.f;
        }


        float v, h;
        v = VFlyhack / (VMaxFlyhack - 1.0f);
        h = HFlyhack / (HMaxFlyhack - 1.0f);

        static int cases = 0;
        static float r = 1.00f, g = 0.00f, b = 1.00f;
        switch (cases)
        {
        case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
        case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
        case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
        case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
        default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
        }
        functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 870, 250, 10), Vector4(0, 0, 0, 0.9));
        functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 850, 250, 10), Vector4(0, 0, 0, 0.9));
        functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 870, 250 * (v >= 1.0f ? 1.0f : v), 10), Vector4(r, g,b, 1));
        functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 850, 250 * (h >= 1.0f ? 1.0f : h), 10), Vector4(r, g, b, 1));




    }




    
   }
   
  
       

    
    




 

   
    
  