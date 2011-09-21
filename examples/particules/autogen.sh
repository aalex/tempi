#!/bin/sh
rm -rf autom4te.cache

if [ `uname -s` = 'Darwin' ]; then
  LIBTOOLIZE="glibtoolize"
else
  LIBTOOLIZE="libtoolize"
fi

$LIBTOOLIZE --force --copy &&  aclocal &&  autoheader &&  automake --add-missing --force-missing --foreign --copy &&  autoconf
