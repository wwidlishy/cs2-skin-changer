#include <vector>
#include "mem.h"

#pragma once

enum CallingConventions
{
    StdCall,
    FastCall,
};

enum ASM : uint8_t
{
    Call = 0xE8,
    Jmp = 0xE9,
    Mov = 0x8B,
    Push = 0x68,
    Ret = 0xC3
};

enum Registers : uint8_t
{
    ECX = 0xB9,
    EDX = 0xBA,
};

enum ArgumentType
{
    Ptr,
    Float,
    Bool,
    Other,
};

class CParam
{
public:
    std::vector<uint8_t> value;
    ArgumentType type;

    template<typename T>
    CParam(T v, ArgumentType type)
    {
        this->type = type;
        value.resize(sizeof(T));
        std::memcpy(value.data(), &v, sizeof(T));
    }
};

class CParamHandler
{
public:
    std::vector<CParam> args;

    CParamHandler(std::initializer_list<CParam> initArgs)
        : args(initArgs)
    {
    }

    int ArgsSize() const { return static_cast<int>(args.size()); }
};


class WCL
{
private:
    std::vector<uint8_t> FixPtr(uintptr_t target, uintptr_t instrAddrAfterCall)
    {
        int32_t rel = static_cast<int32_t>(target - instrAddrAfterCall);
        uint8_t* relBytes = reinterpret_cast<uint8_t*>(&rel);

        return { relBytes[0], relBytes[1], relBytes[2], relBytes[3] };
    }

    size_t GetArgsSize(CParamHandler* ArgsHandler)
    {
        if (!ArgsHandler)
            return 0;

        size_t size = 0;
        for (int i = 0; i < ArgsHandler->ArgsSize(); i++)   
            //if(ArgsHandler->args[i].type == ArgumentType::Bool)
			//	size += sizeof(uintptr_t);
            //else
                size += ArgsHandler->args[i].value.size();
            
        return size;
    }

    inline size_t GetCallSize() { return sizeof(ASM::Call) + sizeof(uint32_t); }

    uintptr_t Mylloc(size_t size, uintptr_t wantedAddr)
    {
        const size_t allocGran = 0x10000;
        uintptr_t address = 0;

        for (int i = 0; address == 0; i++)//i < 0x200 && 
        {
            uintptr_t candidate = wantedAddr ? (wantedAddr - i * allocGran) & ~(allocGran - 1) : 0;
            address = mem->Allocate(size, candidate);
        }

        if (!address)
            std::cerr << "Allocation failed\n";

        return address;
    }

    void FixArgs(CParamHandler* ArgsHandler, uintptr_t CodeCaveAddress)
    {
        if (!ArgsHandler || ArgsHandler->ArgsSize() == 0)
            return;

        for (int i = 0; i < ArgsHandler->ArgsSize(); i++)
        {
            CParam& param = ArgsHandler->args[i];

            if (param.type == ArgumentType::Ptr && param.value.size() == sizeof(uintptr_t))
            {
                uintptr_t ptrValue = 0;
                for (size_t b = 0; b < sizeof(uintptr_t); b++)
                    ptrValue |= (uintptr_t)param.value[b] << (8 * b);

                std::vector<uint8_t> fixedBytes(sizeof(uintptr_t));
                std::memcpy(fixedBytes.data(), &ptrValue, sizeof(uintptr_t));
                param.value = fixedBytes;
            }

            if (param.type == ArgumentType::Float && param.value.size() == sizeof(float))
            {
                float fValue;
                std::memcpy(&fValue, param.value.data(), sizeof(float));

                uint8_t floatBytes[4];
                std::memcpy(floatBytes, &fValue, sizeof(float));

                param.value.assign(floatBytes, floatBytes + sizeof(float));
            }
        }
    }

    size_t WriteRegisters(CParamHandler* ArgsHandler, uintptr_t CodeCaveAddress, CallingConventions convention)
    {
        if (!ArgsHandler || ArgsHandler->ArgsSize() == 0)
            return 0;

        std::vector<uint8_t> codeBytes;

        switch (convention)
        {
        case CallingConventions::StdCall:
            for (int i = ArgsHandler->ArgsSize() - 1; i >= 0; i--)
            {
                codeBytes.push_back(ASM::Push);
                for (auto byte : ArgsHandler->args[i].value)
                    codeBytes.push_back(byte);
            }
            break;

        case CallingConventions::FastCall:
            if (ArgsHandler->ArgsSize() >= 1)
            {
                codeBytes.push_back(0x48);
                codeBytes.push_back(0xB9);
                for (auto byte : ArgsHandler->args[0].value)
                    codeBytes.push_back(byte);
            }
            if (ArgsHandler->ArgsSize() >= 2)
            {
                codeBytes.push_back(0x48);
                codeBytes.push_back(0xBA);

                for (auto byte : ArgsHandler->args[1].value)
                    codeBytes.push_back(byte);
            }
            if (ArgsHandler->ArgsSize() >= 3 && ArgsHandler->args[2].type == ArgumentType::Float)
            {
                uintptr_t floatMem = Mylloc(sizeof(float), CodeCaveAddress);
                mem->WriteBytes(floatMem, ArgsHandler->args[2].value);

                codeBytes.insert(codeBytes.end(), { 0xF3, 0x0F, 0x10, 0x15 });

                uintptr_t instrAddrAfterDisp = CodeCaveAddress + codeBytes.size() + 4;

                std::vector<uint8_t> rel = FixPtr(floatMem, instrAddrAfterDisp);
                codeBytes.insert(codeBytes.end(), rel.begin(), rel.end());
            }
            for (int i = ArgsHandler->ArgsSize() - 1; i >= 3; i--)
            {
                codeBytes.push_back(ASM::Push);
                for (auto byte : ArgsHandler->args[i].value)
                    codeBytes.push_back(byte);
            }
            break;

        default:
            break;
        }

        mem->WriteBytes(CodeCaveAddress, codeBytes);

        return codeBytes.size();
    }

    void WriteJmpBytes(uintptr_t FuncAddress, uintptr_t JmpLocation)
    {
        std::vector<uint8_t> JmpBytes;

        JmpBytes.push_back(ASM::Jmp);
        std::vector<uint8_t> relBytes = FixPtr(FuncAddress, JmpLocation + 5);
        JmpBytes.insert(JmpBytes.end(), relBytes.begin(), relBytes.end());

        mem->WriteBytes(JmpLocation, JmpBytes);
    }

public:
    void CallFunction(uintptr_t FuncAddress, CallingConventions convention = CallingConventions::StdCall, CParamHandler* ArgsHandler = nullptr)
    {
        size_t estSize = GetArgsSize(ArgsHandler) + GetCallSize();
        uintptr_t CodeCaveAddress = Mylloc(estSize, FuncAddress);

        FixArgs(ArgsHandler, CodeCaveAddress);

        size_t allocSize = WriteRegisters(ArgsHandler, CodeCaveAddress, convention);

       //for (auto arg : ArgsHandler->args)
       //    if(arg.type = ArgumentType::Bool)
       //        allocSize += sizeof(uint32_t) + 0x1;
       //
       //WriteJmpBytes(FuncAddress, CodeCaveAddress + allocSize);
       //
       //std::cout << std::hex
       //    << "CodeCave Address: 0x" << CodeCaveAddress << "\n"
       //    << "Target Function:  0x" << FuncAddress << "\n";
       //while (!GetAsyncKeyState(VK_DELETE)) {}

        mem->CallThread(CodeCaveAddress);
        mem->Free(CodeCaveAddress, estSize);
    }
    
    void CallVFunc(uintptr_t RTTIObject, uint32_t index, CallingConventions convention = CallingConventions::StdCall, CParamHandler* ArgsHandler = nullptr)
    {
        CallFunction(mem->Read<uintptr_t>(mem->Read<uintptr_t>(RTTIObject) + index * sizeof(uintptr_t)), convention, ArgsHandler);
    }
};
WCL* wcl = new WCL();
