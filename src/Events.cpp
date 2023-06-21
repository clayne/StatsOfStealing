#include "Events.h"

#include "Hooks.h"
#include "Settings.h"

namespace Events {
    OnActivateEventHandler* OnActivateEventHandler::GetSingleton() {
        static OnActivateEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnActivateEventHandler::ProcessEvent(const RE::TESActivateEvent* a_event,
                                                                  RE::BSTEventSource<RE::TESActivateEvent>*
                                                                  a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (a_event->actionRef->IsPlayerRef()) {
            const auto& obj = a_event->objectActivated;
            const auto name = obj->GetBaseObject()->GetName();

            if (a_event->actionRef->IsPlayerRef() && obj->IsCrimeToActivate()) {
                if ((obj->GetFormID() == 0xf && Settings::coins_flag) || !"Coin Purse"sv.compare(name))
                    Hooks::IncrementStat();
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnActivateEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered activate event handler");
    }

    OnContainerChangedEventHandler* OnContainerChangedEventHandler::GetSingleton() {
        static OnContainerChangedEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnContainerChangedEventHandler::ProcessEvent(
        const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (const auto player = RE::PlayerCharacter::GetSingleton();
            player->GetParentCell() && player->Is3DLoaded()) {
            if (player->GetFormID() == a_event->newContainer && a_event->baseObj) {
                if (const auto old_container = RE::TESForm::LookupByID<RE::TESObjectREFR>(a_event->oldContainer)) {
                    if (old_container->IsCrimeToActivate()) Hooks::IncrementStat();
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnContainerChangedEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered container changed event handler");
    }
}
