#include <list>
#include <set>
#include "types.hxx"

class Package {
public:
    Package();
    Package(ElementID id);
    Package(Package&&);

    ~Package();
    Package& operator=(Package&& other);

    ElementID get_id() const {return id_;};
private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};
