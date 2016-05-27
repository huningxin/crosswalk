// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef XWALK_NODEJS_NODE_BINDINGS_LINUX_H_
#define XWALK_NODEJS_NODE_BINDINGS_LINUX_H_

#include "base/compiler_specific.h"
#include "xwalk/nodejs/node_bindings.h"

namespace xwalk {

namespace nodejs {

class NodeBindingsLinux : public NodeBindings {
 public:
  explicit NodeBindingsLinux(bool is_browser);
  ~NodeBindingsLinux() override;

  void RunMessageLoop() override;

 private:
  // Called when uv's watcher queue changes.
  static void OnWatcherQueueChanged(uv_loop_t* loop);

  void PollEvents() override;

  // Epoll to poll for uv's backend fd.
  int epoll_;

  DISALLOW_COPY_AND_ASSIGN(NodeBindingsLinux);
};

}  // namespace nodejs

}  // namespace xwalk

#endif  // XWALK_NODEJS_NODE_BINDINGS_LINUX_H_
