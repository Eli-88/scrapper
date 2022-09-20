#pragma once

#include "core/interface/net/context_manager_interface.h"

#include <functional>
#include <memory>

namespace Core {
class ITimer : public std::enable_shared_from_this<ITimer> {
 public:
  using Work = std::function<void()>;

  virtual ~ITimer() = default;
  virtual void Run() = 0;
  virtual void Cancel() = 0;
};

class ITimerFactory {
 public:
  virtual ~ITimerFactory() = default;
  virtual std::shared_ptr<ITimer> CreateTimer(
      Net::IContextManager& contextManager,
      const ITimer::Work& work,
      size_t timeoutInSeconds) = 0;
};
}  // namespace Core