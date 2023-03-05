# Container image that runs your code
FROM gcc

ADD https://github.com/gbdk-2020/gbdk-2020/releases/latest/download/gbdk-linux64.tar.gz /usr/lib/gbd.tar.gz
RUN tar -xvf /usr/lib/gbd.tar.gz -C /usr/lib/

WORKDIR /github/workspace
RUN ls

# Copies your code file from your action repository to the filesystem path `/` of the container
COPY entrypoint.sh /entrypoint.sh
RUN ["chmod", "+x", "/entrypoint.sh"]

# Code file to execute when the docker container starts up (`entrypoint.sh`)
ENTRYPOINT ["/entrypoint.sh"]
