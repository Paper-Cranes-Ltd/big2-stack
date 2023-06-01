//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/bgfx/bgfx_view_scoped.h>
#include <big2/bgfx/bgfx_utils.h>

namespace big2 {

BgfxViewScoped::BgfxViewScoped() {
  view_id_ = ReserveViewId();
}

BgfxViewScoped::~BgfxViewScoped() {
  bgfx::resetView(view_id_);
  FreeViewId(view_id_);
}

BgfxViewScoped::BgfxViewScoped(bgfx::ViewId view_id) : view_id_(view_id) {

}

BgfxViewScoped::operator bgfx::ViewId() const {
  return view_id_;
}

}
