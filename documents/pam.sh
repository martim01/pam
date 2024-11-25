ROTATE=~/pam/rotate
if [ -f "$ROTATE" ]; then
    xrandr -o inverted
fi

FILE=~/pam/setup
if [ -f "$FILE" ]; then
  while true;
  do
    pam2
  done
else
  InitialSetup
  touch "$FILE"

  #expand the filesystem
  PARTSIZE=$( df | sed -n '/root/{s/  */ /gp}' | cut -d ' ' -f2)
  THRESHOLD=6291456
  if [ "$PARTSIZE" -lt "$THRESHOLD" ]
  then
    raspi-config --expand-rootfs
  fi
  sudo shutdown -r now
fi

