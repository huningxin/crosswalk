{
  'targets': [
    {
      'target_name': 'xwalk_box2d',
      'type': 'static_library',
      'dependencies': [
        '../../base/base.gyp:base',
        'third_party/box2d/box2d.gyp:box2d',
        '../extensions/extensions.gyp:xwalk_extensions',
      ],
      'sources': [
        'xwalk_box2d_module.cc',
        'xwalk_box2d_module.h',
      ],
      'include_dirs': [
        'third_party/box2d/',
      ],
    },
  ],
}
