PROGRAM_NAME='hash_table'
TEXT_FILE='text.txt'

browser_name="google-chrome"
run_gdbgui() {
  local dir="$(whereis -b $browser_name)"
  local browser_dir="${dir#"$browser_name: "}"

  gdbgui -g "gdb --q" --browser "$browser_dir" --args ./$PROGRAM_NAME $TEXT_FILE
}

echo "$0 test"

cd build

if [ ! -f $TEXT_FILE ]; then
  echo "$0 $TEXT_FILE not found"
fi

if [ -e $PROGRAM_NAME ]; then
  run_gdbgui
else
  echo "$PROGRAM_NAME not found. please run configure.sh then build.sh"
fi
