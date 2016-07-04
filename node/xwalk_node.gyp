{
  'targets': [
    {
      'variables': {
        'chromium_code': 1,
      },
      'target_name': 'xwalk_node',
      'type': 'static_library',
      'dependencies': [
        'xwalk_node_js2c',
      ],
      'defines': ['ENABLE_NODE'],
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
        '<(SHARED_INTERMEDIATE_DIR)',
      ],
      'sources': [
        'node_array_buffer_bridge.cc',
        'node_array_buffer_bridge.h',
        'node_bindings.cc',
        'node_bindings.h',
        'node_bindings_linux.cc',
        'node_bindings_linux.h',
        'node_bindings_win.cc',
        'node_bindings_win.h',
        'node_includes.h',
      ],
    },
    {
      'target_name': 'xwalk_node_js2c',
      'type': 'none',
      'actions': [
        {
          'action_name': 'xwalk_node_js2c',
          'inputs': [
            'init.js',
          ],
          'outputs': [
            '<(SHARED_INTERMEDIATE_DIR)/xwalk_node_natives.h',
          ],
          'action': [
            'python',
            '<(DEPTH)/third_party/node/tools/js2c.py',
            '<@(_outputs)',
            '<@(_inputs)',
          ],
        },
      ],
    }, # end node_js2c
  ],
}
