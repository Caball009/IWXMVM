#pragma once

namespace IWXMVM::Types
{
    struct DemoInfo
    {
        std::string name;
        std::string path;

        uint32_t gameTick;
        uint32_t endTick;
    };
}  // namespace IWXMVM::Types