{
    "targets": [
        {
            "target_name": "rosevm",
            "sources": [ "addon.cc", "./src/vm/vm.cc", "./src/vm/api_graphics.cc", "./src/vm/api_input.cc", "./src/vm/api_memory.cc", "./src/vm/common.cc", "./src/js/js.cc", "./src/js/js_memory.cc", "./src/js/js_graphics.cc", "./src/js/js_input.cc", "./src/js/js_common.cc"],
            "include_dirs" : [
 	 			"<!(node -e \"require('nan')\")"
			],
            "xcode_settings": {
                "GCC_VERSION": "com.apple.compilers.llvm.clang.1_0",
                "CLANG_CXX_LANGUAGE_STANDARD": "c++14",
                "CLANG_CXX_LIBRARY": "libc++",
            },
            "cflags": [
                "-std=c++14",
                "-stdlib=libc++"
            ]
        }
    ],
}