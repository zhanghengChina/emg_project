# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "armlet_imu: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iarmlet_imu:/home/leon/emg_project/src/armlet_imu/msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(armlet_imu_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/leon/emg_project/src/armlet_imu/msg/IMU.msg" NAME_WE)
add_custom_target(_armlet_imu_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "armlet_imu" "/home/leon/emg_project/src/armlet_imu/msg/IMU.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(armlet_imu
  "/home/leon/emg_project/src/armlet_imu/msg/IMU.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/armlet_imu
)

### Generating Services

### Generating Module File
_generate_module_cpp(armlet_imu
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/armlet_imu
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(armlet_imu_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(armlet_imu_generate_messages armlet_imu_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/leon/emg_project/src/armlet_imu/msg/IMU.msg" NAME_WE)
add_dependencies(armlet_imu_generate_messages_cpp _armlet_imu_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(armlet_imu_gencpp)
add_dependencies(armlet_imu_gencpp armlet_imu_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS armlet_imu_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(armlet_imu
  "/home/leon/emg_project/src/armlet_imu/msg/IMU.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/armlet_imu
)

### Generating Services

### Generating Module File
_generate_module_lisp(armlet_imu
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/armlet_imu
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(armlet_imu_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(armlet_imu_generate_messages armlet_imu_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/leon/emg_project/src/armlet_imu/msg/IMU.msg" NAME_WE)
add_dependencies(armlet_imu_generate_messages_lisp _armlet_imu_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(armlet_imu_genlisp)
add_dependencies(armlet_imu_genlisp armlet_imu_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS armlet_imu_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(armlet_imu
  "/home/leon/emg_project/src/armlet_imu/msg/IMU.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/armlet_imu
)

### Generating Services

### Generating Module File
_generate_module_py(armlet_imu
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/armlet_imu
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(armlet_imu_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(armlet_imu_generate_messages armlet_imu_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/leon/emg_project/src/armlet_imu/msg/IMU.msg" NAME_WE)
add_dependencies(armlet_imu_generate_messages_py _armlet_imu_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(armlet_imu_genpy)
add_dependencies(armlet_imu_genpy armlet_imu_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS armlet_imu_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/armlet_imu)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/armlet_imu
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/armlet_imu)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/armlet_imu
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/armlet_imu)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/armlet_imu\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/armlet_imu
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
