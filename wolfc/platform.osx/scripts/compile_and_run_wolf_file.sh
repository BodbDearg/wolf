# Small script that compiles and runs a wolf script
#
# Args:
# 	$1 - path to 'wolfc'
#	$2 - folder for output executable and object files
#	$3 - file to compile and run
WOLFC_PATH="${1}"
OUTPUT_DIR="${2}"
mkdir -p "${OUTPUT_DIR}"
INPUT_PATH="${3}"

# Get the name of the input file without the extension
INPUT_FILENAME=`basename "${INPUT_PATH}" .wolf`

# Generate the llvm ir code using wolfc
OUTPUT_LL_FILE_PATH="${OUTPUT_DIR}/${INPUT_FILENAME}.ll"
"${WOLFC_PATH}" "${INPUT_PATH}" 2> "${OUTPUT_LL_FILE_PATH}"

if [ $? != 0 ]; then
    echo "A compiler error occurred for file '${INPUT_PATH}'! Error details follow: "
    cat "${OUTPUT_LL_FILE_PATH}"
    exit 1
fi

# Generate assembly
OUTPUT_S_FILE_PATH="${OUTPUT_DIR}/${INPUT_FILENAME}.s"
llc -march=x86-64 -x86-asm-syntax=intel -o "${OUTPUT_S_FILE_PATH}" "${OUTPUT_LL_FILE_PATH}"

if [ $? != 0 ]; then
    echo "Assembly code generation failed for IR code in file '${OUTPUT_LL_FILE_PATH}'!"
    exit 1
fi

# Compile that assembly to an executable
OUTPUT_BIN_FILE_PATH="${OUTPUT_DIR}/${INPUT_FILENAME}"
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

# Run the executable
"${OUTPUT_BIN_FILE_PATH}"

# Return the return code from that program
exit $?
