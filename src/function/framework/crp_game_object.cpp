#include "crp_game_obejct.hpp"
#include "render/crp_device.hpp"
#include "core/macro.hpp"
#include <iostream>

namespace crp {
    void CrpGameObject::tick() {
        for (auto &component: components) {
            component->tick();
        }
    }
}