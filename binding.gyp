{
  "targets": [
    {
      "target_name": "krypto",
       "include_dirs": ["<!(node -p \"require('node-addon-api').include_dir\")"],
      "sources": [ 
        "./src/main.cc",  
        "./src/check.cc", 
        "./src/hashMethod/MD/MD2.cc",  
        "./src/hashMethod/MD/MD4.cc",  
        "./src/hashMethod/MD/MD5.cc",  
        "./src/hashMethod/MD/MD5X.cc",  
        "./src/hashMethod/SHA/SHA-1.cc",  
        "./src/hashMethod/SHA/SHA-224.cc",  
        "./src/hashMethod/SHA/SHA-256.cc",  
        "./src/hashMethod/SHA/SHA-384.cc",  
        "./src/hashMethod/SHA/SHA-512.cc",  
        "./src/hashMethod/RIPEMD/RIPEMD-128.cc",  
        "./src/hashMethod/RIPEMD/RIPEMD-160.cc",  
        "./src/hashMethod/RIPEMD/RIPEMD-256.cc",  
        "./src/hashMethod/RIPEMD/RIPEMD-320.cc",  
        "./src/hashMethod/HAVAL/HAVAL-3.cc",  
        "./src/hashMethod/HAVAL/HAVAL-4.cc",  
        "./src/hashMethod/HAVAL/HAVAL-5.cc",  
        "./src/hashMethod/TIGER/TIGER-3.cc",  
        "./src/hashMethod/TIGER/TIGER-4.cc",  
        "./src/hashMethod/FNV1/FNV1-32.cc",  
        "./src/hashMethod/FNV1/FNV1-64.cc",  
        "./src/hashMethod/FNV1A/FNV1A-32.cc",  
        "./src/hashMethod/FNV1A/FNV1A-64.cc",  
        "./src/hashMethod/FNV1A/FNV1A-128.cc", 
        "./src/hashMethod/FNV1A/FNV1A-256.cc", 
        "./src/hashMethod/FNV1A/FNV1A-512.cc", 
        "./src/hashMethod/FNV1A/FNV1A-1024.cc",
        "./src/hashMethod/FNV1A/FNV1A-method.cc",
        "./src/hashMethod/ADLER/ADLER-32.cc",  
        "./src/hashMethod/CRC/CRC-32.cc"],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }

  ]
}