#include "aimbot.h"

namespace aimbot {


    Vector3 sim_prediction(const Vector3& from, rust::BasePlayer* who, unsigned int bone, float initial_velocity, float gravity_modifier, float drag)
    {
        Vector3 aimpoint = who->get_bone_position(bone);
        float distance = Math::Calc3D_Dist(from, aimpoint);
        auto held_weapon = variables::local_player->get_held_item();
        if (!held_weapon)
            return aimpoint;

        if (!held_weapon->is_gun())
            return aimpoint;

        auto weapon_data = held_weapon->get_weapon_data();

        float m_flBulletSpeed = initial_velocity;
        float bullet_gravity = gravity_modifier;
        float bullet_time = distance / m_flBulletSpeed;
        float m_fDrag = drag;

        const float m_flTimeStep = (0.015625f);
        float YTravelled{}, m_flYSpeed{}, m_flBulletTime{}, m_flDivider{};



        for (float distance_to_travel = 0.f; distance_to_travel < distance;)
        {
            float speed_modifier = 1.f - m_flTimeStep * m_fDrag;
            m_flBulletSpeed *= speed_modifier;

            if (m_flBulletSpeed <= 0.f || m_flBulletSpeed >= 10000.f || YTravelled >= 10000.f || YTravelled < 0.f)
                break;


            if (m_flBulletTime > 8.f)
                break;

            m_flYSpeed += ((9.81f) * bullet_gravity) * m_flTimeStep;
            m_flYSpeed *= speed_modifier;

            distance_to_travel += m_flBulletSpeed * m_flTimeStep;
            YTravelled += m_flYSpeed * m_flTimeStep;
            m_flBulletTime += m_flTimeStep;
        }

        Vector3 velocity = who->get_model()->get_velocity() * 0.75f;
        //if (velocity.y > 0.f)
        //    velocity.y /= 3.25f;

        aimpoint.y += YTravelled;
        aimpoint += velocity * m_flBulletTime;

        return aimpoint;
    }

    rust::BasePlayer* getBestPlayerByFov() {
        rust::BasePlayer* bestEnt = nullptr;
        float closest = FLT_MAX;
        for (int i = 0; i < variables::player_list.size(); i++) {
            auto model = variables::player_list[i]->get_model();
          
            if (variables::player_list[i] == variables::local_player)
                continue;
            if (variables::player_list[i]->get_life_state())
                continue;
            if (variables::player_list[i]->is_sleeping())
                continue;
            Vector3 headPos = variables::player_list[i]->get_model()->get_position();
            Vector2 head2D;
            if (functions::WorldToScreen(headPos, head2D) == false) continue;
            float length = sqrt(pow((variables::screen_width / 2) - head2D.x, 2) + pow((variables::screen_height / 2) - head2D.y, 2));

           
            if (length < closest) {
                closest = length;
                bestEnt = variables::player_list[i];
            }
        }
        return bestEnt;
    }

    int getClosestBoneToCrosshair(rust::BasePlayer* player) {
        int bestBone = 0;
        bool visable;
        float closest = FLT_MAX;
        for (int i = 0; i < rust::valid_bones.size(); i++) {
            Vector3 pos3d = player->get_bone_position(rust::valid_bones[i]);
            visable = false;
            if (visable && !variables::local_player->is_visible(pos3d)) continue;
            Vector2 pos;
            if (!functions::WorldToScreen(pos3d, pos)) continue;
            float length = sqrt(pow((variables::screen_width / 2) - pos.x, 2) + pow((variables::screen_height / 2) - pos.y, 2));
            if (length < closest) {
                closest = length;
               
                bestBone = rust::valid_bones[i];
            }
        }
        return bestBone;
    }

    void run(bool forceHead) {
        rust::BasePlayer* player = getBestPlayerByFov();
        int bone;
        if (forceHead)
            bone = BoneList::neck;
        else 
        {
            bone = getClosestBoneToCrosshair(player);
            bone = bone ? bone : BoneList::neck;
        }
          


        variables::local_player->get_input()->set_view_angles(Math::CalcAngle(variables::local_player->get_bone_position(BoneList::neck), player->get_bone_position(bone)));


    }
}
