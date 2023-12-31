#include "Hooks.h"

namespace Hooks {
    bool IncrementStat(RE::BSFixedString* stat, std::int32_t value) {
        REL::Relocation<decltype(&IncrementStat)> func{RELOCATION_ID(16117, 16359)};
        return func(stat, value);
    }
}
