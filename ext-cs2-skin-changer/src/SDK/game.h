#include "../../ext/mem.h"
#include "../../ext/offsets.h"
#include "../../ext/sigs.h"

#include "entity/entity.h"

#include "../../ext/WCL.h"

#pragma once

class Game
{
private:
	Pawn* localplayer = GetLocalPlayer();

public:
	bool UpdateWeapons = false;
	bool UpdateGloves = false;
	bool UpdateKnife = false;

	Pawn* GetLocalPlayer()
	{
		delete localplayer;

		localplayer = new Pawn(mem->Read<uintptr_t>(mem->GetModuleBase(L"client.dll") + Offsets::dwLocalPlayerPawn));

		return localplayer;
	}

	void Update()
	{
		if (UpdateWeapons)
		{
			Sleep(200);
			wcl->CallFunction(Sigs::RegenerateWeaponSkins);
		}	

		//if (UpdateGloves)
		//{
		//	mem->Write<bool>(localplayer->GetPawn() + Offsets::m_bNeedToReApplyGloves, true);
		//	mem->Write<bool>(localplayer->GetGloves()->GetItemAddress() + Offsets::m_bInitialized, true);
		//}

		UpdateWeapons = false;
		UpdateGloves = false;
		UpdateKnife = false;
	}

	void ForceApplyFallbackValues(uintptr_t pWeapon)
	{
		CParamHandler* handler = new CParamHandler({
			CParam(pWeapon, ArgumentType::Ptr),
			CParam(true, ArgumentType::Other),
			});

		wcl->CallFunction(Sigs::ApplyFallbackValues, CallingConventions::FastCall, handler);
		delete handler;
	}

	void SetModel(uintptr_t pEntity, std::string model)
	{

		uintptr_t ModelAddress = mem->Allocate(model.size() + 1);
		if (!ModelAddress)
			return; // allocation failed

		std::vector<uint8_t> bytes(model.begin(), model.end());
		bytes.push_back(0);

		mem->WriteBytes(ModelAddress, bytes);

		CParamHandler* handler = new CParamHandler({
				CParam(pEntity, ArgumentType::Ptr),
				CParam(ModelAddress, ArgumentType::Ptr),
			});

		wcl->CallFunction(Sigs::SetModel, CallingConventions::FastCall, handler);
		delete handler;
	}

	void ApplySkin(uintptr_t item, float SkinId)
	{
		CParamHandler* handler = new CParamHandler({
				CParam(item, ArgumentType::Ptr),
				CParam(Sigs::pSetItemTexturePrefab, ArgumentType::Ptr),
				CParam(SkinId, ArgumentType::Float)
			});

		wcl->CallFunction(Sigs::SetItemAttribute, CallingConventions::FastCall, handler);
		delete handler;
	}
};
Game* game = new Game();