#ifndef RESOURCESHOLDER_H
#define RESOURCESHOLDER_H

#include "Resource.h"

#include<map>

class ResourcesHolder
{
    ResourcesHolder(const ResourcesHolder &) = delete;
    ResourcesHolder &operator= (const ResourcesHolder &) = delete;

public:
    ResourcesHolder() = default;

    void add(Resource r, size_t n) noexcept {_resources[r] += n;}
    void remove(Resource r, size_t n);

    size_t resources(Resource r) const noexcept;
    size_t resources() const noexcept;

private:
    std::map<Resource, size_t> _resources;
};

#endif // RESOURCESHOLDER_H
