// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

// The Crosswalk adaption code is under:
// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_NODEJS_NODE_BINDINGS_LINUX_H_
#define XWALK_NODEJS_NODE_BINDINGS_LINUX_H_

#include "base/compiler_specific.h"
#include "xwalk/nodejs/node_bindings.h"

namespace xwalk {

namespace nodejs {

class NodeBindingsLinux : public NodeBindings {
 public:
  explicit NodeBindingsLinux();
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
