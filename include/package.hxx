#include <list>
#include <set>
#include "types.hxx"

class Package {
public:
    Package();
    Package(ElementID id): id_(id) {};
private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};
