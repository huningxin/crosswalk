// Copyright (c) 2015 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

// The Crosswalk adaption code is under:
// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_NODEJS_NODE_ARRAY_BUFFER_BRIDGE_H_
#define XWALK_NODEJS_NODE_ARRAY_BUFFER_BRIDGE_H_

namespace xwalk {

namespace nodejs{

// Override Node's ArrayBuffer with DOM's ArrayBuffer.
void OverrideNodeArrayBuffer();

}  // namespace nodejs
}  // namespace xwalk

#endif  // XWALK_NODEJS_NODE_ARRAY_BUFFER_BRIDGE_H_
