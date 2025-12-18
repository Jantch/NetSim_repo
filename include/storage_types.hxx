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
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

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
    void push(Package&&) override;
    bool empty() override {return queue_.empty();};
    std::size_t size() override {return queue_.size();};
    const_iterator begin() override {return queue_.begin();};
    const_iterator end() override {return queue_.end();};
    const_iterator cbegin() const override {return queue_.cbegin();};
    const_iterator cend() const override {return queue_.cend();};

    Package pop();
    PackageQueueType get_queue_type() {return queue_type_;};
private:
    std::list<Package> queue_;
    PackageQueueType queue_type_;
};