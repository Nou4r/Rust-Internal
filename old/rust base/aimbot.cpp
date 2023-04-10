#include "aimbot.h"

namespace aimbot {


    rust::BasePlayer* getBestPlayerByFovAimbot(float amount) {
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

            float height = 250;

            if (length < closest && length <= 300) {
                closest = length;
                bestEnt = variables::player_list[i];
            }
        }
        return bestEnt;
    }

  
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

            float height = 250;
           
            if (length < closest && length <= 300) {
                closest = length;
                bestEnt = variables::player_list[i];
            }
        }
        return bestEnt;
    }

    rust::BasePlayer* hotbar() {
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

            float height = 250;

            if (length < closest && length <= 50) {
                closest = length;
                bestEnt = variables::player_list[i];
            }
        }
        return bestEnt;
    }

    int getClosestBoneToCrosshair(rust::BasePlayer* player, float height) {
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
            if (length < closest && length <= 300) {
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

	Vector3 find_mpv_bone(float height) {
		
		auto target = aimbot::getBestPlayerByFov();
		Vector3 bones = target->get_bone_position(BoneList::head);

		
		return bones;
	}


    Vector3 Prediction(BoneList bone)
    {
        bone;
        rust::BasePlayer* et = aimbot::getBestPlayerByFov();
        rust::BasePlayer* LocalPlayer = variables::local_player;
        float Distance = Math::Calc3D_Dist(variables::local_position, et->get_model()->get_position());
        Vector3 Pos = variables::local_position;
        Vector3 Enemy = et->get_bone_position(bone);

        float BulletSpeed = LocalPlayer->get_held_item()->get_bullet_velocity();
        float BulletGravity = LocalPlayer->get_held_item()->get_bullet_gravity(); // 2.5 m/s
        float Bullet = (BulletSpeed / BulletGravity);
        ///time = distance / speed
        float time = (Distance / BulletSpeed);
        float enemyveloctiy = et->MaxVelocity();
        Vector2 x = Math::CalcAngle(Pos, Enemy);

        if (bone == BoneList::neck)
        {
            return Vector3{x.x, x.y + time, Enemy.z};
        }
        

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
    Vector3 sim_prediction(const Vector3& from, int bone, float initial_velocity, float gravity_modifier, float drag, float height)
    {
        if (variables::local_player->get_held_item()->is_melee())
        {
            return Vector3{ 0,0,0 };
        }
        
        rust::BasePlayer* who = getBestPlayerByFovAimbot(height);

        Vector3 aimpoint = who->get_bone_position(48);
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

    void draw()
    {
        Vector3 Point = sim_prediction(variables::local_player->get_bone_position(BoneList::eyeTranform), BoneList::neck, variables::local_player->get_held_item()->get_weapon_data().initial_velocity, variables::local_player->get_held_item()->get_weapon_data().gravity_modifier, variables::local_player->get_held_item()->get_weapon_data().drag, 250);
        Vector3 p = aimbot::getBestPlayerByFov()->get_bone_position(BoneList::neck);
        functions::line(Vector2(Point.x, Point.y), Vector2( p.x, p.y), Vector4(1, 0, 0, 1));
    }

	void run(bool forceHead, float height) {
        rust::BasePlayer* player = getBestPlayerByFovAimbot(height);
        Vector3 Final = sim_prediction(variables::local_player->get_bone_position(BoneList::eyeTranform), BoneList::neck, variables::local_player->get_held_item()->get_weapon_data().initial_velocity, variables::local_player->get_held_item()->get_weapon_data().gravity_modifier, variables::local_player->get_held_item()->get_weapon_data().drag, height);


      //  Vector3 sim_prediction(const Vector3 & from, rust::BasePlayer * who, unsigned int bone, float initial_velocity, float gravity_modifier, float drag)
		int bone;
		if (forceHead)
			bone = BoneList::neck;
		else
		{
			bone = getClosestBoneToCrosshair(player, height);
			bone = bone ? bone : BoneList::neck;
		}

        if (player->get_bone_position(BoneList::neck) == Vector3{ 0,0,0 })
            return;
       // rust::Attack


		variables::local_player->get_input()->set_view_angles(Math::CalcAngle(variables::local_player->get_bone_position(BoneList::eyeTranform), Final));
	}
}
