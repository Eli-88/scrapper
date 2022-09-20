#include "core/threadpool.h"

#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>

#define THREADPOOL_IMPL(context) impl_->context

namespace Core {

using thread_pool = boost::asio::thread_pool;

struct ThreadPool::Impl {
  Impl(std::size_t threadCount) : threadPool(threadCount) {}
  thread_pool threadPool;
};

ThreadPool::ThreadPool(std::size_t threadCount)
    : impl_(std::make_unique<Impl>(threadCount)) {}

ThreadPool::~ThreadPool() {}

void ThreadPool::Post(Work work) {
  boost::asio::post(THREADPOOL_IMPL(threadPool), work);
}

void ThreadPool::Stop() {
  THREADPOOL_IMPL(threadPool).stop();
}
}  // namespace Core