#include "menu.h"
#include "SDK/weapon/C_EconEntity.h"
#include "SDK/entity/C_CS2HudModelArms.h"
#include "SDK/CEconItemAttributeManager.h"

#define ChunkEntityCount 512
#define ChunkLen 3
#define MaxEntities ChunkEntityCount * ChunkLen

#define ChunkEntry 0x10
#define EntityIdentitySize 0x70

struct FallbackEcon_t
{
    uint32_t m_nFallbackPaintKit;
    uint32_t m_nFallbackSeed;
    float m_flFallbackWear;
    uint32_t m_nFallbackStatTrak;
};

struct ItemCustomName_t
{
	char CustomNameOverride[160];
};

struct ItemViewData_t
{
    int32_t m_iEntityQuality;
    uint32_t m_iEntityLevel;
    uint64_t m_iItemID;
    uint32_t m_iItemIDHigh;
    uint32_t m_iItemIDLow;
    uint32_t m_iAccountID;
};

class SkinInfo
{
public:
	FallbackEcon_t fallbackEcon;
    ItemViewData_t fallbackItemView;
    ItemCustomName_t fallbackItemViewName;

    inline bool IsValid() const { return fallbackEcon.m_nFallbackPaintKit != 0; }
    inline bool CustomNameAdded() { return fallbackItemViewName.CustomNameOverride[0] != '\0'; }

    inline void SetSkin(const uintptr_t weapon)
    {
        if(!IsValid())
			return;

        mem.Write<FallbackEcon_t>(weapon + Offsets::m_nFallbackPaintKit, fallbackEcon);
        mem.Write<ItemViewData_t>(weapon + Offsets::m_AttributeManager + Offsets::m_Item + Offsets::m_iEntityQuality, fallbackItemView);

        if (!CustomNameAdded())
            return;

        mem.Write<ItemCustomName_t>(weapon + Offsets::m_AttributeManager + Offsets::m_Item + Offsets::m_szCustomNameOverride, fallbackItemViewName);
    }
};

int main()
{

   // while (true)
   // {
   //     Sleep(1);
   //
   //     //if (GetAsyncKeyState(VK_HOME) & 1)
   //     //{
   //     //    mem.Write<uint32_t>(0x537BCCCA000 + 0x154, -1);
   //     //    std::cout << "triggered update\n";
   //     //    //Sleep(200);
   //     //}
   //
   //     
   //     //std::cout << std::hex << mem.Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon) + 0x1680 - 0x8 << std::endl;
   //
   //     //if (GetAsyncKeyState(VK_DELETE) & 1)
   //     {
   //         const uintptr_t localPlayer = GetLocalPlayer();
   //         const uintptr_t pWeaponServices = mem.Read<uintptr_t>(localPlayer + Offsets::m_pWeaponServices);
   //         const uintptr_t weapon = mem.Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);
   //         const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
   //         const uintptr_t hudweapon = GetHudWeapon(weapon);
   //
   //         //SetMeshMask(hudweapon, 2);
   //
   //         const CPtrGameVector attribute = mem.Read<CPtrGameVector>(item + Offsets::m_AttributeList + Offsets::m_Attributes);
   //         if (attribute.size)
   //             continue;
   //
   //         SkinInfo_t skin;
   //         skin.Paint = 653;
   //
   //         econItemAttributeManager.Create(item, skin);
   //         
   //
   //         //std::cout << std::hex << item + Offsets::m_NetworkedDynamicAttributes + Offsets::m_Attributes << std::endl;
   //         //std::cout << std::hex << attribute.size << std::endl;
   //         //std::cout << std::hex << attribute.ptr << std::endl;
   //         //mem.Write<float>(attribute.ptr + 0x34, 653);
   //
   //         //mem.Write<uint32_t>(item + Offsets::m_iItemIDHigh, -1);
   //
   //         //FallbackEcon_t fallbackEcon;
   //         //fallbackEcon.m_nFallbackPaintKit = 653;
   //         //fallbackEcon.m_nFallbackStatTrak = 1337;
   //         //fallbackEcon.m_flFallbackWear = 0.01f;
   //         //mem.Write<FallbackEcon_t>(weapon + Offsets::m_nFallbackPaintKit, fallbackEcon);
   //         //std::cout << "meshmask update\n";
   //         //Sleep(200);
   //     }
   // }
   //
   // return 0;
   //
   const uintptr_t localPlayer = GetLocalPlayer();
   const uintptr_t pWeaponServices = mem.Read<uintptr_t>(localPlayer + Offsets::m_pWeaponServices);
   // 
   // //const uintptr_t weapon = GetEntityByHandle(mem.Read<uint32_t>(pWeaponServices + Offsets::m_hActiveWeapon));
   // //
   // //SetMeshMask(weapon, 1);
   //
   // struct ms
   // {
   //     char c[0x300];
   // };
   //
   // const uintptr_t pmb = mem.Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon) + 0x13E0 + 0x200;
   // const uintptr_t mb = mem.Read<uintptr_t>(pmb);
   // mem.Write<uintptr_t>(pmb, NULL);
   //
   // mem.Write<ms>(mb, ms());
   //
   // std::cout << std::hex << mb << std::endl;
   //
   // return 0;
   //
   //  std::cout << std::hex << localPlayer << std::endl;
   //  std::cout << std::hex << mem.Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon) << std::endl;
   //  std::cout << std::hex << GetHudWeapon(mem.Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon)) << std::endl;
   // std::cout << std::hex << GetHudArms() << std::endl;
   // std::cout << std::hex << mem.Read<uintptr_t>(GetHudArms() + 0x330) << std::endl;
   // 
   //
   // //const auto& armsBase = GetHudArms();
   // //const auto& armsNode = mem.Read<uintptr_t>(armsBase + Offsets::m_pGameSceneNode);
   // //for (uintptr_t viewModel = mem.Read<uintptr_t>(armsNode + Offsets::m_pChild); viewModel; viewModel = mem.Read<uintptr_t>(viewModel + Offsets::m_pNextSibling))
   // //{
   // //    mem.Write<uintptr_t>(viewModel + Offsets::m_pNextSibling, NULL);
   // //}
   //
   //return 0;

    while (true)
    {
        //for (const uintptr_t& weapon : GetWeapons(localPlayer))
        {
           const uintptr_t weapon = GetEntityByHandle(mem.Read<uint32_t>(pWeaponServices + Offsets::m_hActiveWeapon));

            if (mem.Read<uint8_t>(weapon + Offsets::m_nFallbackPaintKit))
                continue;

            //const uintptr_t hudWeapon = GetActiveHudWeapon();
            //SetMeshMask(hudWeapon, 2);
            //SetMeshMask(weapon, 2);

            const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
            mem.Write<uint32_t>(item + Offsets::m_iItemIDHigh, -1);

            FallbackEcon_t fallbackEcon;
            fallbackEcon.m_nFallbackPaintKit = 653;
            fallbackEcon.m_nFallbackStatTrak = 1337;
            fallbackEcon.m_flFallbackWear = 0.01f;
            mem.Write<FallbackEcon_t>(weapon + Offsets::m_nFallbackPaintKit, fallbackEcon);

            //mem.Write<ItemCustomName_t>(item + Offsets::m_szCustomNameOverride, ItemCustomName_t("this is a custom name"));

            //if (shouldUpdate)
            //    mem.CallThread(sigs);
        }
    }

    return 0;
}

//const uintptr_t CGameEntitySystem = fastmem.Read<uintptr_t>(client + Offsets::dwGameEntitySystem);
    //
    //const std::array<uintptr_t, ChunkLen> Chunks = fastmem.Read<std::array<uintptr_t, ChunkLen>>(CGameEntitySystem + 0x10);
    //std::array<uintptr_t, MaxEntities> EntityIdenties;
    //
    //for (int i = 0; i < MaxEntities; i++)
    //{
    //    const uintptr_t& chunk = Chunks[i >> 9];
    //    EntityIdenties[i] = chunk + EntityIdentitySize * (i & 0x1FF);
    //}
    //
    //const uintptr_t localPlayer = GetLocalPlayer();
    //const uint16_t localPlayerHandle = GetEntityHandle(localPlayer);
    //static int i = 0;
    //while (true)
    //{
    //    for (const uintptr_t Identity : EntityIdenties)
    //    {
    //        const uintptr_t ent = fastmem.Read<uintptr_t>(Identity);
    //        if (!ent)
    //            continue;
    //		
    //
    //        i++;
    //        //const uintptr_t item = ent + Offsets::m_AttributeManager + Offsets::m_Item;
    //        //if(mem->ReadCUtlSymbolLarge(Identity + 0x20).find("weapon") == std::string::npos)//todo fix this check
    //		//	continue;
    //        //
    //        //std::cout << std::hex << ent << std::endl;
    //        //continue;
    //
    //        //fastmem.Write<uint32_t>(ent + Offsets::m_nFallbackPaintKit, 653);
    //        //fastmem.Write<uint32_t>(item + Offsets::m_iItemIDHigh, -1);
    //        //std::cout << std::hex << ent << std::endl;
    //    }
    //}
    //std::cout << "Total entities processed: " << std::dec << i << std::endl;
    //return 0;
    //std::cout << std::hex << GetEntityByHandle(0x00AC80E7) << std::endl;
    //
    //return 0;