#include "package.hxx"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package() {
    if (!(freed_IDs.empty())) {
        id_ = *(freed_IDs.begin());
        freed_IDs.erase(id_);
    }
    else {
        if (!assigned_IDs.empty()) {
            id_ = *(assigned_IDs.rbegin()) + 1;
        }
        else {
            id_ = 1;
        }
    }
    assigned_IDs.insert(id_);
}

Package::Package(ElementID id) {
    id_ = id;
    assigned_IDs.insert(id_);
}

Package::Package(Package&& other) {
    id_ = other.id_;
    other.id_ = 0;
}

Package::~Package() {
    if (id_ > 0) {
        freed_IDs.insert(id_);
        assigned_IDs.erase(id_);
    }
}

Package& Package::operator=(Package&& other) {
    if (this == &other) {
        return *this;
    }

    if (id_ > 0) {
        assigned_IDs.erase(id_);
        freed_IDs.insert(id_);
    }
    id_ = other.id_;
    other.id_ = 0;
    return *this;

}