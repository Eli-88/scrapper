#include "config.h"
#include <core/logger.h>
#include <boost/program_options.hpp>

#define IMPL(field) impl_->field

namespace Scrapper {
namespace po = boost::program_options;

struct Config::PrivateImpl {
  std::string Host;
  unsigned short Port;
};

std::unique_ptr<IConfig> Config::New(int argc, char const** argv) {
  return std::make_unique<Config>(argc, argv);
}

Config::~Config() {}

Config::Config(int argc, char const** argv)
    : impl_(std::make_unique<PrivateImpl>()) {
  po::variables_map vm;
  po::options_description description("My Scrapper");

  description.add_options()("host",
                            po::value<std::string>()->default_value("0.0.0.0"),
                            "ip for the service")(
      "port", po::value<unsigned short>(), "port number for the service");

  po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
  po::notify(vm);

  if (!vm.count("port")) {
    throw std::runtime_error(
        "host and port arguement is needed. i.e. ./exe --host <ip addr> --port "
        "<port number>");
  }

  IMPL(Host) = vm["host"].as<std::string>();
  IMPL(Port) = vm["port"].as<unsigned short>();
  LOG_DEBUG("ip: %s:%d", IMPL(Host).c_str(), IMPL(Port));
}

const char* Config::Host() const {
  return IMPL(Host).c_str();
}

unsigned short Config::Port() const {
  return IMPL(Port);
}
}  // namespace Scrapper