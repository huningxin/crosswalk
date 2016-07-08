// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

'use strict'

// Reset the module's paths to app root.
module.paths = [];
module.paths = require('module')._nodeModulePaths(process._app_manifest_path);

// Populate to DOM window.
global.__filename = __filename;
global.__dirname = __dirname;
global.exports = exports;
global.module = module;
global.require = require;

return exports
