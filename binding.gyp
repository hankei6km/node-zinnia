{
  'target_defaults': {
    'cflags': [
      '<!@(pkg-config --cflags zinnia)'
    ],
    'libraries': [
      '<!@(pkg-config --libs zinnia)',
    ],
    'include_dirs': [
      'src/'
    ],
    'sources': [
      'src/node-zinnia.cc'
    ]
  },
  'targets': [
    {
      'target_name': 'zinnia',
    }
  ]
}
