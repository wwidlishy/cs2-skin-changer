#include "menu.h"

Overlay* overlay = new Overlay();

int main()
{
	//std::cout << std::hex << mem->SigScan(L"client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 99 ? ? ? ? 48 8B 71") << std::endl;
    //
    ////CParamHandler* handler = new CParamHandler({
    ////        CParam(0x35E0A87A080, ArgumentType::Ptr),
    ////        CParam(0, ArgumentType::Other),
    ////    });
    ////
    ////wcl->CallFunction(Sigs::SetMeshMask, CallingConventions::FastCall, handler);
    ////delete handler;
    ////
    ////return 0;
    //Pawn* localplayer = game->GetLocalPlayer();
    //localplayer->GetActiveViewModel()->SetMeshMask(0);
    //localplayer->GetWeaponService()->GetActiveWeapon()->SetMeshMask(0);
    //
	//return 0;

    skindb->Dump();
    overlay->SetupOverlay("cs2 ext skin changer");

    std::cout << "[SkinChanger] Loaded Press Insert For Menu\n";
    Beep(400, 500);

    while (true)
    {
        overlay->StartRender();

        if (overlay->RenderMenu)
            overlay->Render(&RenderMenu);
    
        Pawn* localplayer = game->GetLocalPlayer();
        if (!localplayer && !localplayer->GetPawn())
            continue;
    
        //cskin->OnAgent(localplayer);
        //cskin->OnGloves(localplayer->GetGloves());

        CEconEntity* Weapon = localplayer->GetWeaponService()->GetActiveWeapon();
        if (!Weapon->GetBase() && !Weapon->GetBase())
            continue;
        
        
        //std::cout << std::hex << Weapon->GetBase() << std::endl;
        //continue;

        if(Weapon->IsKnife())
            cskin->OnKnife(Weapon);
        else
            cskin->OnWeapon(Weapon);

        game->Update();
        overlay->EndRender();
    
        delete Weapon;
		Sleep(10);
    }  
}