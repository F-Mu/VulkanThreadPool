#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "render/render_device.hpp"
#include "render/render_buffer.hpp"
#include "component.hpp"
#include "core/vertex.hpp"
#include "render/render_model.hpp"

namespace crp {
    class RenderComponent : public Component {
    public:
        explicit RenderComponent(const std::weak_ptr<GameObject> &parent,const std::string &type);

        ~RenderComponent();

        void update();

        void tick() override;
    private:
        std::shared_ptr<Model> model{};
    };
}