#include "menu.h"
#include "window/window.hpp"

#include "../ext/offsets.h"
#include "../ext/sigs.h"

#include "SDK/entity/dwEntityListManager.h"
#include "SDK/entity/C_CS2HudModelArms.h"

#include "../ext/wcl.h"

SkinInfo_t GetActiveSkin(const WeaponsEnum& def)
{
    switch (def)
    {
    case WeaponsEnum::UspS:
        return SkinInfo_t{ 653, true };//504
    case WeaponsEnum::Glock:
        return SkinInfo_t{ 1208, false };
    case WeaponsEnum::Awp:
        return SkinInfo_t{ 344, true };
    case WeaponsEnum::Ak47:
        return SkinInfo_t{ 801, true };
    case WeaponsEnum::M4A1Silencer:
        return SkinInfo_t{ 644, true };
    case WeaponsEnum::Ssg08:
        return SkinInfo_t{ 222, true };
    }

	return SkinInfo_t{};
}

void OnMelee(const uintptr_t& pKnife)
{

}

void OnWeapon(const uintptr_t& pWeapon)
{
    const uintptr_t hudWeapon = GetHudWeapon(pWeapon);
    const uintptr_t item = pWeapon + Offsets::m_AttributeManager + Offsets::m_Item;

	SkinInfo_t activeSkin = GetActiveSkin(static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex)));

    if (mem->Read<int32_t>(pWeapon + Offsets::m_nFallbackPaintKit) != activeSkin.Paint)
    {
        SetMeshMask(pWeapon, 1 + activeSkin.bUsesOldModel);
        SetMeshMask(hudWeapon, 1 + activeSkin.bUsesOldModel);

        mem->Write<uint32_t>(item + Offsets::m_iItemIDHigh, -1);
        mem->Write<int32_t>(pWeapon + Offsets::m_nFallbackPaintKit, activeSkin.Paint);
    }

    UpdateWeapon(pWeapon);
}

int main()
{
    while (true)
    {
        Sleep(500);

        const uintptr_t localPlayer = GetLocalPlayer();
        const uintptr_t weapon = mem->Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);
        if (!localPlayer || !weapon)
            continue;

        if (IsMeleeWeapon(weapon))
        {
            OnMelee(weapon);
        }
        else
        {
            OnWeapon(weapon);
        }
    }

    return 0;

    ////mem->Write<uint32_t>(localPlayer + Offsets::m_EconGloves + Offsets::m_iItemIDHigh, -1);
    ////mem->Write<uint16_t>(localPlayer + Offsets::m_EconGloves + Offsets::m_iItemDefinitionIndex, 5030);
    ////AddAttribute(localPlayer + Offsets::m_EconGloves, Attributes::Skin, 10048.f);
    ////
    ////mem->Write<bool>(localPlayer + Offsets::m_EconGloves + Offsets::m_bInitialized, true);
    ////mem->Write<bool>(localPlayer + Offsets::m_bNeedToReApplyGloves, true);

    //std::string model = "weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl";
    //std::string model = "weapons/models/knife/knife_m9/weapon_knife_m9.vmdl";
    //
    //SetModel(weapon, model);
    //SetModel(hudWeapon, model);
    //
   // return 0;
   //
   // static const std::map<uint16_t, uint64_t> m_subclassIdMap = {
   //         {500, 3933374535},
   //         {503, 3787235507},
   //         {505, 4046390180},
   //         {506, 2047704618},
   //         {507, 1731408398},
   //         {508, 1638561588},
   //         {509, 2282479884},
   //         {512, 3412259219},
   //         {514, 2511498851},
   //         {515, 1353709123},
   //         {516, 4269888884},
   //         {517, 1105782941},
   //         {518, 275962944},
   //         {519, 1338637359},
   //         {520, 3230445913},
   //         {521, 3206681373},
   //         {522, 2595277776},
   //         {523, 4029975521},
   //         {524, 2463111489},
   //         {525, 365028728},
   //         {526, 3845286452},
   // };
   //
   // const uint16_t definition = 508;
   // const uint64_t targetSubclassID = m_subclassIdMap.at(definition);
   //
   // mem->Write<uint16_t>(item + Offsets::m_iItemDefinitionIndex, definition);
   // mem->Write<uint64_t>(weapon + Offsets::m_nSubclassID, targetSubclassID);
   //
   // UpdateSubclass(weapon);
   //
   // return 0;


    //


    //UpdateSubclass(weapon);

    //UpdateModel(weapon);
    //UpdateModel(hudWeapon);

    //SetModel(weapon, model);
    //SetModel(hudWeapon, model);
}