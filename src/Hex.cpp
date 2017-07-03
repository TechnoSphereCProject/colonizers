#include "Hex.h"
#include <sstream>
using std::logic_error;

void Hex::set_number(size_t n)
{
    if (_has_number) {
        std::stringstream err_msg;
        err_msg << "cannot set number " << n;
        err_msg << ": the number on hex is " << _number;
        throw logic_error(err_msg.str());
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
        std::string err_msg = "cannot set resource " + EnumInfo::resource_str(res) +
            " : the resource on hex is " + EnumInfo::resource_str(_resource);
        throw logic_error(err_msg);
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
