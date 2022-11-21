#pragma once

#include <memory>
#include "../crp_device.hpp"

namespace crp {
    class ThreadPoolSystem;

    class RuntimeSystem;

    class GameObjectManager;

    class TaskQueueSystem;

    class GlobalContext {
    public:
        void startEngine(CrpDevice&crpDevice);

        void shutdownEngine();

    public:
        std::shared_ptr<ThreadPoolSystem> threadPoolSystem;
        std::shared_ptr<RuntimeSystem> runTimeSystem;
        std::shared_ptr<GameObjectManager> gameObjectManager;
        std::shared_ptr<TaskQueueSystem> taskQueueSystem;
    };

    extern GlobalContext globalContext;
}