// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_NODE_XWALK_NODE_RENDERER_CONTROLLER_H_
#define XWALK_NODE_XWALK_NODE_RENDERER_CONTROLLER_H_

#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/memory/weak_ptr.h"
#include "v8/include/v8.h"
#include "third_party/node/deps/uv/include/uv.h"
#include "third_party/WebKit/public/web/WebFrame.h"

namespace base {
class MessageLoop;
}

namespace node {
class Environment;
}

namespace xwalk {

namespace node {

class XWalkNodeRendererController {
 public:
  explicit XWalkNodeRendererController(base::FilePath& manifest_path);
  virtual ~XWalkNodeRendererController();

  // To be called in XWalkContentRendererClient so we can create and
  // destroy node environment appropriatedly.
  void DidCreateScriptContext(blink::WebLocalFrame* frame,
                              v8::Handle<v8::Context> context);
  void WillReleaseScriptContext(blink::WebLocalFrame* frame,
                                v8::Handle<v8::Context> context);

  // Start a node process for child_process.fork usage.
  static void StartNodeProcess();

 private:
  void Init();

  // Run the uv event loop without blocking.
  void RunUvLoopNonBlocking();

  // Poll the uv events in uv_polling_thread_.
  static void UvPollingThreadRunner(void *arg);

  // Allow to wakup uv loop from JavaScript.
  static void JsWakeupUvLoop(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void WakeupUvLoopCallback(uv_async_t* handle);
  static uv_async_t uv_wakeup_async_;

  // Initialize once;
  bool initialized_;

  // Message loop of main thread.
  base::MessageLoop* message_loop_;

  // Members for uv event loop.
  uv_loop_t* uv_loop_;
  bool uv_polling_stopped_;
  uv_thread_t uv_polling_thread_;
  uv_sem_t uv_polling_sem_;

  // The node environment of this context.
  ::node::Environment* node_env_;

  // The path of app's manifest file.
  base::FilePath manifest_path_;

  base::WeakPtrFactory<XWalkNodeRendererController> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(XWalkNodeRendererController);
};

}  // namespace node

}  // namespace xwalk

#endif  // XWALK_NODE_XWALK_NODE_RENDERER_CONTROLLER_H_
