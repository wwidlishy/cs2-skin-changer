#include "../entity/econentity.h"
#include "../entity/handle.h"
#include <vector>

class WeaponService
{
private:
    uintptr_t address;

public:
    WeaponService(uintptr_t pWeaponService)
    {
        address = pWeaponService;
    }

    CEconEntity* GetActiveWeapon()
    {
        CHandle* handle = new CHandle(mem->Read<uint32_t>(address + Offsets::m_hActiveWeapon));

        return handle->GetEconFromHandle();
    }

    std::vector<CEconEntity*> GetMyWeapons()
    {
        std::vector<CEconEntity*> weapons;

        CHandle* currentEnt;

        for (int i = 0; i < 1000; i++)
        {
            currentEnt = new CHandle(i);

            if (!currentEnt->Get())
                continue;

            if (mem->ReadCUtlSymbolLarge(mem->Read<uintptr_t>(currentEnt->Get() + Offsets::m_pEntity) + Offsets::m_name).find("weapon_") != std::string::npos)
                continue;

            weapons.push_back(currentEnt->GetEconFromHandle());
        }

        return weapons;
    }
};