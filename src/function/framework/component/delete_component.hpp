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
        static constexpr int MAX_TIME = static_cast<int>(FRAME_TIME);
        int time{0};
        float width;
        float height;
        bool finished{false};
    };
}