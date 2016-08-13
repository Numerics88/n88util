macro (get_glibc_version _glibc_version)
  try_run (RUN_RESULT COMPILE_RESULT
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/cmake/configure_programs/glibc_version.c
      RUN_OUTPUT_VARIABLE RUN_OUTPUT)
  if (COMPILE_RESULT)
    set (${_glibc_version} ${RUN_OUTPUT})
  else()
    set (${_glibc_version})
  endif()
endmacro ()
