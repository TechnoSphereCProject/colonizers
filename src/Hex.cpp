#include "Hex.h"
#include <sstream>
#include "format.h"
using std::logic_error;

void Hex::set_number(size_t n)
{
    if (_has_number) {
        throw logic_error(fmt::format("cannot set number {0} : the number on hex is {1}", n, _number));
    }
    _has_number = true;
    _number = n;
}


void Hex::unset_number()
{
    if (!_has_number) {
        throw logic_error("cannot unset number: there is no number on hex");
    }
    _has_number = false;
}

size_t Hex::number() const
{
    if (!_has_number) {
        throw logic_error("cannot return number: there is no number on hex");
    }
    return _number;
}

void Hex::set_resource(Resource res)
{
    if (_has_resource) {
        throw logic_error(fmt::format("cannot set resource {0} : the resource on hex is {1}", EnumInfo::resource_str(res), EnumInfo::resource_str(_resource)));
    }
    _has_resource = true;
    _resource = res;
}

void Hex::unset_resource()
{
    if (!_has_resource) {
        throw logic_error("cannot unset resource: there is no resource on hex");
    }
    _has_resource = false;
}

Resource Hex::resource() const
{
    if (!_has_resource) {
        throw logic_error("cannot return resource: there is no resource on hex");
    }
    return _resource;
}
