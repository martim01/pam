FILE=~/pam/setup
if [ -f "$FILE" ]; then
  while true;
  do
    pam2
  done
else
  InitialSetup
  touch "$FILE"
  sudo raspi-config --expand-rootfs
  sudo shutdown -r now
fi
