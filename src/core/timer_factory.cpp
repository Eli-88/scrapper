#include "core/timer_factory.h"
#include "timer.h"

#include <boost/asio/io_context.hpp>

namespace Core {

std::shared_ptr<ITimer> TimerFactory::CreateTimer(
    Net::IContextManager& contextManager,
    const ITimer::Work& work,
    size_t timeoutInSeconds) {
  return std::make_shared<Timer>(contextManager.IoContext(), work,
                                 timeoutInSeconds);
}
}  // namespace Core