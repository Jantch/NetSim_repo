#include "storage_types.hxx"

void PackageQueue::push(Package&& package) {
    queue_.emplace_back(std::move(package));
}

Package PackageQueue::pop() {
    Package package;

    if (!(queue_.empty())){
        if (queue_type_ == PackageQueueType::FIFO) {
            package = std::move(queue_.front());
            queue_.pop_front();
        }
        if (queue_type_ == PackageQueueType::LIFO) {
            package = std::move(queue_.back());
            queue_.pop_back();
        }
    }
    return package;
}
