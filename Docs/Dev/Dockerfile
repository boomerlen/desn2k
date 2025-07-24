FROM python:slim

LABEL org.opencontainers.image.source=https://github.com/boomerlen/desn2k
LABEL org.opencontainers.image.description="Simple Python requests image"
LABEL org.opencontainers.image.licences=MIT

RUN python -m pip install requests

WORKDIR /home

COPY query_drawio_server.py /home/query_drawio_server.py

CMD ["/bin/bash"]
