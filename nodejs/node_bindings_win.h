// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

// The Crosswalk adaption code is under:
// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_NODEJS_NODE_BINDINGS_WIN_H_
#define XWALK_NODEJS_NODE_BINDINGS_WIN_H_

#include "base/compiler_specific.h"
#include "xwalk/nodejs/node_bindings.h"

namespace xwalk {

namespace nodejs {

class NodeBindingsWin : public NodeBindings {
 public:
  explicit NodeBindingsWin(base::FilePath& manifest_path);
  virtual ~NodeBindingsWin();

 private:
  void PollEvents() override;

  DISALLOW_COPY_AND_ASSIGN(NodeBindingsWin);
};

}  // namespace nodejs

}  // namespace xwalk

#endif  // XWALK_NODEJS_NODE_BINDINGS_WIN_H_
