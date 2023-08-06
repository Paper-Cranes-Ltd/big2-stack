//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_APP_EXTENSION_BASE_H_
#define BIG2_STACK_APP_EXTENSION_BASE_H_

#include <bgfx/bgfx.h>
#include <gsl/pointers>

namespace big2 {

class App;
class Window;

class AppExtensionBase {
 public:

 protected:
  virtual void OnInitialize() {};
  virtual void OnTerminate() {};
  virtual void OnWindowCreated(Window& window) {};
  virtual void OnWindowDestroyed(Window& window) {};
  virtual void OnFrameBegin() {};
  virtual void OnWindowUpdate(Window& window) {};
  virtual void OnFrameEnd() {};

  App *app_ = nullptr;

 private:
  void Initialize(App *app);

  friend class App;
};

}

#endif //BIG2_STACK_APP_EXTENSION_BASE_H_
