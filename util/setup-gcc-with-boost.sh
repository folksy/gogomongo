#!/bin/bash

sudo apt-get update --yes
sudo apt-get dist-upgrade --yes
sudo apt-get install emacs24 --yes
sudo apt-get install build-essential --yes
sudo apt-get install zlib1g-dev --yes
sudo apt-get install libbz2-dev --yes
sudo apt-get install ruby1.9.3 --yes
sudo apt-get install python-dev --yes
sudo apt-get install git --yes
sudo apt-get install libxslt1-dev --yes
sudo apt-get install libxml2-dev --yes
sudo apt-get install mysql-client --yes
sudo apt-get install libmysqlclient-dev --yes
sudo apt-get install libmysql++-dev --yes

git config --global user.name douglivesey
git config --global user.email biot023@gmail.com
git config --global color.ui true

mkdir -p ~/work/src && cd ~/work/src
wget http://downloads.sourceforge.net/project/boost/boost/1.53.0/boost_1_53_0.tar.gz
tar zxvf boost_1_53_0.tar.gz
cd boost_1_53_0
./bootstrap.sh
sudo ./b2 install

sudo reboot
