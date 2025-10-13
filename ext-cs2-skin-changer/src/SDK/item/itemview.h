#include "../weapon/weapon.h"
#include "../item/itemAttribute.h"

#pragma once

class CItemView
{
private:
    uintptr_t address;

public:
    CItemView(uintptr_t pWeapon)
    {
        address = pWeapon;
    }

    uintptr_t GetItemAddress()
    {
        return address;
    }

    WeaponsEnum GetItemDefinition()
    {
        return static_cast<WeaponsEnum>(mem->Read<uint16_t>(address + Offsets::m_iItemDefinitionIndex));
    }

    void SetItemDefinition(uint16_t def)
    {
        return mem->Write<uint16_t>(address + Offsets::m_iItemDefinitionIndex, def);
    }

    void SetSkinFallBackName(const char* name)
    {
        char buffer[160] = {};
        strncpy_s(buffer, sizeof(buffer), name, _TRUNCATE);
        mem->Write<char[160]>(address + Offsets::m_szCustomNameOverride, buffer);
    }

    void SetItemIdHigh(uint64_t id)
    {
        mem->Write<uint64_t>(address + Offsets::m_iItemIDHigh, id);
    }

    void SetItemIdHigh(ItemIds id)
    {
        mem->Write<uint64_t>(address + Offsets::m_iItemIDHigh, id);
    }

    uint64_t GetItemIdHigh()
    {
        return mem->Read<uint64_t>(address + Offsets::m_iItemIDHigh);
    }

    CEconItemAttribute* GetItemAttribute(int64_t attributeIndex = 1)
    {
        uintptr_t attributeList = address + Offsets::m_NetworkedDynamicAttributes;

        int64_t ListSize = mem->Read<int64_t>(attributeList + Offsets::m_Attributes);  

        if (!ListSize || !attributeIndex || attributeIndex > ListSize)
            return nullptr;

        uintptr_t pAttribute = mem->Read<uintptr_t>(attributeList + ((Offsets::m_Attributes * 2) * attributeIndex));

        if (!pAttribute)
            return nullptr;

        return new CEconItemAttribute(pAttribute);
    }
};