#pragma once
#include "../hinclude.h"
#include "../performanceui.h"
#include "../ddraw.h"
#include "../baseplayer.h"
#include "../basemelee.h"
#include "../fastloot.h"
#include "../Exploit.h"
namespace hooks
{
	uintptr_t fixcullingorig;
	void fixculling(rust::BasePlayer* baseplayer, float dist, bool visibility = false)
	{
		auto orig_fixculling = reinterpret_cast<void(*)(rust::BasePlayer*, float, bool)>(fixcullingorig);
		//if (chams)
		return SpoofReturn(orig_fixculling, baseplayer, 300.f, true);

		//return SpoofReturn(orig_fixculling, baseplayer, dist, visibility);
	}
	namespace orig {
		//static auto blocksprint = reinterpret_cast<void (*)(rust::BasePlayer*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("BasePlayer"), ("BlockSprint"), 1, xorstr_(""), xorstr_(""))));
	}

	//void hk_blocksprint(rust::BasePlayer* player, float duration) {
	//	if (!options::alwaysShoot)
	//		return orig::blocksprint(player, duration);

	//	return;
	//}

	__forceinline bool init()
	{
		PerformanceUI::performaceui_hook = *reinterpret_cast<uintptr_t*>(il2cpp::method(xorstr_("PerformanceUI"), xorstr_("Update"), 0, xorstr_("Facepunch")));
		il2cpp::hook(&PerformanceUI::hk_performance_ui_update, xorstr_("PerformanceUI"), xorstr_("Update"), 0, xorstr_("Facepunch"));
		il2cpp::hook(&PerformanceUI::ongui_hook, xorstr_("DevControls"), xorstr_("OnGUI"), 0, xorstr_(""));
		il2cpp::hook(&ddraw::ongui_hook, xorstr_("DDraw"), xorstr_("OnGUI"), 0, xorstr_("UnityEngine"));

		basemelee::process_attack_address = il2cpp::hook_virtual_function(xorstr_("BaseMelee"), xorstr_("ProcessAttack"), &basemelee::process_attack_hook, 1);
	//	playerwalkmovement::playwalkmovement_clientinput_address = il2cpp::hook_virtual_function(xorstr_("PlayerWalkMovement"), xorstr_("ClientInput"), &playerwalkmovement::client_input_hook, 2);
		//orig::blocksprint = reinterpret_cast<void (*)(rust::BasePlayer*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(xorstr_("BasePlayer"), xorstr_("BlockSprint"), 1, xorstr_(""), xorstr_(""))));
		//il2cpp::hook_virtual_function(xorstr_("BasePlayer"), xorstr_("BlockSprint"), &hk_blocksprint, -1);
		fixcullingorig = il2cpp::hook_virtual_function(xorstr_("BasePlayer"), xorstr_("VisUpdateUsingCulling"), &fixculling, 2);
		baseplayer::client_input_address = il2cpp::hook_virtual_function(xorstr_("BasePlayer"), xorstr_("ClientInput"), &baseplayer::client_input_hook, 1);
		return true;
	}
}