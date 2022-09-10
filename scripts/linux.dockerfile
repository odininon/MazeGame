FROM registry.gitlab.steamos.cloud/steamrt/scout/sdk:latest

COPY scripts/setup-linux.sh /scripts/
RUN bash /scripts/setup-linux.sh

WORKDIR /build
VOLUME /drift