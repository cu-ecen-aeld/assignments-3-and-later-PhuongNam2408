#!/bin/sh

# Get the runtime args:
# 1st args: path to a directory <filesdirs>
# 2nd args: text strings searched within the files <searchstr>
# Return error (1), print statement when the params were not specified
# Return error (1), print statement when <filesdir> does not exist
# Success => "The number of files are X and the number of matching lines are Y"

# Declare an file list array
declare -a FILE_LIST_ARR
declare -i MATCHING_LINES=0

# Verify the number of input args
if [ $# -ne 2 ] ; then
  echo "ERROR: Invalid number of input args. Expected 2 args, but you provided $# args"
  exit 1
fi

# 1st arg
FILESDIR=$1
# Verify the 1st arg whether it is a dir
if [ -d $FILESDIR ] ; then
  echo "$1 is a directory, going through all files..."
else 
  echo "ERROR: $1 is not a directory"
  exit 1
fi

# 2nd arg
SEARCHSTR=$2

# Get all files placed in FILESDIR
FILE_LIST_ARR=$( ls $FILESDIR )
echo "All files and sub dirs in $FILESDIR: ${FILE_LIST_ARR}"

# Loop through all files, grep the SEARCHSTR, count the nuber of lines that contains SEARCHSTR
for FILE in ${FILE_LIST_ARR}
  do 
    if [ -d $FILE ] ; then
      echo "$FILE is a directory, ignoring..."
    else
      GREP_OUT=$( grep $SEARCHSTR $FILE)
      # Count the line of the output grep
      MATCHING_LINE=$(( $MATCHING_LINE + $( echo "$GREP_OUT" | wc -l )))
    fi
done

FILE_LITS_NUMBER=$( echo "$FILE_LIST_ARR" | wc -w )

echo "The number of files are ${FILE_LITS_NUMBER} and the number of matching lines are $MATCHING_LINE"