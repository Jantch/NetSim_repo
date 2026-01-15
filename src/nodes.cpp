#include "nodes.hxx"

void ReceiverPreferences::add_receiver(IPackageReceiver* r){
    preferences_[r] = 1.0;
    double n = preferences_.size();

    if (n > 1){
        for(auto ite = preferences_.begin(); ite != preferences_.end(); ite++){
          ite->second = 1.0/n;
        }
    }
};

void ReceiverPreferences::remove_receiver(IPackageReceiver* r){
    preferences_.erase(r);
    double n = preferences_.size();

    if (n > 0){
      for(auto ite = preferences_.begin(); ite != preferences_.end(); ite++){
        ite->second = 1.0/n;
      }
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver(){
    if (preferences_.empty()){
        return nullptr;
    }

    double random = pg_();
    double sum = 0.0;

    for (auto ite = preferences_.begin(); ite != preferences_.end(); ite++){
        sum += ite->second;
        if(random < sum){
        return ite->first;
        }
    }
   return preferences_.rbegin()->first;
}

void PackageSender::send_package(){
    if(buffer_.has_value()){
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        if(receiver){
            receiver -> receive_package(std::move(*buffer_));
            buffer_.reset();
        }
    }
}
