docker build -t build-linux -f scripts/linux.dockerfile . && docker run --rm -v %cd%:/drift build-linux bash /drift/scripts/build-linux.sh