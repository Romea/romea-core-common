function(romea_add_gtest test_name source_file)
  set(options)
  set(one_value_args CXX_STANDARD)
  set(multi_value_args ARGS LIBRARIES)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT ARG_CXX_STANDARD)
    set(ARG_CXX_STANDARD 17)
  endif()

  set(target_name ${PROJECT_NAME}_${test_name})

  add_executable(${target_name} ${source_file})
  target_link_libraries(${target_name}
    GTest::GTest
    GTest::Main
    ${ARG_LIBRARIES})
  target_compile_features(${target_name} PRIVATE cxx_std_${ARG_CXX_STANDARD})

  add_test(NAME ${test_name} COMMAND ${target_name} ${ARG_ARGS})
endfunction()
