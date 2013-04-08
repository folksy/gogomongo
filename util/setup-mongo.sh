#!/bin/bash

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv 7F0CEB10
sudo sh -c 'echo "deb http://downloads-distro.mongodb.org/repo/ubuntu-upstart dist 10gen" > /etc/apt/sources.list.d/10gen.list'
sudo apt-get update --yes
sudo apt-get install mongodb-10gen --yes
sudo apt-get install scons --yes

mkdir -p ~/work/src
cd ~/work/src
wget http://downloads.mongodb.org/cxx-driver/mongodb-linux-x86_64-latest.tgz
tar zxvf mongodb-linux-x86_64-latest.tgz
cd mongo-cxx-driver-nightly
scons

sudo sh -c 'echo "export LD_LIBRARY_PATH=\"/usr/local/lib\"" >> /etc/environment'
sudo reboot
