FROM python:3.13-slim

LABEL org.opencontainers.image.source=https://github.com/boomerlen/desn2k
LABEL org.opencontainers.image.description="Simple MKDocs launcher container"
LABEL org.opencontainers.image.licences=MIT

RUN python -m pip install mkdocs

RUN python -m pip install mkdocs-drawio

cmd ["/bin/bash"]
