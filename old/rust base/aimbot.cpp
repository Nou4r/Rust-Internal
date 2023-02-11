#include "aimbot.h"

namespace aimbot {
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
