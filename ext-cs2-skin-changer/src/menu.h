#include "skin/CSkin.h"
#include "../ext/skindb.h"

namespace Globals
{
	static bool AllowAllSkins = false;
}

void RenderWeapons()
{
	WeaponsEnum WantedWeapon = WeaponsEnum::none;
	WeaponsEnum CurrentType = game->GetLocalPlayer()->GetWeaponService()->GetActiveWeapon()->GetItem()->GetItemDefinition();

	if (!Globals::AllowAllSkins)
		WantedWeapon = CurrentType;

	std::vector<SkinInfo> skins = skindb->GetWeapons(WantedWeapon);
	int32_t WeantedSkinIndex = 0;
	static char search_buffer[64] = "";
	static std::vector<int> filtered_indices;

	SkinInfo* currentskin = cskin->GetCurrentAddedSkin();

	if (currentskin && currentskin->Paint)
		for (int i = 0; i < skins.size(); i++)
			if (skins[i].Paint == currentskin->Paint)
				WeantedSkinIndex = i;

	ImGui::InputText("Search", search_buffer, IM_ARRAYSIZE(search_buffer));
	ImGui::BeginChild("SkinsList", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);

	filtered_indices.clear();

	for (int i = 0; i < skins.size(); i++)
	{
		std::string name = skins[i].name;
		std::string query = search_buffer;

		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		std::transform(query.begin(), query.end(), query.begin(), ::tolower);

		if (query.empty() || name.find(query) != std::string::npos)
			filtered_indices.push_back(i);
	}

	for (int idx : filtered_indices)
	{
		bool is_selected = (WeantedSkinIndex == idx);
		if (ImGui::Selectable(skins[idx].name.c_str(), is_selected))
			WeantedSkinIndex = idx;

		if (is_selected)
			ImGui::SetItemDefaultFocus();
	}

	if (!filtered_indices.empty() && WeantedSkinIndex >= 0 && WeantedSkinIndex < filtered_indices.size())
	{
		auto& selectedSkin = skins[filtered_indices[WeantedSkinIndex]];
		cskin->AddSkin(new SkinInfo(
			selectedSkin.Paint,
			selectedSkin.name,
			selectedSkin.bUsesOldModel,
			CurrentType
		));
	}

	ImGui::EndChild();
}

void RenderMenu()
{
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("CS2 Skin Changer", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);

	// Sidebar tabs
	static int active_tab = 0; // 0: Weapon, 1: Gloves, 2: Knife, 3: Agents, 4: Misc

	ImGui::BeginChild("Sidebar", ImVec2(120, 0), true);
	{
		if (ImGui::Button("Weapon", ImVec2(100, 30))) active_tab = 0;
		if (ImGui::Button("Gloves", ImVec2(100, 30))) active_tab = 1;
		if (ImGui::Button("Knife", ImVec2(100, 30))) active_tab = 2;
		if (ImGui::Button("Agents", ImVec2(100, 30))) active_tab = 3;
		if (ImGui::Button("Misc", ImVec2(100, 30))) active_tab = 4;
	}
	ImGui::EndChild();

	ImGui::SameLine();

	// Main content area
	ImGui::BeginChild("Content", ImVec2(0, 0), false);
	{
		switch (active_tab)
		{
		case 0: // Weapon
			RenderWeapons();
			break;
		case 1: // Gloves
			ImGui::Text("Gloves tab content here");
			break;
		case 2: // Knife
			ImGui::Text("Knife tab content here");
			break;
		case 3: // Agents
			ImGui::Text("Agents tab content here");
			break;
		case 4: // Misc
			ImGui::Checkbox("Allow All Skins", &Globals::AllowAllSkins);
			break;
		}
	}
	ImGui::EndChild();

	ImGui::End();
}