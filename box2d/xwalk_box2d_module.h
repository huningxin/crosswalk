// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_BOX2D_XWALK_BOX2D_MODULE_H_
#define XWALK_BOX2D_XWALK_BOX2D_MODULE_H_

#include "xwalk/extensions/renderer/xwalk_module_system.h"

namespace xwalk {
namespace extensions {

class XWalkBox2DModule : public XWalkNativeModule {
 public:
  XWalkBox2DModule();
  virtual ~XWalkBox2DModule();

 private:
  virtual v8::Handle<v8::Object> NewInstance() OVERRIDE;

  v8::Persistent<v8::ObjectTemplate> object_template_;
};

}  // namespace extensions
}  // namespace xwalk

#endif  // XWALK_BOX2D_XWALK_BOX2D_MODULE_H_
