#!/bin/bash
cd $(dirname $0)
cd ..
HTML_DIR=doc/html
RST2HTML_OPTS="--stylesheet voidspace.css --link-stylesheet"
mkdir -p ${HTML_DIR}
rst2html ${RST2HTML_OPTS} NEWS > ${HTML_DIR}/news.html
rst2html ${RST2HTML_OPTS} README > ${HTML_DIR}/readme.html
rst2html ${RST2HTML_OPTS} doc/index.rst > ${HTML_DIR}/index.html
src/tempi-inspect/tempi-inspect -a | rst2html ${RST2HTML_OPTS} > ${HTML_DIR}/nodes.html

make html
mkdir -p ${HTML_DIR}/doxygen
cp html/* ${HTML_DIR}/doxygen/


