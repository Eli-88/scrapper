#pragma once

#include "interface/config_interface.h"

#include <memory>
#include <string>

namespace Scrapper {

class Config : public IConfig {
 public:
  static std::unique_ptr<IConfig> New(int argc, char const** argv);
  virtual ~Config() override;
  Config(int argc, char const** argv);
  virtual const char* Host() const override;
  virtual unsigned short Port() const override;

 private:
  struct PrivateImpl;
  std::unique_ptr<PrivateImpl> impl_;
};

}  // namespace Scrapper