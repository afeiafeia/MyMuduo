file(REMOVE_RECURSE
  "LogTest.pdb"
  "LogTest"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/LogTest.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
