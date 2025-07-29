cmake_minimum_required(VERSION 3.12)

find_package(Doxygen REQUIRED)

# Set color theme
set(DOXYGEN_HTML_COLORSTYLE_HUE 220)
set(DOXYGEN_HTML_COLORSTYLE_SAT 100)
set(DOXYGEN_HTML_COLORSTYLE_GAMMA 80)

doxygen_add_docs(
    doxygen
    ${PROJECT_SOURCE_DIR}/include
)
