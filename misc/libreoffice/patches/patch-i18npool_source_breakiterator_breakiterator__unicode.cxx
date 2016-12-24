$NetBSD: patch-i18npool_source_breakiterator_breakiterator__unicode.cxx,v 1.2 2016/12/24 21:45:03 ryoon Exp $

Unbreak build break caused by icu 58 (from FreeBSD)

--- i18npool/source/breakiterator/breakiterator_unicode.cxx.orig	2016-12-13 16:57:41.000000000 +0000
+++ i18npool/source/breakiterator/breakiterator_unicode.cxx
@@ -63,10 +63,13 @@ BreakIterator_Unicode::~BreakIterator_Un
 class OOoRuleBasedBreakIterator : public RuleBasedBreakIterator
 {
     public:
+#if (U_ICU_VERSION_MAJOR_NUM < 58)
+    // RuleBasedBreakIterator::setBreakType() is private as of ICU 58.
     inline void publicSetBreakType(int32_t type)
         {
             setBreakType(type);
         };
+#endif
     OOoRuleBasedBreakIterator(UDataMemory* image,
                               UErrorCode &status)
         : RuleBasedBreakIterator(image, status)
@@ -142,12 +145,21 @@ void SAL_CALL BreakIterator_Unicode::loa
                 }
             }
             if (rbi) {
+#if (U_ICU_VERSION_MAJOR_NUM < 58)
+                // ICU 58 made RuleBasedBreakIterator::setBreakType() private
+                // instead of protected, so the old workaround of
+                // https://ssl.icu-project.org/trac/ticket/5498
+                // doesn't work anymore. However, they also claim to have fixed
+                // the cause that an initial fBreakType==-1 would lead to an
+                // endless loop under some circumstances.
+                // Let's see ...
                 switch (rBreakType) {
                     case LOAD_CHARACTER_BREAKITERATOR: rbi->publicSetBreakType(UBRK_CHARACTER); break;
                     case LOAD_WORD_BREAKITERATOR: rbi->publicSetBreakType(UBRK_WORD); break;
                     case LOAD_SENTENCE_BREAKITERATOR: rbi->publicSetBreakType(UBRK_SENTENCE); break;
                     case LOAD_LINE_BREAKITERATOR: rbi->publicSetBreakType(UBRK_LINE); break;
                 }
+#endif
                 icuBI->aBreakIterator = rbi;
             }
         }
