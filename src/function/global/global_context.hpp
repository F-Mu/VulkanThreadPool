#pragma once

#include <memory>
#include "render/render_device.hpp"
#include "resources/manager/game_object_manager.hpp"
#include "resources/systems/thread_pool_system.hpp"
#include "resources/systems/task_queue_system.hpp"
#include "resources/systems/runtime_system.hpp"
#include "render/window_system.hpp"
#include "render/render_system.hpp"
#include "render/global_resources.hpp"
#include "render/pass/simple_render_pass.hpp"

namespace crp {

    class GlobalContext {
    public:
        void startEngine();

        void shutdownEngine();

    public:
        std::shared_ptr<RuntimeSystem> runTimeSystem;
        std::shared_ptr<GameObjectManager> gameObjectManager;
        std::shared_ptr<GlobalResources> globalResources;
        std::shared_ptr<SimpleRenderPass> simpleRenderPass;
        std::shared_ptr<WindowSystem> windowSystem;
        std::shared_ptr<RenderSystem> renderSystem;
        std::shared_ptr<RenderDevice> device;
        bool debug{};
    };

    extern GlobalContext globalContext;
}