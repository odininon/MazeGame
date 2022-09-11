apt-get update -y
apt-get install -y tar curl zip unzip git
apt-get upgrade curl
apt-get clean

git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh