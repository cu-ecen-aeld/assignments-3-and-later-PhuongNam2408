# Accepts the following arguments: the first argument is a full path to a file (including filename) on the filesystem, referred to below as writefile; the second argument is a text string which will be written within this file, referred to below as writestr
# Exits with value 1 error and print statements if any of the arguments above were not specified
# Creates a new file with name and path writefile with content writestr, overwriting any existing file and creating the path if it doesn’t exist. Exits with value 1 and error print statement if the file could not be created.

# Verify the number of argument
if [ $# -ne 2 ] ; then
  echo "ERROR: Need exactly 2 arguments, but you provided $#"
  exit 1
fi

WRITEFILE=$1
WRITESTR=$2

WRITEPATH=$( dirname $WRITEFILE )

# Create a directory if WRITEPATH doesn't exist
if [ ! -d $WRITEPATH ] ; then
  echo "$WRITEPATH doesn't exist. Creating it..."
  mkdir -p "$WRITEPATH"
fi

# Create WRITEFILE
touch "$WRITEFILE"

# Write WRITESTR into WRITEFILE
printf "$WRITESTR" >> "$WRITEFILE"
