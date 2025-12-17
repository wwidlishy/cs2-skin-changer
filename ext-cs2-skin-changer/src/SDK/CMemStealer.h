#include "../../ext/offsets.h"
#include "../../ext/mem.h"

#pragma once

#define MemBlockSize 0x300
#define DefaultPreCacheSize 3
class CMemoryStealer
{
private:
    std::vector<uintptr_t> cachedBlocks;

    uintptr_t GetCachedBlock()
    {
        if (cachedBlocks.empty())
            return NULL;

        for (uint64_t i = 0; i < cachedBlocks.size(); i++)
        {
            if (cachedBlocks[i]) 
            {
                const uintptr_t pickedBlock = cachedBlocks[i];
                cachedBlocks.erase(cachedBlocks.begin() + i);
                return pickedBlock;
            }
        }

        return NULL;
    }

    uintptr_t GetNewDirtyBlock(const uintptr_t item)
    {
        while (!mem.Read<uintptr_t>(item + Offsets::m_pMemBlock)) { Sleep(1); }

        const uintptr_t Block = mem.Read<uintptr_t>(item + Offsets::m_pMemBlock);

        mem.Write<uintptr_t>(item + Offsets::m_pMemBlock, NULL);//clear to regen a new block

        return Block;
    }

public:
    void Cache(const uintptr_t item, const uint64_t sizeToCache)
    {
        if (!item)
            return;

        while(cachedBlocks.size() != sizeToCache)
        {
            const uintptr_t newBlock = GetNewDirtyBlock(item);
            if (!newBlock)
                continue;

            for (uint16_t i = 0; i < MemBlockSize; i++)
            {
                mem.Write<char>(newBlock + i, NULL);//clear old stolen mem section
            }

            cachedBlocks.push_back(newBlock);
        }
    }

    uintptr_t GetBlock(const uintptr_t item, const bool PreCache = false)
    {
        if (!item)
            return NULL;

        const uintptr_t cachedBlock = GetCachedBlock();
        if (cachedBlock)
        {
            Cache(item, DefaultPreCacheSize);
            return cachedBlock;
        }  

        const uintptr_t newBlock = GetNewDirtyBlock(item);

        for (uint16_t i = 0; i < MemBlockSize; i++)
        {
            mem.Write<char>(newBlock + i, NULL);
        }

        return newBlock;
    }
};
CMemoryStealer memStealer;