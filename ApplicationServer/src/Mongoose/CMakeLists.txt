# create a library called "Mongoose" which includes the source file "mongoose.cpp"
add_library (Mongoose STATIC mongoose.c)


# Set the location for library installation -- i.e, /usr/lib in this case
# Use "sudo make install" to apply
install (TARGETS Mongoose DESTINATION "${CMAKE_BINARY_DIR}/lib")
