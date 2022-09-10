apt-get update -y
apt-get install -y wget
apt-get clean

wget https://github.com/conan-io/conan/releases/latest/download/conan-ubuntu-64.deb

dpkg -i conan-ubuntu-64.deb