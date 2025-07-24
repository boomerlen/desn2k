FROM python:3.13-alpine

LABEL org.opencontainers.image.source=https://github.com/boomerlen/desn2k
LABEL org.opencontainers.image.description="Simple MKDocs launcher container"
LABEL org.opencontainers.image.licences=MIT

RUN apt update && \
    apt install -y git

RUN python -m pip install mkdocs

RUN python -m pip install mkdocs-drawio

cmd ["/bin/bash"]
