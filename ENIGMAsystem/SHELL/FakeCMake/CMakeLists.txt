# Some hacks to ensure our codegen and workdir exist before doing our codegen
# otherwise the compiler will explode

cmake_minimum_required(VERSION 3.10)

# Make codegen dir
file(MAKE_DIRECTORY "${CODEGEN}/Preprocessor_Environment_Editable/")

# Make working dir
file(MAKE_DIRECTORY "${WORKDIR}")
