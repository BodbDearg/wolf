# Small script that compiles and runs a wolf script
#
# Args:
# 	$1 - path to 'wolfc'
#	$2 - folder for intermidate files (.ll and .s)
#	$3 - folder for output executable
#	$4 - file to compile and run
WOLFC_PATH="${1}"
INTERMEDIATE_OUTPUT_DIR="${2}"
EXECUTABLE_OUTPUT_DIR="${3}"
mkdir -p "${INTERMEDIATE_OUTPUT_DIR}"
mkdir -p "${EXECUTABLE_OUTPUT_DIR}"
INPUT_PATH="${4}"

# Get the name of the input file without the extension
INPUT_FILENAME=`basename "${INPUT_PATH}" .wolf`

# Generate the llvm ir code using wolfc
OUTPUT_LL_FILE_PATH="${INTERMEDIATE_OUTPUT_DIR}/${INPUT_FILENAME}.ll"
"${WOLFC_PATH}" "${INPUT_PATH}" 2> "${OUTPUT_LL_FILE_PATH}"

if [ $? != 0 ]; then
    echo "A compiler error occurred for file '${INPUT_PATH}'! Error details follow: "
    cat "${OUTPUT_LL_FILE_PATH}"
    exit 1
fi

# Generate assembly
OUTPUT_S_FILE_PATH="${INTERMEDIATE_OUTPUT_DIR}/${INPUT_FILENAME}.s"
llc -march=x86-64 -x86-asm-syntax=intel -o "${OUTPUT_S_FILE_PATH}" "${OUTPUT_LL_FILE_PATH}"

if [ $? != 0 ]; then
    echo "Assembly code generation failed for IR code in file '${OUTPUT_LL_FILE_PATH}'!"
    exit 1
fi

# Compile that assembly to an executable
OUTPUT_BIN_FILE_PATH="${EXECUTABLE_OUTPUT_DIR}/${INPUT_FILENAME}"
clang "${OUTPUT_S_FILE_PATH}" -o "${OUTPUT_BIN_FILE_PATH}"

if [ $? != 0 ]; then
    echo "Compiling failed for assembly file '${OUTPUT_S_FILE_PATH}'!"
    exit 1
fi

chmod +x "${OUTPUT_BIN_FILE_PATH}"

if [ $? != 0 ]; then
    echo "Failed to change permissions for '${OUTPUT_BIN_FILE_PATH}'!"
    exit 1
fi

# Return the return code from that program
exit $?
