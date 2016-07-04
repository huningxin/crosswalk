'use strict'

// reset-search-path.js

const path = require('path');
const Module = require('module');

// Clear Node's global search paths.
Module.globalPaths.length = 0;

// Clear current and parent(init.js)'s search paths.
module.paths = [];

// Prevent Node from adding paths outside this app to search paths.
Module._nodeModulePaths = function (from) {
  from = path.resolve(from)

  // If "from" is outside the app then we do nothing.
  const skipOutsidePaths = from.startsWith(process._app_manifest_path)

  // Following logic is copied from module.js.
  const splitRe = process.platform === 'win32' ? /[\/\\]/ : /\//
  const paths = []
  const parts = from.split(splitRe)

  let tip
  let i
  for (tip = i = parts.length - 1; i >= 0; tip = i += -1) {
    const part = parts[tip]
    if (part === 'node_modules') {
      continue
    }
    const dir = parts.slice(0, tip + 1).join(path.sep)
    if (skipOutsidePaths && !dir.startsWith(process._app_manifest_path)) {
      break
    }
    paths.push(path.join(dir, 'node_modules'))
  }
  return paths
}

// renderer/init.js
global.require = require;
global.module = module;
global.__filename = __filename;
global.__dirname = __dirname;

return exports;
