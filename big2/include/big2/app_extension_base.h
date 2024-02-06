//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_APP_EXTENSION_BASE_H_
#define BIG2_STACK_APP_EXTENSION_BASE_H_

#include <bgfx/bgfx.h>
#include <gsl/pointers>
#include <cmath>

namespace big2 {

class App;
class Window;

class AppExtensionBase {
 public:
  virtual ~AppExtensionBase() = default;
 protected:
  virtual void OnInitialize() {};
  virtual void OnTerminate() {};
  virtual void OnWindowCreated(Window& window) {};
  virtual void OnWindowDestroyed(Window& window) {};
  virtual void OnFrameBegin() {};
  virtual void OnUpdate(std::float_t dt) {};
  virtual void OnRender(Window& window) {};
  virtual void OnFrameEnd() {};

  App *app_ = nullptr;

 private:
  void Initialize(App *app);

  friend class App;
};

}

#endif //BIG2_STACK_APP_EXTENSION_BASE_H_
