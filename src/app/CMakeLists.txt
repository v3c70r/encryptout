include_directories(${encryptout_SOURCE_DIR}/src/lib)
link_directories(${encryptout_SOURCE_DIR}/src/lib)

set(CMAKE_MODULE_PATH "${encryptout_SOURCE_DIR}/cmake")
find_package(gpgme)

add_executable(encryptout main.cpp App.cpp)
target_link_libraries(encryptout pwdMgr gpgme )
