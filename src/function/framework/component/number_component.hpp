#pragma once

#include "component.hpp"
#include "utils/number.hpp"
#include "render/render_model.hpp"

namespace crp {
    class NumberComponent : public Component {
    public:
        explicit NumberComponent(const std::weak_ptr<GameObject> &parent, const std::string &type,
                                 float meshWidth = 0, float meshHeight = 0, int num = 5);

        void tick() override;

        void setBegin() { begin = true; }

    private:
        void setNumber(int number);

        std::shared_ptr<Model> model{};
        float width;
        float height;
        int number;
        float deltaTime;
        std::chrono::steady_clock::time_point lastTime;
        std::atomic<bool> fistTick{true};
        std::atomic<bool> begin;
    };
}