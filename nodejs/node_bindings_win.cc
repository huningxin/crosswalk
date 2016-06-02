// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

// The Crosswalk adaption code is under:
// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/nodejs/node_bindings_win.h"

#include <windows.h>

#include "base/logging.h"

extern "C" {
#include "vendor/node/deps/uv/src/win/internal.h"
}

namespace xwalk {

namespace nodejs {

NodeBindingsWin::NodeBindingsWin(base::FilePath& manifest_path)
    : NodeBindings(manifest_path) {
}

NodeBindingsWin::~NodeBindingsWin() {
}

void NodeBindingsWin::PollEvents() {
  // If there are other kinds of events pending, uv_backend_timeout will
  // instruct us not to wait.
  DWORD bytes, timeout;
  ULONG_PTR key;
  OVERLAPPED* overlapped;

  timeout = uv_backend_timeout(uv_loop_);

  GetQueuedCompletionStatus(uv_loop_->iocp,
                            &bytes,
                            &key,
                            &overlapped,
                            timeout);

  // Give the event back so libuv can deal with it.
  if (overlapped != NULL)
    PostQueuedCompletionStatus(uv_loop_->iocp,
                               bytes,
                               key,
                               overlapped);
}

// static
NodeBindings* NodeBindings::Create(base::FilePath& manifest_path) {
  return new NodeBindingsWin(manifest_path);
}

}  // namespace nodejs

}  // namespace xwalk
