#pragma once

#include "component.hpp"
#include "core/macro.hpp"

namespace crp {
    class DeleteComponent : public Component {
    public:
        explicit DeleteComponent(const std::weak_ptr<GameObject> &parent, float width = 0, float height = 0);

        void tick() override;

        [[nodiscard]] bool isFinished() const { return finished; }

    private:
        float time = FRAME_TIME;
        float width;
        float height;
        bool finished{false};
    };
}