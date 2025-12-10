# Force MSVC debug-friendly flags for memcheck (needed by Dr. Memory)
set(CMAKE_CXX_FLAGS "/Zi /MT /EHsc /Oy- /Ob0" CACHE STRING "MSVC flags for memcheck")
set(CMAKE_C_FLAGS "/Zi /MT /EHsc /Oy- /Ob0" CACHE STRING "MSVC flags for memcheck")
