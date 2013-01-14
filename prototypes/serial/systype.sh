#!/bin/sh
case `uname -s` in
"FreeBSD")
	PLATFORM="FREEBSD"
	;;
"Linux")
	PLATFORM="LINUX"
	;;
"Darwin")
	PLATFORM="MACOS"
	;;
"SunOS")
	PLATFORM="SOLARIS"
	;;
*)
	echo "Unknown platform" >&2
	exit 1
esac
echo $PLATFORM
exit 0
