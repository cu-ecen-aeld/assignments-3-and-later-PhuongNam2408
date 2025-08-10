# Create a Bash script which will accept a file as a command line argument and analyse it in certain ways. eg. you could check if the file is executable or writable. You should print a certain message if true and another if false.

# Take a file from arguments
file=$1

# Verify whether it is a file
if [ "$#" -ne 1 ] ; then
  echo "ERROR: Invalid argument"
  exit 1
elif [ ! -e "$file" ] ; then
  echo "ERROR: $file is not a file. Please correct the argument"
  exit 1
fi

# Retrieve the file permission
permission=$( ls -l "$file" )

echo "File permission is $permission "

# 2nd pos is read
if [ ${permission:1:1} = "r" ] ; then
  echo "The file is readable"
else
  echo "The file is NOT readable"
fi

# 3rd write
if [ ${permission:2:1} = "w" ] ; then
  echo "The file is writable"
else
  echo "The file is NOT writable"
fi

# 4th position is execute
if [ ${permission:3:1} = "x" ] ; then
  echo "The file is executable"
else
  echo "The file is NOT executable"
fi
