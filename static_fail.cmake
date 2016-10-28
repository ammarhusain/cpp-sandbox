FUNCTION (add_fail_test file ARG_TEST_CASES ARG_BASE_TARGET)
  # Get the target properties from the base target.
  get_target_property(target_includes ${ARG_BASE_TARGET} INCLUDE_DIRECTORIES)
  get_filename_component(filename ${file} NAME_WE)
  set(all_fail_targets)
  # Loop through all test cases.
  foreach(idx RANGE 1 ${ARG_TEST_CASES})
    set(fail_target ${ARG_BASE_TARGET}_${filename}_${idx})
    add_library(${fail_target} ${file})
    target_compile_definitions(${fail_target} PRIVATE STATIC_TEST_${idx})
    add_test(NAME ${fail_target}
      COMMAND ${CMAKE_COMMAND} --build . --target ${fail_target} --config $<CONFIGURATION>
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    list(APPEND all_fail_targets ${fail_target})
  endforeach()

  set_target_properties(${all_fail_targets} PROPERTIES
    EXCLUDE_FROM_ALL TRUE
    EXCLUDE_FROM_DEFAULT_BUILD TRUE)
  set_tests_properties(${all_fail_targets} PROPERTIES WILL_FAIL TRUE)
ENDFUNCTION()