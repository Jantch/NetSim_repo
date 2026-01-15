#pragma once
#include <map>
#include <optional>

#include "helpers.hxx"
#include "package.hxx"
#include "types.hxx"
#include "storage_types.hxx"

class IPackageReceiver {
  public:
    using const_iterator = IPackageStockpile::const_iterator;
    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;

    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
  public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg) : pg_(pg) {};
    ~ReceiverPreferences() = default;

    const preferences_t& get_preferences() const { return preferences_; };
    IPackageReceiver* choose_receiver();

    void remove_receiver(IPackageReceiver* r);
    void add_receiver(IPackageReceiver* r);

    const_iterator begin() const { return preferences_.begin(); }
    const_iterator cbegin() const { return preferences_.cbegin(); }
    const_iterator end() const { return preferences_.end(); }
    const_iterator cend() const { return preferences_.cend(); }
  private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;
};

class PackageSender {
  public:
    ReceiverPreferences receiver_preferences_;
    PackageSender(PackageSender&&) = default;

    const std::optional<Package>& get_sending_buffer() const {return buffer_;};
    void send_package();

  protected:
    void push_package(Package&& p){buffer_ = std::move(p);};

  private:
    std::optional<Package> buffer_;
};