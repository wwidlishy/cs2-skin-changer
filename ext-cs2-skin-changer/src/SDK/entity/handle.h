#include "../entity/econentity.h"

#include "../../../ext/mem.h"
#include "../../../ext/offsets.h"

#pragma once

class CEconEntity;

class CHandle
{
private:
    uint32_t handle;

public:
    CHandle(uint32_t hHandle)
    {
        handle = hHandle;
    }

    uintptr_t Get()
    {
        uintptr_t entitylist = mem->Read<uintptr_t>(mem->GetModuleBase(L"client.dll") + Offsets::dwEntityList);

        uintptr_t listentry = mem->Read<uintptr_t>(entitylist + 0x8 * ((handle & 0x7FFF) >> 9) + 0x10);

        uintptr_t entity = mem->Read<uintptr_t>(listentry + 0x78 * (handle & 0x1FF));

        return entity;
    }

    CEconEntity* GetEconFromHandle()
    {
        return new CEconEntity(Get());
    }
};