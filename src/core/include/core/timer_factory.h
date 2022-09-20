#pragma once

#include "core/interface/timer_interface.h"

namespace Core {
class TimerFactory final : public ITimerFactory {
 public:
  virtual ~TimerFactory() = default;
  virtual std::shared_ptr<ITimer> CreateTimer(
      Net::IContextManager& contextManager,
      const ITimer::Work& work,
      size_t timeoutInSeconds) override;
};
}  // namespace Core