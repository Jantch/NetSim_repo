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
    virtual ReceiverType get_receiver_type() const = 0;

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