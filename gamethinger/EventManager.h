#pragma once

#include <functional>
#include <vector>

// stole off stackoverflow
// https://stackoverflow.com/questions/59707189/c-callbacks-for-an-event-manager-using-stdfunction-and-stdbind-with-derived
class EventManager
{
public:
    template <typename T>
    static void AddListener(std::function<void(T&)> callback) {
        get_listeners<T>().push_back(std::move(callback));
    }

    /*template <typename T>
    static void RemoveListener(std::function<void(T&)> callback) {
        get_listeners<T>().push_back(std::move(callback));
    }*/

    template <typename T>
    static void TriggerEvent(T& event) {
        for (auto& listener : get_listeners<T>()) {
            listener(event);
        }
    }

private:
    template <typename T>
    static std::vector<std::function<void(T&)>>& get_listeners() {
        static std::vector<std::function<void(T&)>> listeners;
        return listeners;
    }
};