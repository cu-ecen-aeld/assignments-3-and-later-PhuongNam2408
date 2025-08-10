# Create a Bash script which will print a message based upon which day of the week it is (eg. 'Happy hump day' for Wedensday, 'TGIF' for Friday etc).

if [ $# -ne 1 ] ; then
  echo "ERROR: Invalid argument. The input argument should be only Monday/Tuesday/Wednesday/Thursday/Friday/Sartuday/Sunday"
  exit 1
fi

# Take the input argument. It should be a string as a name of the day.
week_day=$1

# Normalize the character to the same lower/upper case
week_day=$(echo $week_day | tr '[A-Z]' '[a-z]')

echo $week_day

case $week_day in 
  monday)
    echo "Back to the grind!"
    ;;
  tuesday)
    echo "Keep it going!"
    ;;
  wednesday)
    echo "Happy hump day!"
    ;;
  thursday)
    echo 	"Almost there!"
    ;;
  friday)
    echo "TGIF!"
    ;;
  sartuday)
    echo 	"Enjoy your weekend!"
    ;;
  sunday)
    echo "Rest and recharge!"
    ;;
  *)
    echo "ERROR: Invalid argument. You typed $week_day. How ever, the input argument should be only Monday/Tuesday/Wednesday/Thursday/Friday/Sartuday/Sunday"
esac
  
