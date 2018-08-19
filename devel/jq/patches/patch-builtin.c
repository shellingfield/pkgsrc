$NetBSD: patch-builtin.c,v 1.1 2018/08/19 18:08:05 leot Exp $

Defining _GNU_SOURCE, _BSD_SOURCE etc. in C sources is problematic,
because the result of the configure command may be inconsistent with it.
to be consistent, such macros have to be defined in the early stage of
the configure command, and the AC_USE_SYSTEM_EXTENSIONS macro does the job.

Part of pull request 1458, commit id `df9a0963f8fa6fca773b059dce22c598152f3edb':

 <https://github.com/stedolan/jq/pull/1458>

Also shared via PR pkg/52460.

--- builtin.c.orig	2015-08-18 04:25:04.000000000 +0000
+++ builtin.c
@@ -1,6 +1,3 @@
-#define _BSD_SOURCE
-#define _GNU_SOURCE
-#define _XOPEN_SOURCE
 #include <sys/time.h>
 #include <stdlib.h>
 #include <stddef.h>
