#include "ResourcesHolder.h"

#include <sstream>
#include <stdexcept>
#include "format.h"
using std::logic_error;

void ResourcesHolder::remove(Resource r, size_t n)
{
    if (_resources[r] < n) {
        throw logic_error(fmt::format("cannot remove {0} {1} : there is only {2}", n, EnumInfo::resource_str(r), _resources[r]));
    }
    _resources[r] -= n;
}

size_t ResourcesHolder::resources(Resource r) const noexcept
{
    auto rec_it = _resources.find(r);
    if (rec_it == _resources.end()) {
        return 0;
    } else {
        return rec_it->second;
    }
}

size_t ResourcesHolder::resources() const noexcept
{
    size_t sum = 0;
    for (auto i: _resources) {
        sum += i.second;
    }
    return sum;
}
