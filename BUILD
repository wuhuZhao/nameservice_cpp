COPTS = [
    "-D__STDC_FORMAT_MACROS",
    "-DBTHREAD_USE_FAST_PTHREAD_MUTEX",
    "-D__const__=",
    "-D_GNU_SOURCE",
    "-DUSE_SYMBOLIZE",
    "-DNO_TCMALLOC",
    "-D__STDC_LIMIT_MACROS",
    "-D__STDC_CONSTANT_MACROS",
    "-fPIC",
    "-Wno-unused-parameter",
    "-fno-omit-frame-pointer",
    "-DGFLAGS_NS=google",
]

proto_library(
    name = "nameservice_proto",
    srcs = [
        "proto/nameservice.proto"
    ]
)

cc_proto_library(
    name = "nameservice",
    deps = [
        ":nameservice_proto"
    ]
)

cc_library(
    name = "service",
    srcs = ["service/nameService.cpp"],
    hdrs = ["service/nameService.h"],
    deps = [
        "@brpc//:brpc",
        ":nameservice",
    ]
)

cc_binary(
    name = "name_c++_server",
    srcs = [
        "main.cpp",
    ],
    deps = [
        ":service",
    ],
    copts = COPTS,
)