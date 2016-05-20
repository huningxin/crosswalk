// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_NODEJS_NODEJS_CONTENT_H_
#define XWALK_NODEJS_NODEJS_CONTENT_H_

#include "third_party/WebKit/public/web/WebFrame.h"
#include "v8/include/v8.h"

namespace blink {
  class WebLocalFrame;
}

namespace xwalk {
namespace nodejs {

void LoadNodeSymbols();
void DidCreateScriptContext(blink::WebLocalFrame* frame,
                            v8::Handle<v8::Context> context);
void WillReleaseScriptContext(blink::WebLocalFrame* frame,
                              v8::Handle<v8::Context> context);

}  // namespace nodejs
}  // namespace xwalk

#endif  // XWALK_NODEJS_NODEJS_CONTENT_H_