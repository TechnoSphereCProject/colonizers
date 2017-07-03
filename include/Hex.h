#ifndef HEX_H
#define HEX_H

#include <stdexcept>
#include <ResourcesHolder.h>

class Hex
{
    Hex(const Hex &) = delete;
    Hex &operator= (const Hex &) = delete;

public:
    Hex() {}

    void set_number(size_t n);
    void unset_number();
    bool has_number() const noexcept {return _has_number;}
    size_t number() const;

    void set_resource(Resource);
    void unset_resource();
    bool has_resource() const noexcept {return _has_resource;}
    Resource resource() const;

private:
    bool _has_number = false;
    size_t _number;
    bool _has_resource = false;
    Resource _resource;
};

#endif // HEX_H
