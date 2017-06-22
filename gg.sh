#!/bin/sh
app_name = gg

if [ -f $app_name ]; then
    rm -r $app_name
fi

g++ -o $app_name -std=c++11 \
  main.cpp \
  -lcurl

if [ ! -f $app_name ]; then
    exit;
fi

 # DEBUG
./gg
exit

while true; do
    read -p "Do you wish to add this program to /bin? (y/n)" yn
    case $yn in
        [Yy]* ) make install; ./$app_name; break;;
        [Nn]* ) ./$app_name;;
        * ) echo "Please answer yes or no.";;
    esac
done
