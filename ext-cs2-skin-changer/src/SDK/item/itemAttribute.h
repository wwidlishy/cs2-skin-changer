#include "../../../ext/mem.h"
#include "../../../ext/offsets.h"

#pragma once

enum AttributeDefinitions
{
    paintkit = 6,
};

class CEconItemAttribute
{
private:
    uintptr_t address;

public:
    CEconItemAttribute(uintptr_t pEconAttribute)
    {
        address = pEconAttribute;
    }

    AttributeDefinitions GetAttributeDefinitions()
    {
        return static_cast<AttributeDefinitions>(mem->Read<uint16_t>(address + Offsets::m_iAttributeDefinitionIndex));
    }

    void SetAttributeValue(float value)
    {
        mem->Write<float>(address + Offsets::m_flValue, value);
    }

    float GetAttributeValue()
    {
        return mem->Read<float>(address + Offsets::m_flValue);
    }

    void SetInitAttributeValue(float value)
    {
        mem->Write<float>(address + Offsets::m_flInitialValue, value);
    }

    float GetInitAttributeValue()
    {
        return mem->Read<float>(address + Offsets::m_flInitialValue);
    }
};