$NetBSD: patch-mozilla_toolkit_library_libxul.mk,v 1.5 2017/01/01 16:14:08 ryoon Exp $

--- mozilla/toolkit/library/libxul.mk.orig	2016-12-14 02:07:00.000000000 +0000
+++ mozilla/toolkit/library/libxul.mk
@@ -55,7 +55,7 @@ endif
 ifdef _MSC_VER
 get_first_and_last = dumpbin -exports $1 | grep _NSModule@@ | sort -k 3 | sed -n 's/^.*?\([^@]*\)@@.*$$/\1/;1p;$$p'
 else
-get_first_and_last = $(TOOLCHAIN_PREFIX)nm -g $1 | grep _NSModule$$ | grep -vw refptr | sort | sed -n 's/^.* _*\([^ ]*\)$$/\1/;1p;$$p'
+get_first_and_last = $(TOOLCHAIN_PREFIX)nm $1 | grep _NSModule$$ | grep -vw refptr | sort | sed -n 's/^.* _*\([^ ]*\)$$/\1/;1p;$$p'
 endif
 
 LOCAL_CHECKS = test "$$($(get_first_and_last) | xargs echo)" != "start_kPStaticModules_NSModule end_kPStaticModules_NSModule" && echo "NSModules are not ordered appropriately" && exit 1 || exit 0
