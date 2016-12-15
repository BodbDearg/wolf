# Small script that compiles and runs a wolf script.
# Returns the exit code of the program run, or '1' if the build fails.
#
# Args:
# 	$1 - path to 'wolfc'
#	$2 - folder for intermidate files (.ll and .s)
#	$3 - folder for output executable
#	$4 - file to compile and run

# Get the folder that this file is in
THIS_FILE_DIR="$( cd "$( dirname "$0" )" && pwd )"

# Try to compile the file first, if compile fails then bail out
"${THIS_FILE_DIR}/compile_wolf_file.sh" "${1}" "${2}" "${3}" "${4}" 

if [ $? != 0 ]; then
    exit 1
fi

# Get the output dir and input file path
EXECUTABLE_OUTPUT_DIR="${3}"
INPUT_FILE_PATH="${4}"

# Get the name of the input file without the extension
INPUT_FILENAME=`basename "${INPUT_FILE_PATH}" .wolf`

# This is the path to the output executable
OUTPUT_BIN_FILE_PATH="${EXECUTABLE_OUTPUT_DIR}/${INPUT_FILENAME}"

# Run the executable
"${OUTPUT_BIN_FILE_PATH}"

# Return the return code from that program
exit $?
