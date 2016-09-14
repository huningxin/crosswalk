// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_NODEJS_NODE_INCLUDES_H_
#define XWALK_NODEJS_NODE_INCLUDES_H_

#include "base/logging.h"

// Include common headers for using node APIs.

#define BUILDING_NODE_EXTENSION
#define NODE_WANT_INTERNALS 1

#undef ASSERT
#undef CHECK
#undef CHECK_EQ
#undef CHECK_NE
#undef CHECK_GE
#undef CHECK_GT
#undef CHECK_LE
#undef CHECK_LT
#undef DISALLOW_COPY_AND_ASSIGN
#undef NO_RETURN
#undef arraysize
#undef debug_string  // This is defined in OS X 10.9 SDK in AssertMacros.h.
#undef UNLIKELY
#include "third_party/node/src/env.h"
#include "third_party/node/src/env-inl.h"
#include "third_party/node/src/node.h"
#include "third_party/node/src/node_buffer.h"
#include "third_party/node/src/node_internals.h"

#endif  // XWALK_NODEJS_NODE_INCLUDES_H_
