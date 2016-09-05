include_directories (json)
add_library (jsoncpp SHARED jsoncpp.cpp)
install(TARGETS jsoncpp DESTINATION ~/lib)
