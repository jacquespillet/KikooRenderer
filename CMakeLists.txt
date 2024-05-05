cmake_minimum_required(VERSION 2.6)
project(KikooRenderer)
set(PROJECT_BRIEF "KikooRenderer")

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)


set(CMAKE_BUILD_TYPE RELEASE)
set(GCC_COMPILE_FLAGS "-std=c++11 -Wall -O3 -g -static /bigobj")
set(GCC_LINK_FLAGS "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(USE_MSVC_RUNTIME_LIBRARY_DLL  ON)

#qt
find_package(Qt5 COMPONENTS Core Widgets Gui OpenGL 3DCore 3DExtras 3DRender 3DInput 3DAnimation REQUIRED)

#glm
set(GLM_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
include_directories(${GLM_INC_DIR})

#stbi
set(STBI_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb)
include_directories(${STBI_INC_DIR})

# ASSIMP
set(ASSIMP_INSTALL_PDB OFF)
add_subdirectory(vendor/assimp)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/vendor/assimp/include")
include_directories(vendor/assimp/include)

#Bullets
SET(INSTALL_LIBS ON)
SET(BUILD_SHARED_LIBS OFF)

add_subdirectory(vendor/bullet)
include_directories("${PROJECT_SOURCE_DIR}/vendor/bullet/src")

#CTPL
set(CTPL_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ctpl)
include_directories(${CTPL_INC_DIR})

include_directories(src)
add_subdirectory(src)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/3DEngine/Shaders/Sources/
		DESTINATION bin/Shaders
		COMPONENT MainWindowDist
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/
		DESTINATION bin/resources
		COMPONENT MainWindowDist
)

set(QT_DIR "$ENV{QT_PATH}")

Message(${QT_DIR})
install(DIRECTORY ${QT_DIR}/plugins/platforms
		DESTINATION bin
		COMPONENT MainWindowDist
)

install(FILES 
			  ${QT_DIR}/bin/Qt5Core.dll
			  ${QT_DIR}/bin/Qt5Cored.dll
			  ${QT_DIR}/bin/Qt5Widgets.dll
			  ${QT_DIR}/bin/Qt5Widgetsd.dll
			  ${QT_DIR}/bin/Qt5Gui.dll
			  ${QT_DIR}/bin/Qt5Guid.dll
			  
		DESTINATION bin
		COMPONENT MainWindowDist
)
