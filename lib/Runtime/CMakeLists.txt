file(GLOB_RECURSE OPERATOR_C_FILES operator/*.c)

add_libonnc_src(
    ${OPERATOR_C_FILES}
    onnc-runtime.c)
