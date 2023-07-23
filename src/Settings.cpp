#include "Settings.h"
#include "SimpleIni.h"

Settings* Settings::GetSingleton() {
    static Settings singleton;
    return std::addressof(singleton);
}

void Settings::LoadSettings() {
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\StatsOfStealing.ini)");

    coins_flag = ini.GetValue("General", "bEnableCoins");

    logger::info("Loaded settings");
    logger::info("    bEnableCoins = {}", coins_flag);
}
