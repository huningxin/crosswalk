// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

'use strict'

// Reset the module's paths to app root.
module.paths = [];
module.paths = require('module')._nodeModulePaths(process._app_manifest_path);

// setImmediate uses libuv Check handle (uv_check_init/uv_check_start) to schedule
// the callbacks. libuv doc says "the Check handles will run the given callback
// once per loop iteration, right after polling for i/o.". Sometimes, there is no
// real i/o events when running Node.js in Crosswalk. So the workaround is to wakeup
// the uv loop explicitly after setImmediate. This workaround has negative impact to
// performance.
// process.nextTick queues the callbacks till next MakeCallback called. It is not
// triggered by real i/o either. So use the same workaround as setImmediate.
var wakeupUvLoopWrapper = function (func) {
  return function () {
    process._wakeupUvLoop();
    return func.apply(this, arguments);
  };
}

process.nextTick = wakeupUvLoopWrapper(process.nextTick)

// Populate timers to DOM window.
const timers = require('timers');
global.setImmediate = wakeupUvLoopWrapper(timers.setImmediate)
global.clearImmediate = timers.clearImmediate

// Populate to DOM window.
global.__filename = __filename;
global.__dirname = __dirname;
global.exports = exports;
global.module = module;
global.require = require;

return exports
