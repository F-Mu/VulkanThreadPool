#include "global_context.hpp"
#include "../resources/game_object_manager.hpp"
#include "../systems/thread_pool_system.hpp"
#include "../systems/task_queue_system.hpp"
#include "../systems/runtime_system.hpp"

namespace crp {
    class CrpDevice;

    GlobalContext globalContext;

    void GlobalContext::startEngine(CrpDevice &crpDevice) {
        gameObjectManager = std::make_shared<GameObjectManager>();
        threadPoolSystem = std::make_shared<ThreadPoolSystem>(crpDevice, gameObjectManager);
        runTimeSystem = std::make_shared<RuntimeSystem>(crpDevice, gameObjectManager);
        taskQueueSystem = std::make_shared<TaskQueueSystem>(crpDevice, gameObjectManager);
    }

    void GlobalContext::shutdownEngine() {
        gameObjectManager.reset();
        threadPoolSystem.reset();
        runTimeSystem.reset();
        taskQueueSystem.reset();
    }

}