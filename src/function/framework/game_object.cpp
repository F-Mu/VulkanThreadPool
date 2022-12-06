#include "game_object.hpp"
#include "render/render_device.hpp"
#include "core/macro.hpp"
#include <iostream>

namespace crp {
    void GameObject::tick() {
        if (!shouldTick)return;
        for (auto &component: components) {
            component->tick();
        }
    }

    GameObject::~GameObject() {
        components.clear();
    }
}