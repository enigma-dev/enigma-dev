// serialization.h

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <iostream>
#include <string>

namespace ENIGMA {

class Serializable {
public:
    virtual void serialize(std::ostream& os) const = 0;
    virtual void deserialize(std::istream& is) = 0;
};

} // namespace ENIGMA

#endif // SERIALIZATION_H

// serialization.cpp

#include "serialization.h"

namespace ENIGMA {

template<>
void Serializable::serialize(std::ostream& os) const {
    os << value;
}

template<>
void Serializable::deserialize(std::istream& is) {
    is >> value;
}

class Map : public Serializable {
public:
    Map() {}
    Map(const std::string& key, const std::string& value) : key(key), value(value) {}

    void serialize(std::ostream& os) const override {
        os << key << "=" << value;
    }

    void deserialize(std::istream& is) override {
        is >> key >> value;
    }

private:
    std::string key;
    std::string value;
};

} // namespace ENIGMA
