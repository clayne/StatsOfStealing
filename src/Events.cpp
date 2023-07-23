#include "Events.h"

#include "Hooks.h"
#include "Settings.h"

namespace Events {
    OnContainerChangedEventHandler* OnContainerChangedEventHandler::GetSingleton() {
        static OnContainerChangedEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnContainerChangedEventHandler::ProcessEvent(
        const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (const auto player = RE::PlayerCharacter::GetSingleton();
            player->GetParentCell() && player->Is3DLoaded()) {
            if (player->GetFormID() == a_event->newContainer) {
                if (a_event->oldContainer) {
                    const auto changes = player->GetInventoryChanges(true);
                    const auto obj = RE::TESForm::LookupByID<RE::TESBoundObject>(a_event->baseObj);
                    for (const auto item : *changes->entryList) {
                        if (item->object == obj) {
                            if (!item->GetOwner()->As<RE::PlayerCharacter>()) {
                                Hooks::IncrementStat();
                            }
                        }
                    }
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

    OnActivateEventHandler* OnActivateEventHandler::GetSingleton() {
        static OnActivateEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnActivateEventHandler::ProcessEvent(const RE::TESActivateEvent* a_event,
                                                                  RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (a_event->actionRef->IsPlayerRef()) {
            const auto obj = a_event->objectActivated;
            if (obj->IsCrimeToActivate()
                && (!"Coin Purse"sv.compare(obj->GetName())
                    || (Settings::coins_flag && !"Gold"sv.compare(obj->GetName()))))
                Hooks::IncrementStat();
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnActivateEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered activate event handler");
    }
}
