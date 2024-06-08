#pragma once
#include "StdInclude.hpp"
#include "Utilities/Signatures.hpp"

namespace IWXMVM::IW5::Signatures
{
    struct IW5Addresses
    {
#define Sig IWXMVM::Signatures::Signature < IWXMVM::Signatures::SignatureImpl
#define Lambda IWXMVM::Signatures::Lambdas

        using GAType = IWXMVM::Signatures::GameAddressType;

        Sig("83 C4 04 89 06 8B 0E", GAType::Data, -5, Lambda::FollowCodeFlow) > Dvar_FindDvar;
        Sig("8B 44 24 0C 8A 08 83 C4 04", GAType::Code, -7) > Cbuf_AddText;
        Sig("56 89 74 24 0C E8", GAType::Code, -8) > Com_TimeScaleMsec;
        
        Sig("E8 ?? ?? ?? ?? 83 C4 04 84 C0 74 15 A1 ?? ?? ?? ?? 6A 00", GAType::Code, -5) > IN_Frame;
        Sig("56 E8 ?? ?? ?? ?? 83 C4 04 84 C0 0F 84 B9 04 00 00", GAType::Code, -6) > CL_Demo_HandleInput;
        Sig("83 EC 78 56 8B 75 08", GAType::Code, -6) > CL_Demo_FreeCameraMove;
        Sig("83 EC 34 53 56 8B 75 08 57", GAType::Code, -6) > CL_Demo_FollowCameraMove;
        Sig("33 C0 83 C4 40 89 44 24 60", GAType::Code, -5, Lambda::FollowCodeFlow) > CL_Demo_GetStartAndEndTime;
        Sig("8B 04 8D ?? ?? ?? ?? 81 EC 4C 02 00 00", GAType::Code, -6) > CL_Demo_Play;

        Sig("C6 05 ?? ?? ?? ?? ?? 8B 08 8B 91 A4 00 00 00", GAType::Data, -4) > d3d9DevicePointer;
        Sig("6A 02 FF 15 ?? ?? ?? ?? 8B 6C 24 48", GAType::Code, -5) > MainWndProc;

#undef Sig
#undef Lambda
    };
}  // namespace IWXMVM::IW5::Signatures