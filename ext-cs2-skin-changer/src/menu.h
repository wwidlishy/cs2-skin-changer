#include "SDK/weapon/C_EconEntity.h"

#include "../ext/skindb.h"

#include "window/window.hpp"

static WeaponsEnum CurrentWeaponDef;

static int selectedSkinIndex = 0;
static uint16_t selectedMusicKitIndex = static_cast<uint16_t>(MusicKit::CounterStrike2);

void RenderMusicKitTab()
{
    const std::vector<MusicKit_t>& availableMusicKits = musicKits;

    // Safety: no music kits
    if (availableMusicKits.empty())
    {
        if (ImGui::BeginCombo("Select Music Kit", ""))
            ImGui::EndCombo();
        return;
    }

    // Sync selected index from current equipped kit
    for (uint16_t i = 0; i < availableMusicKits.size(); i++)
    {
        if (availableMusicKits[i].id == skinManager->MusicKit.id)
        {
            selectedMusicKitIndex = i;
            break;
        }
    }

    // Clamp just in case
    if (selectedMusicKitIndex < 0 ||
        selectedMusicKitIndex >= static_cast<int>(availableMusicKits.size()))
    {
        selectedMusicKitIndex = 0;
    }

    const char* preview = availableMusicKits[selectedMusicKitIndex].name.c_str();
    if (ImGui::BeginCombo("Select Music Kit", preview))
    {
        for (int i = 0; i < static_cast<int>(availableMusicKits.size()); i++)
        {
            bool isSelected = (selectedMusicKitIndex == i);
            if (ImGui::Selectable(availableMusicKits[i].name.c_str(), isSelected))
                selectedMusicKitIndex = i;

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // Apply selected music kit
    // index 0 = default / no kit (same idea as skins)
    if (selectedMusicKitIndex != 0)
        skinManager->MusicKit = availableMusicKits[selectedMusicKitIndex];
}

void RenderWeaponTab()
{
    const std::vector<SkinInfo_t> availableSkins = skindb->GetWeaponSkins(CurrentWeaponDef);

    // Safety: ensure selectedSkinIndex is valid for this availableSkins
    if (availableSkins.empty()) {
        // no skins for this weapon: show an empty/placeholder preview
        if (ImGui::BeginCombo("Select Skin", "")) {
            ImGui::EndCombo();
        }
        return;
    }

    //if (selectedSkinIndex < 0 || selectedSkinIndex >= static_cast<int>(availableSkins.size()))
    //    selectedSkinIndex = 0; // clamp or reset when changing weapon

	selectedSkinIndex = skinManager->GetSkinIndexFromArray(availableSkins, skinManager->GetSkin(CurrentWeaponDef));

    const char* preview = availableSkins[selectedSkinIndex].name.c_str();
    if (ImGui::BeginCombo("Select Skin", preview))
    {
        for (int i = 0; i < static_cast<int>(availableSkins.size()); i++)
        {
            bool isSelected = (selectedSkinIndex == i);
            if (ImGui::Selectable(availableSkins[i].name.c_str(), isSelected))
                selectedSkinIndex = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (selectedSkinIndex != 0) // keep existing semantic for default slot 0
		skinManager->AddSkin(availableSkins[selectedSkinIndex]);

	//if (ImGui::Button("Force Update")) not needed or working
	//	ForceUpdate = true;
}

//void RenderGlovesTab()
//{
//
//}
//
//static int selectedKnifeIndex = 0;
//static bool bKnife = false;
//
//void RenderKnifeTab()
//{
//	ImGui::Checkbox("Knifes", &bKnife);
//
//	if (ImGui::BeginCombo("Select Knife", Knifes[selectedKnifeIndex].name.c_str()))
//	{
//		for (int i = 0; i < Knifes.size(); i++)
//		{
//			bool isSelected = (selectedKnifeIndex == i);
//			if (ImGui::Selectable(Knifes[i].name.c_str(), isSelected))
//				selectedKnifeIndex = i;
//
//			// Set the initial focus when opening the combo (optional)
//			if (isSelected)
//				ImGui::SetItemDefaultFocus();
//		}
//		ImGui::EndCombo();
//	}
//
//	skinManager->ActiveKnife = Knifes[selectedKnifeIndex];
//}

void RenderMenu()
{
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("CS2 Skin Changer", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);

	// Sidebar tabs
	static int active_tab = 0; // 0: Weapon, 1: Gloves, 2: Knife, 3: Agents, 4: Misc

	ImGui::BeginChild("Sidebar", ImVec2(120, 0), true);
	{
		if (ImGui::Button("Skins", ImVec2(100, 30))) active_tab = 0;
		if (ImGui::Button("MusicKit", ImVec2(100, 30))) active_tab = 1;
	}
	ImGui::EndChild();

	ImGui::SameLine();

	// Main content area
	ImGui::BeginChild("Content", ImVec2(0, 0), false);
	{
		switch (active_tab)
		{
		case 0:
			RenderWeaponTab();
			break;
        case 1:
            RenderMusicKitTab();
            break;
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

void OnFrame()
{
    overlay::Render();
    RenderMenu();
    overlay::EndRender();
}

//bool MenuOpen = true;
void MenuThread()
{
	while (true)
	{
		Sleep(10);

		//if (GetAsyncKeyState(VK_INSERT) & 1)
		//	MenuOpen = !MenuOpen;
        //
		//if (!MenuOpen)
		//	continue;

        OnFrame();
	}
}

void UpdateActiveMenuDef(const uintptr_t localPlayer)
{
	const uintptr_t activeWeapon = mem.Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);

	const uintptr_t activeItem = activeWeapon + Offsets::m_AttributeManager + Offsets::m_Item;

	CurrentWeaponDef = mem.Read<WeaponsEnum>(activeItem + Offsets::m_iItemDefinitionIndex);
}

void InitMenu(const bool autoThread = false)
{
	overlay::Setup();
	
    if (autoThread)
    {
        std::thread menuthread(MenuThread);
        menuthread.detach();
    }

	std::cout << "Menu Setup\n";
}