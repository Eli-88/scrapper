#include "engine.h"
#include "protocol/json_decoder.h"
#include "scrape_manager.h"

#include <core/algorithm.h>
#include <core/json.h>
#include <core/logger.h>

#include <iostream>

namespace Scrapper {

namespace Net = Core::Net;

Engine::Engine(const char* serverHost, unsigned short serverPort)
    : requestHandlerFactory_(
          std::make_unique<Net::HttpRequestHandlerFactory>()),
      scrapeManager_(std::make_unique<ScrapeManager>()),
      healthcheckHandler_(std::make_unique<Handler::HealthCheckHandler>()),
      registrationHandler_(
          std::make_unique<Handler::RegistrationHandler>(*scrapeManager_)) {
  server_ =
      Net::CreateHttpServer(*requestHandlerFactory_, serverHost, serverPort);

  requestHandlerFactory_->RegisterHandler(Net::HttpVerb::get, "/scrap",
                                          *registrationHandler_);
  requestHandlerFactory_->RegisterHandler(Net::HttpVerb::get, "/health_check",
                                          *healthcheckHandler_);
}

void Engine::Run() {
  workGrpThreads_.emplace_back(std::thread([this]() {
    LOG_INFO("Starting Http Server...");
    try {
      this->server_->Run();
    } catch (std::exception ex) {
      LOG_WARN("Http Server Error: %s", ex.what());
    }
    LOG_INFO("Exiting Http Server...");
  }));
  workGrpThreads_.emplace_back(
      std::thread([this]() { this->scrapeManager_->Run(); }));
}

void Engine::Join() {
  for (auto& t : workGrpThreads_) {
    t.join();
  }
}

}  // namespace Scrapper