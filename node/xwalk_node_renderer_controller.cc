// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/node/xwalk_node_renderer_controller.h"

#include <string>
#include <vector>

#if defined(OS_LINUX)
#include <sys/epoll.h>
#endif

#include "base/command_line.h"
#include "base/bind.h"
#include "base/message_loop/message_loop.h"
#include "third_party/WebKit/public/web/WebScopedMicrotaskSuppression.h"
#include "xwalk/node/node_includes.h"
#include "xwalk/runtime/common/xwalk_switches.h"

#include "xwalk_node_natives.h"  // NOLINT

namespace xwalk {

namespace node {

namespace {

void UvDummyCallback(uv_async_t* handle) {
}

// Convert the given vector to an array of C-strings. The strings in the
// returned vector are only guaranteed valid so long as the vector of strings
// is not modified.
std::unique_ptr<const char*[]> StringVectorToArgArray(
    const std::vector<std::string>& vector) {
  std::unique_ptr<const char*[]> array(new const char*[vector.size()]);
  for (size_t i = 0; i < vector.size(); ++i) {
    array[i] = vector[i].c_str();
  }
  return array;
}

}  // namespace

XwalkNodeRendererController::XwalkNodeRendererController(
    base::FilePath& manifest_path)
    : message_loop_(nullptr),
      uv_loop_(uv_default_loop()),
      uv_polling_stopped_(false),
      node_env_(nullptr),
      manifest_path_(manifest_path),
      weak_factory_(this) {
}

XwalkNodeRendererController::~XwalkNodeRendererController() {
  // Stop the uv polling thread.
  uv_polling_stopped_ = true;
  uv_sem_post(&uv_polling_sem_);
  uv_async_send(&uv_dummy_async_);
  uv_thread_join(&uv_polling_thread_);
  uv_sem_destroy(&uv_polling_sem_);
}

void XwalkNodeRendererController::DidCreateScriptContext(
    blink::WebLocalFrame* frame, v8::Handle<v8::Context> context) {
  // Init node with nullptr means embedding mode.
  ::node::Init(nullptr, nullptr, nullptr, nullptr);

  // Add dummy callback to avoid spin on uv backend fd.
  uv_async_init(uv_loop_, &uv_dummy_async_, UvDummyCallback);

  // Create the uv polling thread and semaphore.
  uv_sem_init(&uv_polling_sem_, 0);
  uv_thread_create(&uv_polling_thread_, UvPollingThreadRunner, this);

  blink::WebScopedMicrotaskSuppression suppression;
  // TODO(nhu): use m51 V8 microtask API.
  std::vector<std::string> args = base::CommandLine::ForCurrentProcess()->argv();
  std::unique_ptr<const char*[]> c_argv = StringVectorToArgArray(args);

  // Create the environment from DOM script context.
  node_env_ = ::node::CreateEnvironment(
      context->GetIsolate(), uv_default_loop(), context,
      args.size(), c_argv.get(), 0, nullptr);

  // Setup the process object.
  node_env_->process_object()->DefineOwnProperty(
      node_env_->context(),
      v8::String::NewFromUtf8(node_env_->isolate(), "_init_native"),
      v8::String::NewFromUtf8(
          node_env_->isolate(),
          reinterpret_cast<const char*>(::node::init_native),
          v8::NewStringType::kNormal,
          sizeof(::node::init_native)).ToLocalChecked()).FromJust();
  node_env_->process_object()->DefineOwnProperty(
      node_env_->context(),
      v8::String::NewFromUtf8(node_env_->isolate(), "_app_manifest_path"),
      v8::String::NewFromUtf8(
          node_env_->isolate(),
          manifest_path_.DirName().MaybeAsASCII().c_str())).FromJust();

  // Execute node.js in this envrionment.
  ::node::LoadEnvironment(node_env_);

  // Keep the message loop of main thread.
  message_loop_ = base::MessageLoop::current();

  // Run uv loop once.
  RunUvLoopNonBlocking();
}

void XwalkNodeRendererController::WillReleaseScriptContext(
    blink::WebLocalFrame* frame, v8::Handle<v8::Context> context) {
  ::node::Environment* env = ::node::Environment::GetCurrent(context);
  std::vector<v8::Local<v8::Value>> args = {
    v8::String::NewFromUtf8(env->isolate(), "exit") };
  blink::WebScopedMicrotaskSuppression suppression;
  // TODO(nhu): use m51 V8 microtask API.
  ::node::MakeCallback(
      env->isolate(), env->process_object(), "emit", args.size(), &args.front());
}

void XwalkNodeRendererController::RunUvLoopNonBlocking() {
  v8::HandleScope handle_scope(node_env_->isolate());
  v8::Context::Scope context_scope(node_env_->context());

  /*
  // Perform microtask checkpoint after running JavaScript.
  std::unique_ptr<v8::MicrotasksScope> script_scope(is_browser_ ?
      nullptr :
      new v8::MicrotasksScope(env->isolate(),
                              v8::MicrotasksScope::kRunMicrotasks));
  */
  blink::WebScopedMicrotaskSuppression suppression;
  // TODO(nhu): use m51 V8 microtask API.

  // Poll for i/o once but don’t block if there are no pending callbacks.
  uv_run(uv_loop_, UV_RUN_NOWAIT);

  // Notify polling thread for next event polling.
  uv_sem_post(&uv_polling_sem_);
}

// static
void XwalkNodeRendererController::UvPollingThreadRunner(void *arg) {
  XwalkNodeRendererController* self =
      static_cast<XwalkNodeRendererController*>(arg);

  while (!self->uv_polling_stopped_) {
#if defined(OS_LINUX)
    int r, timeout, fd;
    do {
      struct epoll_event ev;
      timeout = uv_backend_timeout(self->uv_loop_);
      fd = uv_backend_fd(self->uv_loop_);
      r = epoll_wait(fd, &ev, 1, timeout);
    } while (r == -1 && errno == EINTR);
#endif
    // Run uv event loop in main thread.
    self->message_loop_->PostTask(
        FROM_HERE,
        base::Bind(&XwalkNodeRendererController::RunUvLoopNonBlocking,
                   self->weak_factory_.GetWeakPtr()));
    // Wait for the main loop to notify next polling.
    uv_sem_wait(&self->uv_polling_sem_);
  }
}

}  // node

}  // namespace xwalk
