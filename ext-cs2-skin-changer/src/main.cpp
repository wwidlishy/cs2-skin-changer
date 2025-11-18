#include "menu.h"
#include "skins.h"
#include "window/window.hpp"

void dostuff()
{
    const uintptr_t localPlayer = GetLocalPlayer();
    const uintptr_t CGameEntitySystem = mem->Read<uintptr_t>(client + Offsets::dwGameEntitySystem);

    while (true)
    {
        const uintptr_t weapon = mem->Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);
        const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;

		std::cout << IsAttributeUpdated(item, AttributeDefinitionIndex::Paint, (float)mem->Read<uint32_t>(weapon + Offsets::m_nFallbackPaintKit)) << std::endl;
		//std::cout << std::hex << weapon << std::endl;
        const uint32_t clientWeaponIndex = GetClientEntIndex(CGameEntitySystem, weapon);
        if (!clientWeaponIndex || clientWeaponIndex == 1)
            continue;
		//std::cout << std::dec << GetClientEnt(CGameEntitySystem, clientWeaponIndex).pad_000C << std::endl;
        SetPostDataUpdateId(CGameEntitySystem, clientWeaponIndex, PostDataUpdateIds::Update);
    }
}

int main()
{
    std::thread dst(dostuff);
    dst.detach();

    skindb->Dump();

    overlay::Setup();

    std::thread MenuThread(MenuThreadFunc);
	MenuThread.detach();

	std::cout << "Discord Overlay Hooked!" << std::endl;
	std::cout << "CS2 Skin Changer Loaded!" << std::endl;

    while (true)
    { 
		Sleep(1);

        const uintptr_t localPlayer = GetLocalPlayer();
        const uintptr_t weapon = mem->Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);
        const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
        CurrentWeaponDef = static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex));
        if (!localPlayer || !weapon || !item || !CurrentWeaponDef)
            continue;

        //std::cout << std::hex << weapon << std::endl;
        //std::cout << std::hex << localPlayer + Offsets::m_EconGloves << std::endl;
		////OnAgent(localPlayer);
		//OnGloves(localPlayer + Offsets::m_EconGloves);

        if (IsMeleeWeapon(weapon) && bKnife)
            OnMelee(weapon);
        else if(bSkin)
            OnWeapon(weapon);

        if (ShouldUpdateWeapon)
            UpdateWeapon(weapon);
            
    }

    overlay::CloseOverlay();

    return 0;
}