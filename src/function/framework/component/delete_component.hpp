#pragma once

#include "component.hpp"
#include "core/macro.hpp"

namespace crp {
    class DeleteComponent : public Component {
    public:
        DeleteComponent(const std::weak_ptr<CrpGameObject> &parent) : Component(parent) {
            type = "DeleteComponent";
        }

        void tick();

        float time = FRAME_TIME;
    };
}