#include <iostream>
#include "../../../ext/WCL.h"
#include "../../../ext/sigs.h"

class HudModelArms
{
private:
	uintptr_t address;

public:
    HudModelArms(uintptr_t arms)
    {
        address = arms;
    }

    uintptr_t GetBase()
    {
        return address;
    }

    void SetMeshMask(uint64_t mask)
    {
		std::cout << std::hex << address << std::endl;
		//mem->Write<bool>(address + 0xCC0 + 0x51, false);
        
        CParamHandler* handler = new CParamHandler({
                CParam(address, ArgumentType::Ptr),
            });
        
        wcl->CallFunction(Sigs::SetMeshMask, CallingConventions::FastCall, handler);
    }
};