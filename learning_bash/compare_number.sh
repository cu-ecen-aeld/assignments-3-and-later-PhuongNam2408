# take 2 numbers as command line arguments.
num_1=$1
num_2=$2

# Verify the arguments
if [ "$#" -ne 2 ] ; then
  echo "Please input 2 numbers"
  exit 1
fi

if [ $num_1 -gt $num_2 ] ; then
  echo "The larger number is $num_1"

elif [ $num_2 -gt $num_1 ] ; then 
  echo "The larger number is $num_2"

else
  echo "2 number has the same values"
fi
