# This file is similar to src/build/common.gypi. It contains common variable
# definitions and other actions that are then used in other build system files.
#
# The values can be overriden on the GYP command line (-D) or by setting them
# in ~/.gyp/include.gypi.
#
# IMPORTANT: Like src/build/common.gypi, this file must not be included by
# other .gyp or .gypi files, it is included automatically when gyp_xwalk is
# called. This file is always included immediately after src/build/common.gypi
# itself so that variables originally set there can have their values
# overridden here.

{
  # Organization of the variables below:
  # 1. Variables copied from inner scopes.
  # 2. New variables defined in current scope.
  # 3. Conditions.
  # Some variables are new, but most come from other .gyp files and are
  # overridden here. The default values should match the ones in the original
  # .gyp files.
  'variables': {
    # Putting a variables dict inside another variables dict looks kind of
    # weird. This is caused by the way GYP variable expansion and evaluation
    # works: one cannot set a variable in a scope and use it in a conditional
    # block in the same scope level. For example, setting |mediacodecs_EULA|
    # and using it in a conditional in the same scope does not work. Similarly,
    # since this file is automatically included and at the root level into all
    # other .gyp and .gypi files their own |variables| section might have a
    # check for a variable set in a condition block below, so we need to put
    # variables such as |enable_widevine| into an inner scope to avoid having
    # the conditions block here be at the same level as one from another file.
    # Lastly, we have some extra conditional blocks (like for |use_sysroot|)
    # because src/build/common.gypi might define some variables in nested
    # blocks and to override their value we must set them at least within the
    # same scope level.
    'variables': {
      'variables': {
        'variables': {
          # From src/build/common.gypi.
          # Android and Windows do not use the sysroot, and we cannot use it on
          # Linux because we use libnotify that is not present there.
          'use_sysroot%': 0,
        },
        'use_sysroot%': '<(use_sysroot)',

        # From src/build/common.gypi.
        # Which target type to build most targets as.
        'component%': 'static_library',

        # This variable used to be interpreted in the gyp_xwalk script and
        # translated into changing |ffmpeg_branding|. We keep it here for
        # compatibility for now.
        # TODO(rakuco): Remove this after a while.
        'mediacodecs_EULA%': 0,
      },
      'component%': '<(component)',
      'mediacodecs_EULA%': '<(mediacodecs_EULA)',
      'use_sysroot%': '<(use_sysroot)',

      # Whether to disable NaCl support.
      'disable_nacl%': 0,

      # From src/third_party/widevine/cdm/widevine_cdm.gyp.
      # Whether to build Crosswalk with support for the Widevine CDM.
      'enable_widevine%': 0,

      # From src/third_party/ffmpeg/ffmpeg.gyp.
      # Whether to build the Chromium or Google Chrome version of FFmpeg (the
      # latter contains additional codecs).
      'ffmpeg_branding%': 'Chromium',

      # From src/third_party/ffmpeg/ffmpeg.gyp.
      # Which target type to build libffmpeg as.
      'ffmpeg_component%': '<(component)',

      # From src/build/common.gypi.
      # Whether to include stack unwinding support for backtrace().
      'release_unwind_tables%': 1,

      # From src/build/common.gypi.
      # Whether to use external startup data for V8.
      'v8_use_external_startup_data%': 1,

      # Whether to use the RealSense SDK (libpxc) video capture.
      'use_rssdk%': 0,

      'conditions': [
        ['mediacodecs_EULA==1', {
          'ffmpeg_branding%': 'Chrome',
        }],

        ['OS=="android"', {
          # Make release builds smaller by omitting stack unwind support for
          # backtrace().
          # TODO(rakuco): determine if we only want this in official builds.
          'release_unwind_tables%': 0,

          # Temporarily disable use of external snapshot files (XWALK-3516).
          'v8_use_external_startup_data%': 0,
        }],

        ['OS=="linux"', {
          # Since M44, ffmpeg is built as a static library by default. On Linux,
          # keep the previous behavior or building it as a shared library while
          # we figure out if it makes sense to switch to a static library by
          # default.
          'ffmpeg_component%': 'shared_library',
        }],

        ['OS=="mac" or OS=="win"', {
          'disable_nacl%': 1,
        }],

        ['OS=="win"', {
          'use_rssdk%': 1,
        }],
      ],
    },
    # Copy conditionally-set variables out one scope.
    'component%': '<(component)',
    'disable_nacl%': '<(disable_nacl)',
    'enable_widevine%': '<(enable_widevine)',
    'ffmpeg_branding%': '<(ffmpeg_branding)',
    'ffmpeg_component%': '<(ffmpeg_component)',
    'mediacodecs_EULA%': '<(mediacodecs_EULA)',
    'release_unwind_tables%': '<(release_unwind_tables)',
    'use_rssdk%': '<(use_rssdk)',
    'use_sysroot%': '<(use_sysroot)',
    'v8_use_external_startup_data%': '<(v8_use_external_startup_data)',

    # Whether to build and use Crosswalk's internal extensions (device
    # capabilities, sysapps etc).
    'disable_bundled_extensions%': 0,

    # From src/build/common.gypi.
    # Whether to include support for proprietary codecs..
    'proprietary_codecs%': 1,

    # Whether to use a WebUI-based file picker.
    'use_webui_file_picker%': 0,

    # Android: whether the integrity of the Crosswalk runtime library should be
    # verified when Crosswalk is run in shared mode.
    'verify_xwalk_apk%': 0,

    # Name of Crosswalk Maven artifacts used to generate their
    # respective POM files.
    'xwalk_core_library_artifact_id%': 'xwalk_core_library_canary',
    'xwalk_shared_library_artifact_id%': 'xwalk_shared_library_canary',

    # Reflects node's config.gypi.
    'component%': 'static_library',
    'python': 'python',
    'openssl_fips': '',
    'openssl_no_asm': 1,
    'node_release_urlbase': '',
    'node_byteorder': 'little',
    'node_target_type': 'shared_library',
    'node_install_npm': 'false',
    'node_prefix': '',
    'node_shared_cares': 'false',
    'node_shared_http_parser': 'false',
    'node_shared_libuv': 'false',
    'node_shared_openssl': 'false',
    'node_shared_v8': 'true',
    'node_shared_zlib': 'false',
    'node_tag': '',
    'node_use_dtrace': 'false',
    'node_use_etw': 'false',
    'node_use_mdb': 'false',
    'node_use_openssl': 'true',
    'node_use_perfctr': 'false',
    'uv_library': 'static_library',
    'uv_parent_path': 'third_party/node/deps/uv',
    'uv_use_dtrace': 'false',
    'V8_BASE': '',
    'v8_postmortem_support': 'false',
    'v8_enable_i18n_support': 'false',
    'libchromiumcontent_component': 0,
    'v8_libraries': '["v8", "v8_snapshot", "v8_nosnapshot", "v8_external_snapshot", "v8_base", "v8_libbase", "v8_libplatform"]',
    'icu_libraries': '["icui18n", "icuuc"]',
  },
  'target_defaults': {
    'conditions': [
      # TODO(rakuco): Remove this once we stop supporting Ubuntu Precise or
      # default to use_sysroot==1. This is only required because Precise's dbus
      # 1.4.18 causes
      # <https://bugs.chromium.org/p/chromium/issues/detail?id=263960#c6>.
      ['clang==1 and use_sysroot==0', {
        'cflags': [
          '-Wno-reserved-user-defined-literal',
        ],
      }],
    ],
    'target_conditions': [
      ['_target_name in <(v8_libraries) + <(icu_libraries)', {
        'xcode_settings': {
          'DEAD_CODE_STRIPPING': 'NO',  # -Wl,-dead_strip
          'GCC_INLINES_ARE_PRIVATE_EXTERN': 'NO',
          'GCC_SYMBOLS_PRIVATE_EXTERN': 'NO',
        },
        'cflags!': [
          '-fvisibility=hidden',
          '-fdata-sections',
          '-ffunction-sections',
        ],
        'cflags_cc!': ['-fvisibility-inlines-hidden'],
      }],
      ['_target_name in <(v8_libraries) + ["mksnapshot"]', {
        'defines': [
          'V8_SHARED',
          'BUILDING_V8_SHARED',
        ],
      }],
      ['_target_name in ["libuv", "http_parser", "openssl", "cares", "node", "zlib"]', {
        'msvs_disabled_warnings': [
          4003,  # not enough actual parameters for macro 'V'
          4013,  # 'free' undefined; assuming extern returning int
          4018,  # signed/unsigned mismatch
          4054,  #
          4055,  # 'type cast' : from data pointer 'void *' to function pointer
          4057,  # 'function' : 'volatile LONG *' differs in indirection to slightly different base types from 'unsigned long *'
          4065,  # switch statement contains 'default' but no 'case' labels
          4189,  #
          4131,  # uses old-style declarator
          4133,  # incompatible types
          4146,  # unary minus operator applied to unsigned type, result still unsigned
          4164,  # intrinsic function not declared
          4152,  # function/data pointer conversion in expression
          4206,  # translation unit is empty
          4204,  # non-constant aggregate initializer
          4210,  # nonstandard extension used : function given file scope
          4214,  # bit field types other than int
          4232,  # address of dllimport 'free' is not static, identity not guaranteed
          4291,  # no matching operator delete found
          4295,  # array is too small to include a terminating null character
          4311,  # 'type cast': pointer truncation from 'void *const ' to 'unsigned long'
          4389,  # '==' : signed/unsigned mismatch
          4456,  # declaration of 'm' hides previous local declaration
          4457,  # declaration of 'message' hides function parameter
          4459,  # declaration of 'wq' hides global declaration
          4477,  # format string '%.*s' requires an argument of type 'int'
          4505,  # unreferenced local function has been removed
          4701,  # potentially uninitialized local variable 'sizew' used
          4703,  # potentially uninitialized local pointer variable 'req' used
          4706,  # assignment within conditional expression
          4804,  # unsafe use of type 'bool' in operation
          4996,  # this function or variable may be unsafe.
        ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'WarnAsError': 'false',
          },
        },
        'xcode_settings': {
          'GCC_TREAT_WARNINGS_AS_ERRORS': 'NO',
          'WARNING_CFLAGS': [
            '-Wno-unknown-warning-option',
            '-Wno-parentheses-equality',
            '-Wno-unused-function',
            '-Wno-sometimes-uninitialized',
            '-Wno-pointer-sign',
            '-Wno-sign-compare',
            '-Wno-string-plus-int',
            '-Wno-unused-variable',
            '-Wno-deprecated-declarations',
            '-Wno-return-type',
            '-Wno-gnu-folding-constant',
            '-Wno-shift-negative-value',
          ],
        },
        'conditions': [
          ['OS=="linux"', {
            'cflags': [
              '-Wno-parentheses-equality',
              '-Wno-unused-function',
              '-Wno-sometimes-uninitialized',
              '-Wno-pointer-sign',
              '-Wno-string-plus-int',
              '-Wno-unused-variable',
              '-Wno-unused-value',
              '-Wno-deprecated-declarations',
              '-Wno-return-type',
              '-Wno-shift-negative-value',
              # Required when building as shared library.
              '-fPIC',
            ],
            'cflags!': [
              '-Wstring-conversion',
              '-Wignored-attributes',
            ]
          }],
        ],
      }],
      ['_target_name=="node"', {
        'include_dirs': [
          '<(DEPTH)/v8',
          '<(DEPTH)/v8/include',
        ],
        'conditions': [
          ['OS=="mac" and libchromiumcontent_component==0', {
            # -all_load is the "whole-archive" on OS X.
            'xcode_settings': {
              'OTHER_LDFLAGS': [ '-Wl,-all_load' ],
            },
          }],
          ['OS=="win"', {
            'libraries': [ '-lwinmm.lib' ],
            'conditions': [
              ['libchromiumcontent_component==0', {
                'variables': {
                  'conditions': [
                    ['target_arch=="ia32"', {
                      'reference_symbols': [
                        '_u_errorName_54',
                        '_ubidi_setPara_54',
                        '_ucsdet_getName_54',
                        '_uidna_openUTS46_54',
                        '_ulocdata_close_54',
                        '_unorm_normalize_54',
                        '_uregex_matches_54',
                        '_uscript_getCode_54',
                        '_usearch_setPattern_54',
                        '?createInstance@Transliterator@icu_54@@SAPAV12@ABVUnicodeString@2@W4UTransDirection@@AAW4UErrorCode@@@Z',
                      ],
                    }, {
                      'reference_symbols': [
                        'u_errorName_54',
                        'ubidi_setPara_54',
                        'ucsdet_getName_54',
                        'uidna_openUTS46_54',
                        'ulocdata_close_54',
                        'unorm_normalize_54',
                        'uregex_matches_54',
                        'uscript_getCode_54',
                        'usearch_setPattern_54',
                        '?createInstance@Transliterator@icu_54@@SAPEAV12@AEBVUnicodeString@2@W4UTransDirection@@AEAW4UErrorCode@@@Z',
                      ],
                    }],
                  ],
                },
                'msvs_settings': {
                  'VCLinkerTool': {
                    # There is nothing like "whole-archive" on Windows, so we
                    # have to manually force some objets files to be included
                    # by referencing them.
                    'ForceSymbolReferences': [ '<@(reference_symbols)' ],  # '/INCLUDE'
                  },
                },
              }],
            ],
          }],
          ['OS=="linux" and libchromiumcontent_component==0', {
            # Prevent the linker from stripping symbols.
            'ldflags': [
              '-Wl,--whole-archive',
              'lib/libv8.so',
              '-Wl,--no-whole-archive',
            ],
          }, {
            'libraries': [ 'v8' ],
          }],
        ],
      }],
      ['_target_name=="openssl"', {
        'xcode_settings': {
          'DEAD_CODE_STRIPPING': 'YES',  # -Wl,-dead_strip
          'GCC_INLINES_ARE_PRIVATE_EXTERN': 'YES',
          'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
        },
        'cflags': [
          '-fvisibility=hidden',
        ],
      }],
      ['_target_name=="libuv"', {
        'conditions': [
          ['OS=="win"', {
            # Expose libuv's symbols.
            'defines': [
              'BUILDING_UV_SHARED=1',
            ],
          }],  # OS=="win"
        ],
      }],
      ['_target_name.startswith("breakpad") or _target_name in ["crash_report_sender", "dump_syms"]', {
        'conditions': [
          ['OS=="mac"', {
            'xcode_settings': {
              'WARNING_CFLAGS': [
                '-Wno-deprecated-declarations',
                '-Wno-deprecated-register',
                '-Wno-unused-private-field',
                '-Wno-unused-function',
              ],
            },
          }],  # OS=="mac"
          ['OS=="linux"', {
            'cflags': [
              '-Wno-empty-body',
            ],
          }],  # OS=="linux"
          ['OS=="win"', {
            'msvs_disabled_warnings': [
              # unreferenced local function has been removed.
              4505,
            ],
          }],  # OS=="win"
        ],
      }],
    ],
    'msvs_cygwin_shell': 0, # Strangely setting it to 1 would make building under cygwin fail.
    'msvs_disabled_warnings': [
      4005,  # (node.h) macro redefinition
      4091,  # (node_extern.h) '__declspec(dllimport)' : ignored on left of 'node::Environment' when no variable is declared
      4189,  # local variable is initialized but not referenced
      4201,  # (uv.h) nameless struct/union
      4267,  # conversion from 'size_t' to 'int', possible loss of data
      4302,  # (atldlgs.h) 'type cast': truncation from 'LPCTSTR' to 'WORD'
      4458,  # (atldlgs.h) declaration of 'dwCommonButtons' hides class member
      4503,  # decorated name length exceeded, name was truncated
      4800,  # (v8.h) forcing value to bool 'true' or 'false'
      4819,  # The file contains a character that cannot be represented in the current code page
      4838,  # (atlgdi.h) conversion from 'int' to 'UINT' requires a narrowing conversion
      4996,  # (atlapp.h) 'GetVersionExW': was declared deprecated
    ],
  },
}
