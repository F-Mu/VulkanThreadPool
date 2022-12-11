#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <list>
#include "core/macro.hpp"
#include "component.hpp"

namespace crp {
    class Particle {
    public:
        explicit Particle(const glm::vec4 &position) : position{position} {}

        void logicTick() {
            position += velocity;
        }

        void renderTick();

        [[nodiscard]] int getLife() const { return life; }

        constexpr static int MAX_LIFE = 3;
    private:
        int life{MAX_LIFE};
        glm::vec4 position;
        glm::vec3 color{1., 0., 0.};
        glm::vec4 velocity{0.00001, -0.00001, 0, 0};
    };

    class ParticleEmitter {
    public:
        explicit ParticleEmitter(const glm::vec4 &position) : position{position} {
            particles.emplace_back(position);
        }

        void logicTick() {
            for (auto &particle: particles) {
                particle.logicTick();
            }
        }

        void renderTick();

        glm::vec4 position;
        std::list<Particle> particles;
    };

    class ParticleComponent : public Component {
    public:
        explicit ParticleComponent(const std::weak_ptr<GameObject> &parent) :
                Component(parent) { type = "ParticleComponent"; }

        void addParticleEmitter(const glm::vec3 &position);

        void update();

        void tick() override;

    private:
        std::list<ParticleEmitter> emitters;
    };
}