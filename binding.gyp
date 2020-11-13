{
  "targets": [
    {
      "target_name": "krypto",
       "include_dirs": ["<!(node -p \"require('node-addon-api').include_dir\")"],
      "sources": [ "./src/main.cc" ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }

  ]
}