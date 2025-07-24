#/bin/bash

cd $1

DOCS=$(find Docs/Dev -regex '.*[\.drawio]$')

for diag in $DOCS
do
    dest=$(echo $diag | sed 's/\/Dev//')
    python Docs/Dev/query_drawio_server.py $diag $dest
done <<< $DOCS
