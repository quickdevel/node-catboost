{
  'targets': [
    { 
      'target_name': 'catboost',
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'sources': [ 'catboost.cc', 'model.cc' ],
      'include_dirs' : [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'conditions': [
        ['OS=="win"', {
          'libraries': [
            '../libs/catboostmodel.lib'
          ],
          'copies': [{
            'destination': '<(PRODUCT_DIR)',
            'files': [
              'libs/catboostmodel.dll'
            ]
          }]
        }],
        ['OS=="linux"', {
          'libraries': [
            '../libs/libcatboostmodel.so',
            '-Wl,--disable-new-dtags -Wl,-rpath=\'$${ORIGIN}/../../libs\''
          ]
        }],
        ['OS=="mac"', {
          'libraries': [
            '../libs/libcatboostmodel.dylib',
            '-rpath \'@loader_path/../../libs\''
          ],
          "postbuilds": [{
            "postbuild_name": "Adjust link path to CatBoostModel lib",
            "action": [
              "install_name_tool",
              "-change",
              "libcatboostmodel.dylib.1",
              "@loader_path/../../libs/libcatboostmodel.dylib.1",
              "<@(PRODUCT_DIR)/catboost.node"
            ]
          }],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}
