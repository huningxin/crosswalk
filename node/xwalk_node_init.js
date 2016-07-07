// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

'use strict'

// Reset the module's paths to app root.
module.paths = [];
module.paths = require('module')._nodeModulePaths(process._app_manifest_path);
