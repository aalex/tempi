#!/bin/bash
cd $(dirname $0)
cd ..
PDF_DIR=doc/pdf
RST2PDF_OPTS="--verbose"
mkdir -p ${PDF_DIR}
rst2pdf ${RST2PDF_OPTS} NEWS -o ${PDF_DIR}/news.pdf
rst2pdf ${RST2PDF_OPTS} README -o ${PDF_DIR}/readme.pdf
src/tempi-inspect/tempi-inspect -a | rst2pdf ${RST2PDF_OPTS} -o ${PDF_DIR}/nodes.pdf

