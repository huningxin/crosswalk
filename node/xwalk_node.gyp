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
        'node_includes.h',
        'xwalk_node_renderer_controller.cc',
        'xwalk_node_renderer_controller.h',
      ],
      'conditions': [
        [ 'OS=="linux"', {
          'cflags': ['-Wno-ignored-attributes'],
        }],
      ],
    },
    {
      'target_name': 'xwalk_node_js2c',
      'type': 'none',
      'actions': [
        {
          'action_name': 'xwalk_node_js2c',
          'inputs': [
            'xwalk_node_init.js',
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
