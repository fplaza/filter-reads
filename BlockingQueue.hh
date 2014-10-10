#ifndef BLOCKING_QUEUE_HH
#define BLOCKING_QUEUE_HH
 
#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp> 

template <typename T>
class BlockingQueue
{
private:
	std::queue<T> queue_;
	const size_t max_size_;
	boost::mutex mutex_;
	boost::condition_variable not_empty_;
	boost::condition_variable not_full_;

public:
	BlockingQueue<T>(size_t max_size);
	void enqueue(const T& val);
	T dequeue();
};

template <typename T>
BlockingQueue<T>::BlockingQueue(size_t max_size)
	: queue_(), max_size_(max_size), mutex_(), not_empty_(), not_full_()

{};

template <typename T>
void BlockingQueue<T>::enqueue(const T& val)
{
    boost::unique_lock<boost::mutex> lock(mutex_);

	if (queue_.size() == max_size_)
		not_full_.wait(lock);

	queue_.push(val);
	not_empty_.notify_one();
}

template <typename T>
T BlockingQueue<T>::dequeue()
{
	boost::unique_lock<boost::mutex> lock(mutex_);
	
	if (queue_.empty())
		not_empty_.wait(lock);

	T val = queue_.front();
	queue_.pop();
	not_full_.notify_one();

	return val;
}

#endif // BLOCKING_QUEUE_HH
