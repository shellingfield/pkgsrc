$NetBSD: patch-ipc_chromium_src_base_file__version__info.h,v 1.2 2014/02/08 09:36:00 ryoon Exp $

* Treat Solaris/SunOS like Linux, fix build.

--- ipc/chromium/src/base/file_version_info.h.orig	2014-01-28 04:03:44.000000000 +0000
+++ ipc/chromium/src/base/file_version_info.h
@@ -86,7 +86,7 @@ class FileVersionInfo {
   explicit FileVersionInfo(NSBundle *bundle);
 
   NSBundle *bundle_;
-#elif defined(OS_LINUX)
+#elif defined(OS_LINUX) || defined(OS_SOLARIS)
   FileVersionInfo();
 #endif
 
