#include <iostream>
#include <array>
#include <vector>
#include <thread>
#include "../SDK/sdk.h"
#include "../../ext/skindb.h"
#include "../window/window.hpp"

#pragma once

class CSkin
{
public:
    std::vector<SkinInfo*> AddedSkins;

    void AddSkin(SkinInfo* skin)
    {
		game->UpdateWeapons = true;
        for (auto& CurrentSkin : AddedSkins)
        {
            if (CurrentSkin->weaponType == skin->weaponType)
            {
                *CurrentSkin = *skin;
                delete skin;         
                return;
            }
        }

        AddedSkins.push_back(skin);
    }

    SkinInfo* GetCurrentAddedSkin()
    {
        auto local = game->GetLocalPlayer();
        if (!local)
            return nullptr;

        auto weapon = local->GetWeaponService()->GetActiveWeapon();
        if (!weapon || !weapon->GetItem())
            return nullptr;

        WeaponsEnum CurrentType = weapon->GetItem()->GetItemDefinition();

        for (auto CurrentSkin : AddedSkins)
        {
            if (CurrentSkin->weaponType == CurrentType)
                return CurrentSkin;
        }

        return nullptr;
    }

    void OnWeapon(CEconEntity* pWeapon)
    {
        SkinInfo* CurrentSkin = GetCurrentAddedSkin();
        if (!CurrentSkin || !CurrentSkin->Paint)
            return;

        if (!game->UpdateWeapons)//ik its bad impl
            return;
        
        //pWeapon->GetItem()->SetItemIdHigh(UseFallBackValues);
        //
        //pWeapon->SetFallbackPaintKit(CurrentSkin->Paint);
        //pWeapon->SetFallbackWear(0.01f);
        //pWeapon->SetFallbackStatTrak(1337);
        
        
        game->ApplySkin(pWeapon->GetItem()->GetItemAddress(), (float)CurrentSkin->Paint);
		game->UpdateWeapons = true;
        //game->ForceApplyFallbackValues(pWeapon->GetBase());
    }

    void OnGloves(CItemView* pGloves)
    {
        if (!pGloves) return;

        uint64_t paint = 10084;
        if (pGloves->GetItemIdHigh() == paint)
            return;

        pGloves->SetItemDefinition(5032);
        pGloves->SetItemIdHigh(paint);
        game->ApplySkin(pGloves->GetItemAddress(), (float)paint);
        game->UpdateGloves = true;
    }

    void OnKnife(CEconEntity* pKnife)
    {
		return;

        if (!pKnife) return;

        int32_t paint = 506;
        if (pKnife->GetItem()->GetItemIdHigh() == paint)
            return;

        pKnife->GetItem()->SetItemDefinition(515);
        pKnife->GetItem()->SetItemIdHigh(paint);
        mem->Write<bool>(pKnife->GetItem()->GetItemAddress() + Offsets::m_bInitialized, true);
        game->ApplySkin(pKnife->GetItem()->GetItemAddress(), (float)paint);
        game->UpdateWeapons = true;
    }

    void OnAgent(Pawn* pAgent)
    {
        if (!pAgent) return;
        game->SetModel(pAgent->GetPawn(), "characters/models/ctm_fbi/ctm_fbi_varianta.vmdl");
    }
};
CSkin* cskin = new CSkin();
