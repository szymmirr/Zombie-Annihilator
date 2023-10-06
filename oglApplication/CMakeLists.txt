set(PROJECT_NAME oglApplication)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "ReadMe.txt"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "Camera.hpp"
    "Content.h"
    "Effect.h"
    "Game.h"
    "Keyboard.h"
    "main.h"
    "Model.h"
    "Quad.h"
    "RenderTarget2D.h"
    "stdafx.h"
    "SubModel.h"
    "targetver.h"
    "Texture2D.h"
    "Zombie.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Content.cpp"
    "Effect.cpp"
    "Game.cpp"
    "Keyboard.cpp"
    "main.cpp"
    "Model.cpp"
    "Quad.cpp"
    "RenderTarget2D.cpp"
    "stdafx.cpp"
    "SubModel.cpp"
    "Texture2D.cpp"
    "Zombie.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(shaders
    "shaders/color.ps"
    "shaders/color.vs"
    "shaders/texture.ps"
    "shaders/texture.vs"
    "shaders/tmo.ps"
    "shaders/tmo.vs"
)
source_group("shaders" FILES ${shaders})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Source_Files}
    ${shaders}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE oglApplication)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "glgame4"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/../bin/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd/Assimp/include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "WIN32;"
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy
        >
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            -lwinmm;
            /FORCE:MULTIPLE
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /DEBUG;
        /SUBSYSTEM:CONSOLE;
        /INCREMENTAL:NO
    )
endif()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "glfw3dll;"
        "assimp;"
        "FreeImage;"
        "OpenGL32;"
        "glew32;"
        "glew32s"
    ">"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}//3rd/GLFW/lib;"
        "${CMAKE_CURRENT_SOURCE_DIR}//3rd/freeImage;"
        "${CMAKE_CURRENT_SOURCE_DIR}//3rd/Assimp/lib/x86;"
        "${CMAKE_CURRENT_SOURCE_DIR}//3rd/glew/lib/Release/Win32"
    ">"
)
