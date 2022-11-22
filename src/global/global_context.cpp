#include "global_context.hpp"
#include "../resources/game_object_manager.hpp"
#include "../systems/thread_pool_system.hpp"
#include "../systems/task_queue_system.hpp"
#include "../systems/runtime_system.hpp"
#include "../resources/move_task_manager.hpp"

namespace crp {
    class CrpDevice;

    GlobalContext globalContext;

    void GlobalContext::startEngine(CrpDevice &crpDevice) {
        moveTaskManager = std::make_shared<MoveTaskManager>();
        gameObjectManager = std::make_shared<GameObjectManager>();
        runTimeSystem = std::make_shared<RuntimeSystem>(crpDevice);
    }

    void GlobalContext::shutdownEngine() {
        runTimeSystem->clear();
        runTimeSystem.reset();
        gameObjectManager.reset();
        moveTaskManager.reset();
    }

}