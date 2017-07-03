#include "ResourcesHolder.h"

#include <sstream>
#include <stdexcept>
using std::logic_error;

void ResourcesHolder::remove(Resource r, size_t n)
{
    if (_resources[r] < n) {
        std::stringstream err_msg;
        err_msg << "cannot remove " << n << ' ' << EnumInfo::resource_str(r);
        err_msg << ": there is only "<< _resources[r];
        throw logic_error(err_msg.str());
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
