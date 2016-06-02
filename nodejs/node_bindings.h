// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

// The Crosswalk adaption code is under:
// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_NODEJS_NODE_BINDINGS_H_
#define XWALK_NODEJS_NODE_BINDINGS_H_

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

namespace nodejs {

class NodeBindings {
 public:
  static NodeBindings* Create();

  virtual ~NodeBindings();

  // To be called in XWalkContentRendererClient so we can create and
  // destroy extensions contexts appropriatedly.
  void DidCreateScriptContext(blink::WebLocalFrame* frame,
                              v8::Handle<v8::Context> context);
  void WillReleaseScriptContext(blink::WebLocalFrame* frame,
                                v8::Handle<v8::Context> context);

 protected:
  // Setup V8, libuv.
  void Initialize();

  // Create the environment and load node.js.
  node::Environment* CreateEnvironment(v8::Handle<v8::Context> context);

  // Load node.js in the environment.
  void LoadEnvironment(node::Environment* env);

  // Prepare for message loop integration.
  void PrepareMessageLoop();

  // Do message loop integration.
  virtual void RunMessageLoop();

  // Gets/sets the environment to wrap uv loop.
  void set_uv_env(node::Environment* env) { uv_env_ = env; }
  node::Environment* uv_env() const { return uv_env_; }

  explicit NodeBindings();

  // Called to poll events in new thread.
  virtual void PollEvents() = 0;

  // Run the libuv loop for once.
  void UvRunOnce();

  // Make the main thread run libuv loop.
  void WakeupMainThread();

  // Interrupt the PollEvents.
  void WakeupEmbedThread();

  // Main thread's MessageLoop.
  base::MessageLoop* message_loop_;

  // Main thread's libuv loop.
  uv_loop_t* uv_loop_;

 private:
  // Thread to poll uv events.
  static void EmbedThreadRunner(void *arg);

  // Whether the libuv loop has ended.
  bool embed_closed_;

  // Dummy handle to make uv's loop not quit.
  uv_async_t dummy_uv_handle_;

  // Thread for polling events.
  uv_thread_t embed_thread_;

  // Semaphore to wait for main loop in the embed thread.
  uv_sem_t embed_sem_;

  // Environment that to wrap the uv loop.
  node::Environment* uv_env_;

  base::WeakPtrFactory<NodeBindings> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(NodeBindings);
};

}  // namespace nodejs

}  // namespace xwalk

#endif  // XWALK_NODEJS_NODE_BINDINGS_H_
