#include "../weapon/weaponservice.h"
#include "hudModelArms.h"

#pragma once

class Pawn
{
private:
    uintptr_t address;

    HudModelArms* GetViewModelBase()
    {
        CHandle* hArms = new CHandle(mem->Read<uint32_t>(address + Offsets::m_hHudModelArms));
        return new HudModelArms(hArms->Get());
    }

public:
    Pawn(uintptr_t pPawn)
    {
        address = pPawn;
    }

    CItemView* GetGloves()
    {
        return new CItemView(address + Offsets::m_EconGloves);
    }

    HudModelArms* GetActiveViewModel()
    {
        uintptr_t armsBase = this->GetViewModelBase()->GetBase();
        uintptr_t armsNode = mem->Read<uintptr_t>(armsBase + Offsets::m_pGameSceneNode);
        for (uintptr_t viewModel = mem->Read<uintptr_t>(armsNode + Offsets::m_pChild); viewModel; viewModel = mem->Read<uintptr_t>(viewModel + Offsets::m_pNextSibling))
        {
            if (!viewModel || !mem->Read<uintptr_t>(viewModel + Offsets::m_pOwner))
                continue;
             
             if (CHandle(mem->Read<uint32_t>(mem->Read<uintptr_t>(viewModel + Offsets::m_pOwner) + Offsets::m_hOwnerEntity)).Get() 
                 != this->GetWeaponService()->GetActiveWeapon()->GetBase())
                 continue;

             return new HudModelArms(mem->Read<uintptr_t>(viewModel + Offsets::m_pOwner));
        }
    }

    WeaponService* GetWeaponService()
    {
        return new WeaponService(mem->Read<uintptr_t>(address + Offsets::m_pWeaponServices));
    }

    bool IsValid()
    {
        return address;
    }

    uintptr_t GetPawn()
    {
        return address;
    }
};