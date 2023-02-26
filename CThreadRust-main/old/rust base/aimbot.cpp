#include "aimbot.h"

namespace aimbot {


  
    rust::BasePlayer* getBestPlayerByFov() {
        rust::BasePlayer* bestEnt = nullptr;
        float closest = 999999999;
       
        for (int i = 0; i < variables::player_list.size(); i++) {
            auto model = variables::player_list[i]->get_model();

            
            auto distancea = Math::Calc3D_Dist(variables::local_position, variables::player_list[i]->get_model()->get_position());
            if (distancea > 300) continue;
                
            if (variables::player_list[i] == variables::local_player)
                continue;
            if (variables::player_list[i]->get_life_state())
                continue;
            if (variables::player_list[i]->is_sleeping())
                continue;
            Vector3 headPos = variables::player_list[i]->get_model()->get_position();
            Vector2 head2D;
            if (headPos == Vector3{ 0,0,0 }) continue;
            if (functions::WorldToScreen(headPos, head2D) == false) continue;
            float length = sqrt(pow((variables::screen_width / 2) - head2D.x, 2) + pow((variables::screen_height / 2) - head2D.y, 2));

           
           
            if (length < closest && length <= 250) {
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
            if (length < closest && length <= 250) {
                closest = length;
               
                bestBone = rust::valid_bones[i];
            }
        }
        return bestBone;
    }



   


	
	double get_bullet_drop(double height, double aaaa, float speed, float gravity) {
		double pitch = std::atan2(height, aaaa);
		double vel_double = speed * std::cos(pitch);
		double t = aaaa / vel_double;
		double y = (0.4905f * gravity * t * t);
		return y * 10;
	}

	Vector3 find_mpv_bone() {
		
		auto target = aimbot::getBestPlayerByFov();
		Vector3 bones = target->get_bone_position(BoneList::head);

		
		return bones;
	}

  
	Vector3 MyPhysics30CourceMightPayOff(float height) {

        /*
        Physics Formulas

        v = veloctity;
        d = displament;
        t = time;
        g = gravity thik its 2.5 m/s^2

        V = d/t 
        
        */
        
        rust::BasePlayer* TargetPlayer = aimbot::getBestPlayerByFov();

        rust::BasePlayer* LocalPlayer = variables::local_player;
        Vector3 Speed;
        
        float BulletSpeed = LocalPlayer->get_held_item()->get_bullet_velocity();

        float BulletGravity = LocalPlayer->get_held_item()->get_bullet_gravity(); // 2.5 m/s

        Vector3 FinalPos;

        

      
        if (TargetPlayer)
        {
            float Distance  = Math::Calc3D_Dist(variables::local_position, TargetPlayer->get_model()->get_position());


            FinalPos = { FinalPos.x = TargetPlayer->get_bone_position(BoneList::neck).x,  TargetPlayer->get_bone_position(BoneList::neck).y, FinalPos.z = TargetPlayer->get_bone_position(BoneList::neck).z };

            if (functions::ws2s(LocalPlayer->get_held_item()->get_name()) == "pistol.nailgun")
            {
                Vector3 OurHeight = variables::local_position;
                Vector3 Targetheight = TargetPlayer->get_model()->get_position();
                Vector3 FinalHeight = OurHeight - Targetheight;
                FinalPos = { FinalPos.x = TargetPlayer->get_bone_position(BoneList::neck).x,  TargetPlayer->get_bone_position(BoneList::neck).y + height, FinalPos.z = TargetPlayer->get_bone_position(BoneList::neck).z };
                //Connect 104.129.132.45:28046
            }
           
        }
        return FinalPos;

	}
   
  
	void run(bool forceHead, float height) {

		rust::BasePlayer* player = getBestPlayerByFov();
		int bone;
		if (forceHead)
			bone = BoneList::neck;
		else
		{
			bone = getClosestBoneToCrosshair(player);
			bone = bone ? bone : BoneList::neck;
		}

        if (player->get_bone_position(BoneList::neck) == Vector3{ 0,0,0 })
            return;



		variables::local_player->get_input()->set_view_angles(Math::CalcAngle(variables::local_player->get_bone_position(BoneList::neck), player->get_bone_position(BoneList::neck)));
	}
}
