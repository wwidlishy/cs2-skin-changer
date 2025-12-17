#include "../../../ext/offsets.h"
#include "../ext/mem.h"

#include "../entity/dwEntityListManager.h"

#pragma once

void SetMeshMask(const uintptr_t ent, const uint64_t mask)
{
    const auto& node = mem.Read<uintptr_t>(ent + Offsets::m_pGameSceneNode);
    const auto model = node + Offsets::m_modelState;
    const auto dirtyAttributes = mem.Read<uintptr_t>(model + Offsets::m_pDirtyModelData);

    mem.Write<uint64_t>(dirtyAttributes + Offsets::m_DrityMeshGroupMask, mask);
    //mem.Write<uint64_t>(model + Offsets::m_MeshGroupMask, mask);
    //for (int i = 0; i < 1000; i++)
    //{
    //    mem.Write<uint64_t>(model + Offsets::m_MeshGroupMask, mask);
    //}
}
//
//inline uintptr_t GetVdata(const uintptr_t& pWeapon)
//{
//    return mem.Read<uintptr_t>(pWeapon + Offsets::m_pVdata);
//}
//
//inline bool IsMeleeWeapon(const uintptr_t& pWeapon)
//{
//    return mem.Read<bool>(GetVdata(pWeapon) + Offsets::m_bMeleeWeapon);
//}

//inline CSWeaponType GetWeaponType(const uintptr_t& pWeapon)
//inline uint32_t GetWeaponType(const uintptr_t& pWeapon)
//{
//    return mem.Read<uint32_t>(GetVdata(pWeapon) + Offsets::m_WeaponType);
//}
//
//inline void SetWeaponType(const uintptr_t& pWeapon, const CSWeaponType type)
//{
//    mem.Write<CSWeaponType>(GetVdata(pWeapon) + Offsets::m_WeaponType, type);
//}