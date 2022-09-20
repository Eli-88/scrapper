#pragma once

#include "handler/healthcheck_handler.h"
#include "handler/registration_handler.h"
#include "protocol/protocol_interface.h"
#include "scrape_manager.h"

#include <core/interface/json_interface.h>
#include <core/interface/net/client_interface.h>
#include <core/net/http_request_handler.h>
#include <core/interface/net/server_interface.h>
#include <iostream>
#include <thread>
#include <vector>

namespace Scrapper {

class Engine {
 public:
  explicit Engine(const char* serverHost, unsigned short serverPort);
  Engine(const Engine&) = delete;

  void Run();

  void Join();

 private:
  std::unique_ptr<Core::Net::IServer> server_;
  std::unique_ptr<Core::Net::IHttpRequestHandlerFactory> requestHandlerFactory_;
  std::unique_ptr<IScrapeManager> scrapeManager_;
  std::vector<std::thread> workGrpThreads_;
  std::unique_ptr<Handler::HealthCheckHandler> healthcheckHandler_;
  std::unique_ptr<Handler::RegistrationHandler> registrationHandler_;
};

}  // namespace Scrapper