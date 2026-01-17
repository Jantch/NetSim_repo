#pragma once
#include <map>
#include <memory>
#include <optional>
#include "package.hxx"
#include "storage_types.hxx"
#include "helpers.hxx"
#include "types.hxx"

enum class ReceiverType {
    WORKER,
    STOREHOUSE
};

class IPackageReceiver {
public:
    using const_iterator = IPackageStockpile::const_iterator;

    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;

    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;

    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator);

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    const preferences_t& get_preferences() const;

    const_iterator cbegin() const;
    const_iterator cend() const;
    const_iterator begin() const;
    const_iterator end() const;

private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;
};

class PackageSender {
    public:
    PackageSender(PackageSender&&) = default;
    PackageSender() : receiver_preferences_(probability_generator) {}
    ReceiverPreferences receiver_preferences_;

    void send_package();
    const std::optional<Package>& get_sending_buffer() const { return buffer_;}
    protected:
    void push_package(Package&& p) {buffer_ = std::move(p);}
private:
    std::optional<Package> buffer_;
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}

    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const { return di_; }
    ElementID get_id() const { return id_; }

private:
    ElementID id_;
    TimeOffset di_;
};

class Worker : public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue);

    void receive_package(Package&& p) override;
    ElementID get_id() const override { return id_; }

    const_iterator cbegin() const override { return queue_->cbegin(); }
    const_iterator cend() const override { return queue_->cend(); }
    const_iterator begin() const override { return queue_->begin(); }
    const_iterator end() const override { return queue_->end(); }

    void do_work(Time t);
    TimeOffset get_processing_duration() const { return pd_; }
    Time get_package_processing_start_time() const { return t_start_; }

private:
    ElementID id_;
    TimeOffset pd_;
    Time t_start_ = 0;
    std::unique_ptr<IPackageQueue> queue_;
    std::optional<Package> processing_buffer_;
};

class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO));

    void receive_package(Package&& p) override;
    ElementID get_id() const override { return id_; }

    const_iterator cbegin() const override { return d_->cbegin(); }
    const_iterator cend() const override { return d_->cend(); }
    const_iterator begin() const override { return d_->begin(); }
    const_iterator end() const override { return d_->end(); }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};