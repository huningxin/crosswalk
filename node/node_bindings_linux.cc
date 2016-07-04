// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

// The Crosswalk adaption code is under:
// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/node/node_bindings_linux.h"

#include <sys/epoll.h>

namespace xwalk {

namespace nodejs {

NodeBindingsLinux::NodeBindingsLinux(base::FilePath& manifest_path)
    : NodeBindings(manifest_path),
      epoll_(epoll_create(1)) {
  int backend_fd = uv_backend_fd(uv_loop_);
  struct epoll_event ev = { 0 };
  ev.events = EPOLLIN;
  ev.data.fd = backend_fd;
  epoll_ctl(epoll_, EPOLL_CTL_ADD, backend_fd, &ev);
}

NodeBindingsLinux::~NodeBindingsLinux() {
}

void NodeBindingsLinux::RunMessageLoop() {
  // Get notified when libuv's watcher queue changes.
  //uv_loop_->data = this;
  //uv_loop_->on_watcher_queue_updated = OnWatcherQueueChanged;

  NodeBindings::RunMessageLoop();
}

// static
void NodeBindingsLinux::OnWatcherQueueChanged(uv_loop_t* loop) {
  NodeBindingsLinux* self = static_cast<NodeBindingsLinux*>(loop->data);

  // We need to break the io polling in the epoll thread when loop's watcher
  // queue changes, otherwise new events cannot be notified.
  self->WakeupEmbedThread();
}

void NodeBindingsLinux::PollEvents() {
  int timeout = uv_backend_timeout(uv_loop_);

  // Wait for new libuv events.
  int r;
  do {
    struct epoll_event ev;
    r = epoll_wait(epoll_, &ev, 1, timeout);
  } while (r == -1 && errno == EINTR);
}

// static
NodeBindings* NodeBindings::Create(base::FilePath& manifest_path) {
  return new NodeBindingsLinux(manifest_path);
}

}  // namespace nodejs

}  // namespace xwalk
