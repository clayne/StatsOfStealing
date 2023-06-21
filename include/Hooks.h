#pragma once

namespace Hooks {
    inline static RE::BSFixedString items_stolen = "Items Stolen";

    bool IncrementStat(RE::BSFixedString* stat = &items_stolen, std::int32_t value = 1);
}