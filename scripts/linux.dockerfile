FROM registry.gitlab.steamos.cloud/steamrt/scout/sdk:latest
ENV CC /usr/bin/gcc-9
ENV CXX /usr/bin/g++-9

COPY scripts/setup-linux.sh /scripts/
RUN bash /scripts/setup-linux.sh

VOLUME /game
WORKDIR /game/build