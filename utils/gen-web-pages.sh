#!/bin/bash
cd $(dirname $0)
cd ..
HTML_DIR=doc/html
mkdir -p ${HTML_DIR}
rst2html NEWS > ${HTML_DIR}/news.html
rst2html README > ${HTML_DIR}/readme.html
rst2html doc/index.rst > ${HTML_DIR}/index.html
src/tempi-inspect/tempi-inspect -a | rst2html > ${HTML_DIR}/nodes.html
make html
mkdir -p ${HTML_DIR}/doxygen
cp html/* ${HTML_DIR}/doxygen/

