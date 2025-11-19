#include "../../../ext/mem.h"

#include <thread>

#define UpdateFlag 0x80//128

void UHW(const uintptr_t& pWeapon)
{
    const uintptr_t pIdentity = mem->Read<uintptr_t>(pWeapon + Offsets::m_pEntity);
    mem->Write<uint32_t>(pIdentity + Offsets::m_flags, UpdateFlag);
    Sleep(300);
    mem->Write<uint32_t>(pIdentity + Offsets::m_flags, false);
}

inline void UpdateHudWeapon(const uintptr_t& pWeapon) { std::thread updateHudThread(UHW, pWeapon); updateHudThread.detach(); };