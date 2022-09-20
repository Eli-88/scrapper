#include "timer.h"
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>

#include <atomic>
#include <chrono>
#include <iostream>

#define TIMER_IMPL(context) impl_->context

namespace Core {

struct Timer::Impl {
  ~Impl() {}
  Impl(boost::asio::io_context& ioContext,
       const Work& work,
       size_t timeoutInSeconds)
      : timer(ioContext, boost::posix_time::seconds(timeoutInSeconds)),
        work(work),
        timeout(timeoutInSeconds) {}

  boost::asio::deadline_timer timer;
  Work work;
  size_t timeout;
};

Timer::~Timer() {}

Timer::Timer(boost::asio::io_context& ioContext,
             const Work& work,
             size_t timeoutInSeconds)
    : impl_(std::make_unique<Impl>(ioContext, work, timeoutInSeconds)) {}

void Timer::Run() {
  StartTimeOut();
}

void Timer::Cancel() {
  TIMER_IMPL(timer).cancel();
}

void Timer::StartTimeOut() {
  impl_->timer.expires_from_now(boost::posix_time::seconds(impl_->timeout));
  auto self{this->shared_from_this()};
  impl_->timer.async_wait([this, self](const boost::system::error_code& ec) {
    if (ec) {
      return;
    }

    TIMER_IMPL(work)();
    StartTimeOut();
  });
}

}  // namespace Core