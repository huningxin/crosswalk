{
  'targets': [
    {
      'variables': {
        'chromium_code': 1,
      },
      'target_name': 'xwalk_nodejs',
      'type': 'static_library',
      'include_dirs': [
        '..',
        '../..',
        '<(DEPTH)/third_party/node/src',
        '<(DEPTH)/third_party/node/deps/http_parser',
        '<(DEPTH)/third_party/node/deps/uv/include',
        '<(DEPTH)/third_party/node/deps/cares/include',
        '<(DEPTH)/v8/include',
        '<(DEPTH)/third_party/WebKit/',
        '<(DEPTH)/third_party/WebKit/Source',
      ],
      'sources': [
        'node_bindings.cc',
        'node_bindings.h',
        'node_bindings_linux.cc',
        'node_bindings_linux.h',
        'node_bindings_win.cc',
        'node_bindings_win.h',
        'node_includes.h',
      ],
    },
  ],
}
