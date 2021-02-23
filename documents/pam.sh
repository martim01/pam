FILE=~/pam/setup
if [ -f "$FILE" ]; then
  while true;
  do
    pam2
  done
else
  InitialSetup
  touch "$FILE"
  sudo shutdown -r now
fi
