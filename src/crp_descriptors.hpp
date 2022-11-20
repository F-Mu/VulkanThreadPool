#pragma once

#include "crp_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace crp {

    class CrpDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(CrpDevice &crpDevice) : crpDevice{crpDevice} {}

            Builder &addBinding(
                    uint32_t binding,
                    VkDescriptorType descriptorType,
                    VkShaderStageFlags stageFlags,
                    uint32_t count = 1);
            std::unique_ptr<CrpDescriptorSetLayout> build() const;

        private:
            CrpDevice &crpDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        CrpDescriptorSetLayout(
                CrpDevice &crpDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~CrpDescriptorSetLayout();
        CrpDescriptorSetLayout(const CrpDescriptorSetLayout &) = delete;
        CrpDescriptorSetLayout &operator=(const CrpDescriptorSetLayout &) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        CrpDevice &crpDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class CrpDescriptorWriter;
    };

    class CrpDescriptorPool {
    public:
        class Builder {
        public:
            Builder(CrpDevice &crpDevice) : crpDevice{crpDevice} {}

            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &setMaxSets(uint32_t count);
            std::unique_ptr<CrpDescriptorPool> build() const;

        private:
            CrpDevice &crpDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        CrpDescriptorPool(
                CrpDevice &crpDevice,
                uint32_t maxSets,
                VkDescriptorPoolCreateFlags poolFlags,
                const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~CrpDescriptorPool();
        CrpDescriptorPool(const CrpDescriptorPool &) = delete;
        CrpDescriptorPool &operator=(const CrpDescriptorPool &) = delete;

        bool allocateDescriptor(
                const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

        void resetPool();

    private:
        CrpDevice &crpDevice;
        VkDescriptorPool descriptorPool;

        friend class CrpDescriptorWriter;
    };

    class CrpDescriptorWriter {
    public:
        CrpDescriptorWriter(CrpDescriptorSetLayout &setLayout, CrpDescriptorPool &pool);

        CrpDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        CrpDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);

    private:
        CrpDescriptorSetLayout &setLayout;
        CrpDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}  // namespace crp