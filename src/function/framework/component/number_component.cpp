#include "number_component.hpp"
#include "utils/number.hpp"
#include "function/framework/game_object.hpp"
#include "mesh_component.hpp"
#include "function/global/global_context.hpp"

namespace crp {
    void NumberComponent::tick() {
        if (!number)return;
        if (!model)return;
        if (isDirty())return;
        if (begin) {
            using namespace std::chrono;
            if (fistTick)//任务执行时第一次计时
                lastTime = steady_clock::now();
            fistTick = false;
            auto nowTime = steady_clock::now();
            duration<float> timeSpan = duration_cast<duration<float>>(nowTime - lastTime);
            deltaTime += timeSpan.count();
            if (deltaTime > 1) {
                setNumber(number - 1);
                deltaTime -= 1;
            } else
                setNumber(number);
            lastTime = nowTime;
        }
        auto transform = m_parent_object.lock()->tryGetComponent(TransformComponent);
        PushConstantData push{};
        push.modelMatrix = transform->mat4();
        push.normalMatrix = transform->normalMatrix();
        push.modelMatrix[3][2] = 0;
        globalContext.simpleRenderPass->bind(push, model);
    }

    void NumberComponent::setNumber(int num) {
        number = num;
        if (number == 0)return;
        auto vertices = std::move(Number::getNumber(width, height,
                                                    glm::vec3{1., 0.8, 0.4} * (static_cast<float>(number) - deltaTime) /
                                                    5.f, number));
        std::vector<uint32_t> indices;
        for (int i = 0; i < vertices.size(); i += 4) {
            for (int j = i; j < i + 2; j++) {
                for (int k = 0; k < 3; ++k) {
                    indices.emplace_back(j + k);
                }
            }
        }
        model = std::make_shared<Model>(vertices, indices);
    }

    NumberComponent::NumberComponent(const std::weak_ptr<GameObject> &parent, const std::string &type, float meshWidth,
                                     float meshHeight, int num) :
            Component(parent, type), width{meshWidth}, height{meshHeight}, number{num}, begin{false} {

        setNumber(number);
    }
}
