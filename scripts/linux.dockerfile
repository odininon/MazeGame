FROM registry.gitlab.steamos.cloud/steamrt/sniper/sdk:latest

COPY scripts/setup-linux.sh /scripts/
RUN bash /scripts/setup-linux.sh

VOLUME /game
WORKDIR /game/build