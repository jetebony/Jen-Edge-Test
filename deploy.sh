#!/usr/bin/env bash

VERSION=$(cat VERSION)
NAME="integra-lib-utils$1"
HEADERS="*.h"

echo "==> Building IPK"
sed -i -e "s/#VERSION#/$VERSION/" package/CONTROL/control
sed -i -e "s/#NAME#/$NAME/" package/CONTROL/control

sed -i -e "s/#VERSION#/$VERSION/" package.arm/CONTROL/control
sed -i -e "s/#NAME#/$NAME/" package.arm/CONTROL/control

if [ "$1" == "-dev" ]; then
  echo "==> Adding header files to IPK"
  mkdir -p package/usr/include/integra-utilslib
  cp $HEADERS package/usr/include/integra-utilslib
  mkdir -p package.arm/integra/include/integra-utilslib
  cp $HEADERS package.arm/integra/include/integra-utilslib
fi
chown -R root:root package/
tree package/
opkg-build package/

chown -R root:root package.arm/
tree package.arm/
opkg-build package.arm/

echo "==> Uploading to OPKG Repository"
scp -o "StrictHostKeyChecking no" *.ipk Granite@integra-edge.net:/etc/integra-packages/
ssh -o "StrictHostKeyChecking no" -t Granite@integra-edge.net "sudo opkg-rebuild"
echo ":ship: $NAME@$VERSION has been uploaded to the Repository" | slacker -n OPKG -c opkg
