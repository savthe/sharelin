#!/bin/sh

DISTNAME="sharelin"

if [ $# -ge 2 ]
then
	echo "Usage: $0 [suffix]"
	exit 1
fi

if [ $# -eq 1 ]
then
	DISTNAME="$DISTNAME-$1"
else
	DISTNAME="$DISTNAME-r`svnversion .`"

fi

svn export . $DISTNAME
tar -czf $DISTNAME-src.tar.gz $DISTNAME
svn delete --force $DISTNAME
