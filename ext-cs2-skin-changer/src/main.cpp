#include "menu.h"
#include "SDK/entity/CWeaponServices.h"
#include "SDK/weapon/C_EconEntity.h"
#include "SDK/CEconItemAttributeManager.h"

int main()
{
    mem.Write<uint16_t>(Sigs::RegenerateWeaponSkins + 0x52, Offsets::m_AttributeManager + Offsets::m_Item + Offsets::m_AttributeList + Offsets::m_Attributes);

    
    
    while (true)
    {
        Sleep(5);

        const uintptr_t localPlayer = GetLocalPlayer();
        const uintptr_t pWeaponServices = mem.Read<uintptr_t>(localPlayer + Offsets::m_pWeaponServices);

        bool ShouldUpdate = false;

        const std::vector<uintptr_t> weapons = GetWeapons(localPlayer);
        for (const uintptr_t& weapon : weapons)
        {
            const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
            if (mem.Read<uint32_t>(item + Offsets::m_iItemIDHigh) == -1)//already applied
                continue;

            mem.Write<uint32_t>(item + Offsets::m_iItemIDHigh, -1);

            SkinInfo_t skin = GetSkin(item);
            if (!skin.Paint) 
                continue;

            mem.Write<uint32_t>(weapon + Offsets::m_nFallbackPaintKit, skin.Paint);

            const uint64_t mask = skin.bUsesOldModel + 1;

            const uintptr_t hudWeapon = GetHudWeapon(weapon);
            SetMeshMask(weapon, mask);
            SetMeshMask(hudWeapon, mask);

            //mem.Write<ItemCustomName_t>(item + Offsets::m_szCustomNameOverride, ItemCustomName_t("this is a custom name"));

            econItemAttributeManager.Create(item, skin);

            ShouldUpdate = true;
        }

        if (ShouldUpdate)
            UpdateWeapons(weapons);
    }
    
    return 0;
}