#include <iostream>

#pragma once

namespace Sigs
{
	uintptr_t RegenerateWeaponSkins = mem->SigScan(L"client.dll", "48 83 EC ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 10");
	uintptr_t SetItemAttribute = mem->SigScan(L"client.dll", "40 53 48 83 EC ? 48 8B D9 48 81 C1 ? ? ? ? E8 ? ? ? ? 48 8B 8B");
	uintptr_t SetModel = mem->SigScan(L"client.dll", "40 53 48 83 EC 20 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24");
	uintptr_t SetMeshMask = mem->SigScan(L"client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 99 ? ? ? ? 48 8B 71");

	uintptr_t ApplyFallbackValues = mem->SigScan(L"client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B DA 48 8B F9 E8 ? ? ? ? F6 C3 ? 0F 84 ? ? ? ? 48 8B 87");

	uintptr_t pSetItemTexturePrefab = mem->SigScan(L"client.dll", "73 65 74 20 69 74 65 6D 20 74 65 78 74 75 72 65 20 70 72 65 66 61 62 00"); //set item texture prefab
	uintptr_t pSetItemTextureWear = mem->SigScan(L"client.dll", "73 65 74 20 69 74 65 6D 20 74 65 78 74 75 72 65 20 77 65 61 72 00");//set item texture wear
	uintptr_t pSetItemTextureSeed = mem->SigScan(L"client.dll", "73 65 74 20 69 74 65 6D 20 74 65 78 74 75 72 65 20 73 65 65 64 00");//set item texture seed
}