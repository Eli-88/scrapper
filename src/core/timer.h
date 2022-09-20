#pragma once

#include "core/interface/timer_interface.h"

#include <memory>

namespace boost {
  namespace asio {
    class io_context;
  }
}

namespace Core {
class Timer final : public ITimer {
 public:
  explicit Timer(boost::asio::io_context& ioContext, const Work& work, size_t timeoutInSeconds);
  virtual ~Timer();
  virtual void Run() override;
  virtual void Cancel() override;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
  void StartTimeOut();
};
}  // namespace Core