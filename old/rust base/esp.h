#pragma once
#include "includes.hpp"
#include "game/classes.hpp"

namespace esp
{
	void render_aimbot(rust::BasePlayer* player, int bone, float height, KeyCode input);
	void render_shitaim(rust::BasePlayer* player, BoneList bone, float height);
	void ShootWhileInAir();
	void render_weapon(rust::BasePlayer* player);
	void render_Health(rust::BasePlayer* player);
	void render_distance(rust::BasePlayer* player);
	void render_name(rust::BasePlayer* player);
	void coolbow();
	void speed();
	void flyhackbar();
	void alldr();
	void render_box(rust::BasePlayer* player);
	void do_chams(rust::BasePlayer* player, float vischamr, float vischamg, float vischamb, float nonchamr, float nonchamg, float nonchamb);
	void SafeAdmin();
	void NoRecoil(float recoil);
	void nosway();
	void instaEoka();
	void ammobar();
	void fakelag();
	void fakeshotaa();
	void TeamInstantRevive();
	void fakeshot();
	void hotbar();
	void MoonJump();
	void FullAuto();
	void WalkOnWalter();
	void nocollision();
	void Backtrack();
	void DysnicBar();
	void fakeshot();
	void do_handchams();


	void ExterndMelee();
	void fastbow();
	void longeye();
	void Dysnic();
	void aimbot(rust::BasePlayer* player, int bone, float height);
	void fastswitch();
	void bones(rust::BasePlayer* player);
	void RaidFire();
	void omean();
	void render_aimbot(rust::BasePlayer* player, int bone, float height, KeyCode input);
	void spiderman();
	void bullettraceshitty(rust::BasePlayer* ent, float x, float y, float z);
	void noanimation();
	void BrightNIght();
	void fovchange(float fov);
	void fatbullet();
	void daytime();
	void shotgunnospread();
	void spinbot();
	void nicesky(float x, float y);
	void GroundFucker();
	void ladder();
	void radar();
	void FovCircle(float amount);
	void instatRevive();
	void omniSprint();
	void autoreload();
	void render_hotbar(rust::BasePlayer* player, float height, float length);
	Vector3 FindManipulationAngle();
}

