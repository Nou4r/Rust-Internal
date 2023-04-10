#pragma once
#include "hinclude.h"

typedef void(__fastcall* FatLoot)(uintptr_t sourceContainer, DWORD64 action);
inline FatLoot FatLootoriginal_mode;



void __fastcall FastLoot(uintptr_t sourceContainer, DWORD64 action)
{
		1.f;
	return FatLootoriginal_mode(sourceContainer, action);
}

typedef void(__fastcall* MagicBullet)(rust::BaseEntity* a1, rust::Item* a2);
inline MagicBullet MagicBulletOriginal;
