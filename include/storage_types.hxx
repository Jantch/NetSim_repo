#include <cstddef>
#include <list>
#include <string>

#include "package.hxx"

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package&&) = 0;
    virtual bool empty() = 0;
    virtual std::size_t size() = 0;
    virtual const_iterator begin() = 0;
    virtual const_iterator end() = 0;
    virtual const_iterator cbegin() = 0;
    virtual const_iterator cend() = 0;

    virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile {
    public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
};

class PackageQueue : public IPackageQueue {
public:
    PackageQueue(PackageQueueType type) : queue_type_(type) {}

private:
    std::list<Package> queue_;
    PackageQueueType queue_type_;
};