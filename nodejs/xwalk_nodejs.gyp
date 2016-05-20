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
        '<(DEPTH)/third_party/WebKit',
        '<(DEPTH)/third_party/WebKit/Source',
      ],
      'sources': [
        'nodejs_content.cc',
        'nodejs_content.h',
      ],
    },
  ],
}
