$NetBSD: patch-src_extension_internal_pdfinput_pdf-parser.cpp,v 1.6 2017/09/09 21:48:56 prlw1 Exp $

- Object.h is included in pdf-parser.h -- see patch for pdf-parser.h.
- Support poppler 0.58
  https://gitlab.com/inkscape/inkscape/commit/9418824967eb4c53371ef8588243fed4cab496e0

--- src/extension/internal/pdfinput/pdf-parser.cpp.orig	2014-06-09 13:24:41.000000000 +0000
+++ src/extension/internal/pdfinput/pdf-parser.cpp
@@ -38,7 +38,7 @@ extern "C" {
 #include "goo/GooHash.h"
 #include "GlobalParams.h"
 #include "CharTypes.h"
-#include "Object.h"
+#define POPPLER_EVEN_NEWER_NEW_COLOR_SPACE_API 1
 #include "Array.h"
 #include "Dict.h"
 #include "Stream.h"
@@ -414,13 +414,21 @@ void PdfParser::parse(Object *obj, GBool
 
   if (obj->isArray()) {
     for (int i = 0; i < obj->arrayGetLength(); ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+      obj2 = obj->arrayGet(i);
+#else
       obj->arrayGet(i, &obj2);
+#endif
       if (!obj2.isStream()) {
 	error(errInternal, -1, "Weird page contents");
+#if !defined(POPPLER_NEW_OBJECT_API)
 	obj2.free();
+#endif
 	return;
       }
+#if !defined(POPPLER_NEW_OBJECT_API)
       obj2.free();
+#endif
     }
   } else if (!obj->isStream()) {
 	error(errInternal, -1, "Weird page contents");
@@ -439,7 +447,11 @@ void PdfParser::go(GBool /*topLevel*/)
 
   // scan a sequence of objects
   int numArgs = 0;
+#if defined(POPPLER_NEW_OBJECT_API)
+  obj = parser->getObj();
+#else
   parser->getObj(&obj);
+#endif
   while (!obj.isEOF()) {
 
     // got a command - execute it
@@ -457,14 +469,20 @@ void PdfParser::go(GBool /*topLevel*/)
       // Run the operation
       execOp(&obj, args, numArgs);
 
+#if !defined(POPPLER_NEW_OBJECT_API)
       obj.free();
       for (int i = 0; i < numArgs; ++i)
 	args[i].free();
+#endif
       numArgs = 0;
 
     // got an argument - save it
     } else if (numArgs < maxArgs) {
+#if defined(POPPLER_NEW_OBJECT_API)
+      args[numArgs++] = std::move(obj);
+#else
       args[numArgs++] = obj;
+#endif
 
     // too many arguments - something is wrong
     } else {
@@ -475,13 +493,21 @@ void PdfParser::go(GBool /*topLevel*/)
 	printf("\n");
 	fflush(stdout);
       }
+#if !defined(POPPLER_NEW_OBJECT_API)
       obj.free();
+#endif
     }
 
     // grab the next object
+#if defined(POPPLER_NEW_OBJECT_API)
+    obj = parser->getObj();
+#else
     parser->getObj(&obj);
+#endif
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj.free();
+#endif
 
   // args at end with no command
   if (numArgs > 0) {
@@ -495,8 +521,10 @@ void PdfParser::go(GBool /*topLevel*/)
       printf("\n");
       fflush(stdout);
     }
+#if !defined(POPPLER_NEW_OBJECT_API)
     for (int i = 0; i < numArgs; ++i)
       args[i].free();
+#endif
   }
 }
 
@@ -692,9 +720,13 @@ void PdfParser::opSetDash(Object args[],
   if (length != 0) {
     dash = (double *)gmallocn(length, sizeof(double));
     for (int i = 0; i < length; ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+      dash[i] = a->get(i).getNum();
+#else
       Object obj;
       dash[i] = a->get(i, &obj)->getNum();
       obj.free();
+#endif
     }
   }
   state->setLineDash(dash, length, args[1].getNum());
@@ -744,12 +776,18 @@ void PdfParser::opSetExtGState(Object ar
   GBool haveBackdropColor = gFalse;
   GBool alpha = gFalse;
 
+#if defined(POPPLER_NEW_OBJECT_API)
+  if ((obj1 = res->lookupGState(args[0].getName())).isNull()) {
+#else
   if (!res->lookupGState(args[0].getName(), &obj1)) {
+#endif
     return;
   }
   if (!obj1.isDict()) {
     error(errSyntaxError, getPos(), "ExtGState '{0:s}' is wrong type"), args[0].getName();
+#if !defined(POPPLER_NEW_OBJECT_API)
     obj1.free();
+#endif
     return;
   }
   if (printCommands) {
@@ -759,7 +797,11 @@ void PdfParser::opSetExtGState(Object ar
   }
 
   // transparency support: blend mode, fill/stroke opacity
+#if defined(POPPLER_NEW_OBJECT_API)
+  if (!((obj2 = obj1.dictLookup(const_cast<char*>("BM"))).isNull())) {
+#else
   if (!obj1.dictLookup(const_cast<char*>("BM"), &obj2)->isNull()) {
+#endif
     GfxBlendMode mode = gfxBlendNormal;
     if (state->parseBlendMode(&obj2, &mode)) {
       state->setBlendMode(mode);
@@ -767,40 +809,71 @@ void PdfParser::opSetExtGState(Object ar
       error(errSyntaxError, getPos(), "Invalid blend mode in ExtGState");
     }
   }
+#if defined(POPPLER_NEW_OBJECT_API)
+  if ((obj2 = obj1.dictLookup(const_cast<char*>("ca"))).isNum()) {
+#else
   obj2.free();
   if (obj1.dictLookup(const_cast<char*>("ca"), &obj2)->isNum()) {
+#endif
     state->setFillOpacity(obj2.getNum());
   }
+#if defined(POPPLER_NEW_OBJECT_API)
+  if ((obj2 = obj1.dictLookup(const_cast<char*>("CA"))).isNum()) {
+#else
   obj2.free();
   if (obj1.dictLookup(const_cast<char*>("CA"), &obj2)->isNum()) {
+#endif
     state->setStrokeOpacity(obj2.getNum());
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj2.free();
+#endif
 
   // fill/stroke overprint
   GBool haveFillOP = gFalse;
+#if defined(POPPLER_NEW_OBJECT_API)
+  if ((haveFillOP = (obj2 = obj1.dictLookup(const_cast<char*>("op"))).isBool())) {
+#else
   if ((haveFillOP = (obj1.dictLookup(const_cast<char*>("op"), &obj2)->isBool()))) {
+#endif
     state->setFillOverprint(obj2.getBool());
   }
+#if defined(POPPLER_NEW_OBJECT_API)
+  if ((obj2 = obj1.dictLookup(const_cast<char*>("OP"))).isBool()) {
+#else
   obj2.free();
   if (obj1.dictLookup(const_cast<char*>("OP"), &obj2)->isBool()) {
+#endif
     state->setStrokeOverprint(obj2.getBool());
     if (!haveFillOP) {
       state->setFillOverprint(obj2.getBool());
     }
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj2.free();
+#endif
 
   // stroke adjust
+#if defined(POPPLER_NEW_OBJECT_API)
+  if ((obj2 = obj1.dictLookup(const_cast<char*>("SA"))).isBool()) {
+#else
   if (obj1.dictLookup(const_cast<char*>("SA"), &obj2)->isBool()) {
+#endif
     state->setStrokeAdjust(obj2.getBool());
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj2.free();
+#endif
 
   // transfer function
+#if defined(POPPLER_NEW_OBJECT_API)
+  if ((obj2 = obj1.dictLookup(const_cast<char*>("TR2"))).isNull()) {
+    obj2 = obj1.dictLookup(const_cast<char*>("TR"));
+#else
   if (obj1.dictLookup(const_cast<char*>("TR2"), &obj2)->isNull()) {
     obj2.free();
     obj1.dictLookup(const_cast<char*>("TR"), &obj2);
+#endif
   }
   if (obj2.isName(const_cast<char*>("Default")) ||
       obj2.isName(const_cast<char*>("Identity"))) {
@@ -809,9 +882,15 @@ void PdfParser::opSetExtGState(Object ar
   } else if (obj2.isArray() && obj2.arrayGetLength() == 4) {
     int pos = 4;
     for (int i = 0; i < 4; ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+      obj3 = obj2.arrayGet(i);
+#else
       obj2.arrayGet(i, &obj3);
+#endif
       funcs[i] = Function::parse(&obj3);
+#if !defined(POPPLER_NEW_OBJECT_API)
       obj3.free();
+#endif
       if (!funcs[i]) {
 	pos = i;
 	break;
@@ -828,21 +907,37 @@ void PdfParser::opSetExtGState(Object ar
   } else if (!obj2.isNull()) {
     error(errSyntaxError, getPos(), "Invalid transfer function in ExtGState");
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj2.free();
+#endif
 
   // soft mask
+#if defined(POPPLER_NEW_OBJECT_API)
+  if (!((obj2 = obj1.dictLookup(const_cast<char*>("SMask"))).isNull())) {
+#else
   if (!obj1.dictLookup(const_cast<char*>("SMask"), &obj2)->isNull()) {
+#endif
     if (obj2.isName(const_cast<char*>("None"))) {
       builder->clearSoftMask(state);
     } else if (obj2.isDict()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+      if ((obj3 = obj2.dictLookup(const_cast<char*>("S"))).isName(const_cast<char*>("Alpha"))) {
+#else
       if (obj2.dictLookup(const_cast<char*>("S"), &obj3)->isName(const_cast<char*>("Alpha"))) {
+#endif
 	alpha = gTrue;
       } else { // "Luminosity"
 	alpha = gFalse;
       }
+#if !defined(POPPLER_NEW_OBJECT_API)
       obj3.free();
+#endif
       funcs[0] = NULL;
+#if defined(POPPLER_NEW_OBJECT_API)
+      if (!((obj3 = obj2.dictLookup(const_cast<char*>("TR"))).isNull())) {
+#else
       if (!obj2.dictLookup(const_cast<char*>("TR"), &obj3)->isNull()) {
+#endif
 	funcs[0] = Function::parse(&obj3);
 	if (funcs[0]->getInputSize() != 1 ||
 	    funcs[0]->getOutputSize() != 1) {
@@ -851,26 +946,45 @@ void PdfParser::opSetExtGState(Object ar
 	  funcs[0] = NULL;
 	}
       }
+#if defined(POPPLER_NEW_OBJECT_API)
+      if ((haveBackdropColor = (obj3 = obj2.dictLookup(const_cast<char*>("BC"))).isArray())) {
+#else
       obj3.free();
       if ((haveBackdropColor = obj2.dictLookup(const_cast<char*>("BC"), &obj3)->isArray())) {
+#endif
 	for (int i = 0; i < gfxColorMaxComps; ++i) {
 	  backdropColor.c[i] = 0;
 	}
 	for (int i = 0; i < obj3.arrayGetLength() && i < gfxColorMaxComps; ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+	  obj4 = obj3.arrayGet(i);
+#else
 	  obj3.arrayGet(i, &obj4);
+#endif
 	  if (obj4.isNum()) {
 	    backdropColor.c[i] = dblToCol(obj4.getNum());
 	  }
+#if !defined(POPPLER_NEW_OBJECT_API)
 	  obj4.free();
+#endif
 	}
       }
+#if defined(POPPLER_NEW_OBJECT_API)
+      if ((obj3 = obj2.dictLookup(const_cast<char*>("G"))).isStream()) {
+	if ((obj4 = obj3.streamGetDict()->lookup(const_cast<char*>("Group"))).isDict()) {
+#else
       obj3.free();
       if (obj2.dictLookup(const_cast<char*>("G"), &obj3)->isStream()) {
 	if (obj3.streamGetDict()->lookup(const_cast<char*>("Group"), &obj4)->isDict()) {
+#endif
 	  GfxColorSpace *blendingColorSpace = 0;
 	  GBool isolated = gFalse;
 	  GBool knockout = gFalse;
+#if defined(POPPLER_NEW_OBJECT_API)
+	  if (!((obj5 = obj4.dictLookup(const_cast<char*>("CS"))).isNull())) {
+#else
 	  if (!obj4.dictLookup(const_cast<char*>("CS"), &obj5)->isNull()) {
+#endif
 #if defined(POPPLER_EVEN_NEWER_NEW_COLOR_SPACE_API)
 	    blendingColorSpace = GfxColorSpace::parse(NULL, &obj5, NULL, NULL);
 #elif defined(POPPLER_EVEN_NEWER_COLOR_SPACE_API)
@@ -879,15 +993,25 @@ void PdfParser::opSetExtGState(Object ar
 	    blendingColorSpace = GfxColorSpace::parse(&obj5, NULL);
 #endif
 	  }
+#if defined(POPPLER_NEW_OBJECT_API)
+	  if ((obj5 = obj4.dictLookup(const_cast<char*>("I"))).isBool()) {
+#else
 	  obj5.free();
 	  if (obj4.dictLookup(const_cast<char*>("I"), &obj5)->isBool()) {
+#endif
 	    isolated = obj5.getBool();
 	  }
+#if defined(POPPLER_NEW_OBJECT_API)
+	  if ((obj5 = obj4.dictLookup(const_cast<char*>("K"))).isBool()) {
+#else
 	  obj5.free();
 	  if (obj4.dictLookup(const_cast<char*>("K"), &obj5)->isBool()) {
+#endif
 	    knockout = obj5.getBool();
 	  }
+#if !defined(POPPLER_NEW_OBJECT_API)
 	  obj5.free();
+#endif
 	  if (!haveBackdropColor) {
 	    if (blendingColorSpace) {
 	      blendingColorSpace->getDefaultColor(&backdropColor);
@@ -906,18 +1030,24 @@ void PdfParser::opSetExtGState(Object ar
 	} else {
 	  error(errSyntaxError, getPos(), "Invalid soft mask in ExtGState - missing group");
 	}
+#if !defined(POPPLER_NEW_OBJECT_API)
 	obj4.free();
+#endif
       } else {
 	error(errSyntaxError, getPos(), "Invalid soft mask in ExtGState - missing group");
       }
+#if !defined(POPPLER_NEW_OBJECT_API)
       obj3.free();
+#endif
     } else if (!obj2.isNull()) {
       error(errSyntaxError, getPos(), "Invalid soft mask in ExtGState");
     }
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj2.free();
 
   obj1.free();
+#endif
 }
 
 void PdfParser::doSoftMask(Object *str, GBool alpha,
@@ -938,43 +1068,79 @@ void PdfParser::doSoftMask(Object *str, 
   dict = str->streamGetDict();
 
   // check form type
+#if defined(POPPLER_NEW_OBJECT_API)
+  obj1 = dict->lookup(const_cast<char*>("FormType"));
+#else
   dict->lookup(const_cast<char*>("FormType"), &obj1);
+#endif
   if (!(obj1.isNull() || (obj1.isInt() && obj1.getInt() == 1))) {
     error(errSyntaxError, getPos(), "Unknown form type");
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj1.free();
+#endif
 
   // get bounding box
+#if defined(POPPLER_NEW_OBJECT_API)
+  obj1 = dict->lookup(const_cast<char*>("BBox"));
+#else
   dict->lookup(const_cast<char*>("BBox"), &obj1);
+#endif
   if (!obj1.isArray()) {
+#if !defined(POPPLER_NEW_OBJECT_API)
     obj1.free();
+#endif
     error(errSyntaxError, getPos(), "Bad form bounding box");
     return;
   }
   for (i = 0; i < 4; ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+    obj2 = obj1.arrayGet(i);
+#else
     obj1.arrayGet(i, &obj2);
+#endif
     bbox[i] = obj2.getNum();
+#if defined(POPPLER_NEW_OBJECT_API)
+  }
+#else
     obj2.free();
   }
   obj1.free();
+#endif
 
   // get matrix
+#if defined(POPPLER_NEW_OBJECT_API)
+  obj1 = dict->lookup(const_cast<char*>("Matrix"));
+#else
   dict->lookup(const_cast<char*>("Matrix"), &obj1);
+#endif
   if (obj1.isArray()) {
     for (i = 0; i < 6; ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+      obj2 = obj1.arrayGet(i);
+#else
       obj1.arrayGet(i, &obj2);
+#endif
       m[i] = obj2.getNum();
+#if !defined(POPPLER_NEW_OBJECT_API)
       obj2.free();
+#endif
     }
   } else {
     m[0] = 1; m[1] = 0;
     m[2] = 0; m[3] = 1;
     m[4] = 0; m[5] = 0;
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj1.free();
+#endif
 
   // get resources
+#if defined(POPPLER_NEW_OBJECT_API)
+  obj1 = dict->lookup(const_cast<char*>("Resources"));
+#else
   dict->lookup(const_cast<char*>("Resources"), &obj1);
+#endif
   resDict = obj1.isDict() ? obj1.getDict() : (Dict *)NULL;
 
   // draw it
@@ -987,7 +1153,9 @@ void PdfParser::doSoftMask(Object *str, 
   if (blendingColorSpace) {
     delete blendingColorSpace;
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj1.free();
+#endif
 }
 
 void PdfParser::opSetRenderingIntent(Object /*args*/[], int /*numArgs*/)
@@ -1084,7 +1252,11 @@ void PdfParser::opSetFillColorSpace(Obje
   Object obj;
 
   state->setFillPattern(NULL);
+#if defined(POPPLER_NEW_OBJECT_API)
+  obj = res->lookupColorSpace(args[0].getName());
+#else
   res->lookupColorSpace(args[0].getName(), &obj);
+#endif
 
   GfxColorSpace *colorSpace = 0;
 #if defined(POPPLER_EVEN_NEWER_NEW_COLOR_SPACE_API)
@@ -1106,7 +1278,9 @@ void PdfParser::opSetFillColorSpace(Obje
     colorSpace = GfxColorSpace::parse(&obj, NULL);
   }
 #endif
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj.free();
+#endif
   if (colorSpace) {
   GfxColor color;
     state->setFillColorSpace(colorSpace);
@@ -1125,7 +1299,11 @@ void PdfParser::opSetStrokeColorSpace(Ob
   GfxColorSpace *colorSpace = 0;
 
   state->setStrokePattern(NULL);
+#if defined(POPPLER_NEW_OBJECT_API)
+  obj = res->lookupColorSpace(args[0].getName());
+#else
   res->lookupColorSpace(args[0].getName(), &obj);
+#endif
 #if defined(POPPLER_EVEN_NEWER_NEW_COLOR_SPACE_API)
   if (obj.isNull()) {
     colorSpace = GfxColorSpace::parse(NULL, &args[0], NULL, NULL);
@@ -1145,7 +1323,9 @@ void PdfParser::opSetStrokeColorSpace(Ob
     colorSpace = GfxColorSpace::parse(&obj, NULL);
   }
 #endif
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj.free();
+#endif
   if (colorSpace) {
     GfxColor color;
     state->setStrokeColorSpace(colorSpace);
@@ -2375,7 +2555,11 @@ void PdfParser::opShowSpaceText(Object a
   wMode = state->getFont()->getWMode();
   a = args[0].getArray();
   for (int i = 0; i < a->getLength(); ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+    obj = a->get(i);
+#else
     a->get(i, &obj);
+#endif
     if (obj.isNum()) {
       // this uses the absolute value of the font size to match
       // Acrobat's behavior
@@ -2392,7 +2576,9 @@ void PdfParser::opShowSpaceText(Object a
     } else {
       error(errSyntaxError, getPos(), "Element of show/space array must be number or string");
     }
+#if !defined(POPPLER_NEW_OBJECT_API)
     obj.free();
+#endif
   }
 }
 
@@ -2465,7 +2651,11 @@ void PdfParser::doShowText(GooString *s)
       //out->updateCTM(state, 1, 0, 0, 1, 0, 0);
       if (0){ /*!out->beginType3Char(state, curX + riseX, curY + riseY, tdx, tdy,
 			       code, u, uLen)) {*/
+#if defined(POPPLER_NEW_OBJECT_API)
+	charProc = ((Gfx8BitFont *)font)->getCharProc(code);
+#else
 	((Gfx8BitFont *)font)->getCharProc(code, &charProc);
+#endif
 	if ((resDict = ((Gfx8BitFont *)font)->getResources())) {
 	  pushResources(resDict);
 	}
@@ -2478,7 +2668,9 @@ void PdfParser::doShowText(GooString *s)
 	if (resDict) {
 	  popResources();
 	}
+#if !defined(POPPLER_NEW_OBJECT_API)
 	charProc.free();
+#endif
       }
       restoreState();
       // GfxState::restore() does *not* restore the current position,
@@ -2541,23 +2733,43 @@ void PdfParser::opXObject(Object args[],
   Object obj1, obj2, obj3, refObj;
 
   char *name = args[0].getName();
+#if defined(POPPLER_NEW_OBJECT_API)
+  if ((obj1 = res->lookupXObject(name)).isNull()) {
+#else
   if (!res->lookupXObject(name, &obj1)) {
+#endif
     return;
   }
   if (!obj1.isStream()) {
     error(errSyntaxError, getPos(), "XObject '{0:s}' is wrong type", name);
+#if !defined(POPPLER_NEW_OBJECT_API)
     obj1.free();
+#endif
     return;
   }
+#if defined(POPPLER_NEW_OBJECT_API)
+  obj2 = obj1.streamGetDict()->lookup(const_cast<char*>("Subtype"));
+#else
   obj1.streamGetDict()->lookup(const_cast<char*>("Subtype"), &obj2);
+#endif
   if (obj2.isName(const_cast<char*>("Image"))) {
+#if defined(POPPLER_NEW_OBJECT_API)
+    refObj = res->lookupXObjectNF(name);
+#else
     res->lookupXObjectNF(name, &refObj);
+#endif
     doImage(&refObj, obj1.getStream(), gFalse);
+#if !defined(POPPLER_NEW_OBJECT_API)
     refObj.free();
+#endif
   } else if (obj2.isName(const_cast<char*>("Form"))) {
     doForm(&obj1);
   } else if (obj2.isName(const_cast<char*>("PS"))) {
+#if defined(POPPLER_NEW_OBJECT_API)
+    obj3 = obj1.streamGetDict()->lookup(const_cast<char*>("Level1"));
+#else
     obj1.streamGetDict()->lookup(const_cast<char*>("Level1"), &obj3);
+#endif
 /*    out->psXObject(obj1.getStream(),
     		   obj3.isStream() ? obj3.getStream() : (Stream *)NULL);*/
   } else if (obj2.isName()) {
@@ -2565,8 +2777,10 @@ void PdfParser::opXObject(Object args[],
   } else {
     error(errSyntaxError, getPos(), "XObject subtype is missing or wrong type");
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj2.free();
   obj1.free();
+#endif
 }
 
 void PdfParser::doImage(Object * /*ref*/, Stream *str, GBool inlineImg)
@@ -2593,10 +2807,18 @@ void PdfParser::doImage(Object * /*ref*/
     dict = str->getDict();
     
     // get size
+#if defined(POPPLER_NEW_OBJECT_API)
+    obj1 = dict->lookup(const_cast<char*>("Width"));
+#else
     dict->lookup(const_cast<char*>("Width"), &obj1);
+#endif
     if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+        obj1 = dict->lookup(const_cast<char*>("W"));
+#else
         obj1.free();
         dict->lookup(const_cast<char*>("W"), &obj1);
+#endif
     }
     if (obj1.isInt()){
         width = obj1.getInt();
@@ -2607,11 +2829,19 @@ void PdfParser::doImage(Object * /*ref*/
     else {
         goto err2;
     }
+#if defined(POPPLER_NEW_OBJECT_API)
+    obj1 = dict->lookup(const_cast<char*>("Height"));
+#else
     obj1.free();
     dict->lookup(const_cast<char*>("Height"), &obj1);
+#endif
     if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+        obj1 = dict->lookup(const_cast<char*>("H"));
+#else
         obj1.free();
         dict->lookup(const_cast<char*>("H"), &obj1);
+#endif
     }
     if (obj1.isInt()) {
         height = obj1.getInt();
@@ -2622,26 +2852,46 @@ void PdfParser::doImage(Object * /*ref*/
     else {
         goto err2;
     }
+#if !defined(POPPLER_NEW_OBJECT_API)
     obj1.free();
+#endif
     
     // image interpolation
+#if defined(POPPLER_NEW_OBJECT_API)
+    obj1 = dict->lookup("Interpolate");
+#else
     dict->lookup("Interpolate", &obj1);
+#endif
     if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+      obj1 = dict->lookup("I");
+#else
       obj1.free();
       dict->lookup("I", &obj1);
+#endif
     }
     if (obj1.isBool())
       interpolate = obj1.getBool();
     else
       interpolate = gFalse;
+#if !defined(POPPLER_NEW_OBJECT_API)
     obj1.free();
+#endif
     maskInterpolate = gFalse;
 
     // image or mask?
+#if defined(POPPLER_NEW_OBJECT_API)
+    obj1 = dict->lookup(const_cast<char*>("ImageMask"));
+#else
     dict->lookup(const_cast<char*>("ImageMask"), &obj1);
+#endif
     if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+        obj1 = dict->lookup(const_cast<char*>("IM"));
+#else
         obj1.free();
         dict->lookup(const_cast<char*>("IM"), &obj1);
+#endif
     }
     mask = gFalse;
     if (obj1.isBool()) {
@@ -2650,14 +2900,24 @@ void PdfParser::doImage(Object * /*ref*/
     else if (!obj1.isNull()) {
         goto err2;
     }
+#if !defined(POPPLER_NEW_OBJECT_API)
     obj1.free();
+#endif
     
     // bit depth
     if (bits == 0) {
+#if defined(POPPLER_NEW_OBJECT_API)
+        obj1 = dict->lookup(const_cast<char*>("BitsPerComponent"));
+#else
         dict->lookup(const_cast<char*>("BitsPerComponent"), &obj1);
+#endif
         if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = dict->lookup(const_cast<char*>("BPC"));
+#else
             obj1.free();
             dict->lookup(const_cast<char*>("BPC"), &obj1);
+#endif
         }
         if (obj1.isInt()) {
             bits = obj1.getInt();
@@ -2666,7 +2926,9 @@ void PdfParser::doImage(Object * /*ref*/
         } else {
             goto err2;
         }
+#if !defined(POPPLER_NEW_OBJECT_API)
         obj1.free();
+#endif
     }
     
     // display a mask
@@ -2676,21 +2938,37 @@ void PdfParser::doImage(Object * /*ref*/
             goto err1;
         }
         invert = gFalse;
+#if defined(POPPLER_NEW_OBJECT_API)
+        obj1 = dict->lookup(const_cast<char*>("Decode"));
+#else
         dict->lookup(const_cast<char*>("Decode"), &obj1);
+#endif
         if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = dict->lookup(const_cast<char*>("D"));
+#else
             obj1.free();
             dict->lookup(const_cast<char*>("D"), &obj1);
+#endif
         }
         if (obj1.isArray()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj2 = obj1.arrayGet(0);
+#else
             obj1.arrayGet(0, &obj2);
+#endif
             if (obj2.isInt() && obj2.getInt() == 1) {
                 invert = gTrue;
             }
+#if !defined(POPPLER_NEW_OBJECT_API)
             obj2.free();
+#endif
         } else if (!obj1.isNull()) {
             goto err2;
         }
+#if !defined(POPPLER_NEW_OBJECT_API)
         obj1.free();
+#endif
         
         // draw it
         builder->addImageMask(state, str, width, height, invert, interpolate);
@@ -2698,18 +2976,36 @@ void PdfParser::doImage(Object * /*ref*/
     } else {
         // get color space and color map
         GfxColorSpace *colorSpace;
+#if defined(POPPLER_NEW_OBJECT_API)
+        obj1 = dict->lookup(const_cast<char*>("ColorSpace"));
+#else
         dict->lookup(const_cast<char*>("ColorSpace"), &obj1);
+#endif
         if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = dict->lookup(const_cast<char*>("CS"));
+#else
             obj1.free();
             dict->lookup(const_cast<char*>("CS"), &obj1);
+#endif
         }
         if (obj1.isName()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj2 = res->lookupColorSpace(obj1.getName());
+#else
             res->lookupColorSpace(obj1.getName(), &obj2);
+#endif
             if (!obj2.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                    obj1 = std::move(obj2);
+#else
 	            obj1.free();
 	            obj1 = obj2;
+#endif
             } else {
+#if !defined(POPPLER_NEW_OBJECT_API)
 	            obj2.free();
+#endif
             }
         }
         if (!obj1.isNull()) {
@@ -2729,17 +3025,29 @@ void PdfParser::doImage(Object * /*ref*/
         } else {
             colorSpace = NULL;
         }
+#if !defined(POPPLER_NEW_OBJECT_API)
         obj1.free();
+#endif
         if (!colorSpace) {
             goto err1;
         }
+#if defined(POPPLER_NEW_OBJECT_API)
+        obj1 = dict->lookup(const_cast<char*>("Decode"));
+#else
         dict->lookup(const_cast<char*>("Decode"), &obj1);
+#endif
         if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = dict->lookup(const_cast<char*>("D"));
+#else
             obj1.free();
             dict->lookup(const_cast<char*>("D"), &obj1);
+#endif
         }
         GfxImageColorMap *colorMap = new GfxImageColorMap(bits, &obj1, colorSpace);
+#if !defined(POPPLER_NEW_OBJECT_API)
         obj1.free();
+#endif
         if (!colorMap->isOk()) {
             delete colorMap;
             goto err1;
@@ -2753,8 +3061,13 @@ void PdfParser::doImage(Object * /*ref*/
         int maskHeight = 0;
         maskInvert = gFalse;
         GfxImageColorMap *maskColorMap = NULL;
+#if defined(POPPLER_NEW_OBJECT_API)
+        maskObj = dict->lookup(const_cast<char*>("Mask"));
+        smaskObj = dict->lookup(const_cast<char*>("SMask"));
+#else
         dict->lookup(const_cast<char*>("Mask"), &maskObj);
         dict->lookup(const_cast<char*>("SMask"), &smaskObj);
+#endif
         Dict* maskDict;
         if (smaskObj.isStream()) {
             // soft mask
@@ -2763,58 +3076,108 @@ void PdfParser::doImage(Object * /*ref*/
             }
             maskStr = smaskObj.getStream();
             maskDict = smaskObj.streamGetDict();
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("Width"));
+#else
             maskDict->lookup(const_cast<char*>("Width"), &obj1);
+#endif
             if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+	            obj1 = maskDict->lookup(const_cast<char*>("W"));
+#else
         	    obj1.free();
 	            maskDict->lookup(const_cast<char*>("W"), &obj1);
+#endif
             }
             if (!obj1.isInt()) {
 	            goto err2;
             }
             maskWidth = obj1.getInt();
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("Height"));
+#else
             obj1.free();
             maskDict->lookup(const_cast<char*>("Height"), &obj1);
+#endif
             if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                    obj1 = maskDict->lookup(const_cast<char*>("H"));
+#else
 	            obj1.free();
 	            maskDict->lookup(const_cast<char*>("H"), &obj1);
+#endif
             }
             if (!obj1.isInt()) {
 	            goto err2;
             }
             maskHeight = obj1.getInt();
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("BitsPerComponent"));
+#else
             obj1.free();
             maskDict->lookup(const_cast<char*>("BitsPerComponent"), &obj1);
+#endif
             if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                    obj1 = maskDict->lookup(const_cast<char*>("BPC"));
+#else
         	    obj1.free();
 	            maskDict->lookup(const_cast<char*>("BPC"), &obj1);
+#endif
             }
             if (!obj1.isInt()) {
 	            goto err2;
             }
             int maskBits = obj1.getInt();
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("Interpolate"));
+#else
             obj1.free();
 	    maskDict->lookup("Interpolate", &obj1);
+#endif
 	    if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+              obj1 = maskDict->lookup(const_cast<char*>("I"));
+#else
 	      obj1.free();
 	      maskDict->lookup("I", &obj1);
+#endif
 	    }
 	    if (obj1.isBool())
 	      maskInterpolate = obj1.getBool();
 	    else
 	      maskInterpolate = gFalse;
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("ColorSpace"));
+#else
 	    obj1.free();
             maskDict->lookup(const_cast<char*>("ColorSpace"), &obj1);
+#endif
             if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                    obj1 = maskDict->lookup(const_cast<char*>("CS"));
+#else
 	            obj1.free();
 	            maskDict->lookup(const_cast<char*>("CS"), &obj1);
+#endif
             }
             if (obj1.isName()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+	            obj2 = res->lookupColorSpace(obj1.getName());
+#else
 	            res->lookupColorSpace(obj1.getName(), &obj2);
+#endif
 	            if (!obj2.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                        obj1 = std::move(obj2);
+#else
 	                obj1.free();
     	            obj1 = obj2;
+#endif
 	            } else {
+#if !defined(POPPLER_NEW_OBJECT_API)
 	                obj2.free();
+#endif
 	            }
             }
 #if defined(POPPLER_EVEN_NEWER_NEW_COLOR_SPACE_API)
@@ -2824,17 +3187,29 @@ void PdfParser::doImage(Object * /*ref*/
 #else
             GfxColorSpace *maskColorSpace = GfxColorSpace::parse(&obj1, NULL);
 #endif
+#if !defined(POPPLER_NEW_OBJECT_API)
             obj1.free();
+#endif
             if (!maskColorSpace || maskColorSpace->getMode() != csDeviceGray) {
                 goto err1;
             }
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("Decode"));
+#else
             maskDict->lookup(const_cast<char*>("Decode"), &obj1);
+#endif
             if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                obj1 = maskDict->lookup(const_cast<char*>("D"));
+#else
 	            obj1.free();
     	        maskDict->lookup(const_cast<char*>("D"), &obj1);
+#endif
             }
             maskColorMap = new GfxImageColorMap(maskBits, &obj1, maskColorSpace);
+#if !defined(POPPLER_NEW_OBJECT_API)
             obj1.free();
+#endif
             if (!maskColorMap->isOk()) {
                 delete maskColorMap;
                 goto err1;
@@ -2845,9 +3220,15 @@ void PdfParser::doImage(Object * /*ref*/
             // color key mask
             int i;
             for (i = 0; i < maskObj.arrayGetLength() && i < 2*gfxColorMaxComps; ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                obj1 = maskObj.arrayGet(i);
+#else
                 maskObj.arrayGet(i, &obj1);
+#endif
                 maskColors[i] = obj1.getInt();
+#if !defined(POPPLER_NEW_OBJECT_API)
                 obj1.free();
+#endif
             }
               haveColorKeyMask = gTrue;
         } else if (maskObj.isStream()) {
@@ -2857,61 +3238,111 @@ void PdfParser::doImage(Object * /*ref*/
             }
             maskStr = maskObj.getStream();
             maskDict = maskObj.streamGetDict();
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("Width"));
+#else
             maskDict->lookup(const_cast<char*>("Width"), &obj1);
+#endif
             if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                obj1 = maskDict->lookup(const_cast<char*>("W"));
+#else
                 obj1.free();
                 maskDict->lookup(const_cast<char*>("W"), &obj1);
+#endif
             }
             if (!obj1.isInt()) {
                 goto err2;
             }
             maskWidth = obj1.getInt();
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("Height"));
+#else
             obj1.free();
             maskDict->lookup(const_cast<char*>("Height"), &obj1);
+#endif
             if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                obj1 = maskDict->lookup(const_cast<char*>("H"));
+#else
                 obj1.free();
                 maskDict->lookup(const_cast<char*>("H"), &obj1);
+#endif
             }
             if (!obj1.isInt()) {
                 goto err2;
             }
             maskHeight = obj1.getInt();
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("ImageMask"));
+#else
             obj1.free();
             maskDict->lookup(const_cast<char*>("ImageMask"), &obj1);
+#endif
             if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                obj1 = maskDict->lookup(const_cast<char*>("IM"));
+#else
                 obj1.free();
                 maskDict->lookup(const_cast<char*>("IM"), &obj1);
+#endif
             }
             if (!obj1.isBool() || !obj1.getBool()) {
                 goto err2;
             }
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup("Interpolate");
+#else
             obj1.free();
 	    maskDict->lookup("Interpolate", &obj1);
+#endif
 	    if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+	      obj1 = maskDict->lookup("I");
+#else
 	      obj1.free();
 	      maskDict->lookup("I", &obj1);
+#endif
 	    }
 	    if (obj1.isBool())
 	      maskInterpolate = obj1.getBool();
 	    else
 	      maskInterpolate = gFalse;
+#if !defined(POPPLER_NEW_OBJECT_API)
 	    obj1.free();
+#endif
             maskInvert = gFalse;
+#if defined(POPPLER_NEW_OBJECT_API)
+            obj1 = maskDict->lookup(const_cast<char*>("Decode"));
+#else
             maskDict->lookup(const_cast<char*>("Decode"), &obj1);
+#endif
             if (obj1.isNull()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                obj1 = maskDict->lookup(const_cast<char*>("D"));
+#else
                 obj1.free();
                 maskDict->lookup(const_cast<char*>("D"), &obj1);
+#endif
             }
             if (obj1.isArray()) {
+#if defined(POPPLER_NEW_OBJECT_API)
+                obj2 = obj1.arrayGet(0);
+#else
                 obj1.arrayGet(0, &obj2);
+#endif
                 if (obj2.isInt() && obj2.getInt() == 1) {
                     maskInvert = gTrue;
                 }
+#if !defined(POPPLER_NEW_OBJECT_API)
                 obj2.free();
+#endif
             } else if (!obj1.isNull()) {
                 goto err2;
             }
+#if !defined(POPPLER_NEW_OBJECT_API)
             obj1.free();
+#endif
             haveExplicitMask = gTrue;
         }
         
@@ -2929,14 +3360,18 @@ void PdfParser::doImage(Object * /*ref*/
         }
         delete colorMap;
         
+#if !defined(POPPLER_NEW_OBJECT_API)
         maskObj.free();
         smaskObj.free();
+#endif
     }
 
     return;
 
  err2:
+#if !defined(POPPLER_NEW_OBJECT_API)
     obj1.free();
+#endif
  err1:
     error(errSyntaxError, getPos(), "Bad image parameters");
 }
@@ -2961,52 +3396,97 @@ void PdfParser::doForm(Object *str) {
   dict = str->streamGetDict();
 
   // check form type
+#if defined(POPPLER_NEW_OBJECT_API)
+  obj1 = dict->lookup(const_cast<char*>("FormType"));
+#else
   dict->lookup(const_cast<char*>("FormType"), &obj1);
+#endif
   if (!(obj1.isNull() || (obj1.isInt() && obj1.getInt() == 1))) {
     error(errSyntaxError, getPos(), "Unknown form type");
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj1.free();
+#endif
 
   // get bounding box
+#if defined(POPPLER_NEW_OBJECT_API)
+  bboxObj = dict->lookup(const_cast<char*>("BBox"));
+#else
   dict->lookup(const_cast<char*>("BBox"), &bboxObj);
+#endif
   if (!bboxObj.isArray()) {
+#if !defined(POPPLER_NEW_OBJECT_API)
     bboxObj.free();
+#endif
     error(errSyntaxError, getPos(), "Bad form bounding box");
     return;
   }
   for (i = 0; i < 4; ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+    obj1 = bboxObj.arrayGet(i);
+#else
     bboxObj.arrayGet(i, &obj1);
+#endif
     bbox[i] = obj1.getNum();
+#if defined(POPPLER_NEW_OBJECT_API)
+  }
+#else
     obj1.free();
   }
   bboxObj.free();
+#endif
 
   // get matrix
+#if defined(POPPLER_NEW_OBJECT_API)
+  matrixObj = dict->lookup(const_cast<char*>("Matrix"));
+#else
   dict->lookup(const_cast<char*>("Matrix"), &matrixObj);
+#endif
   if (matrixObj.isArray()) {
     for (i = 0; i < 6; ++i) {
+#if defined(POPPLER_NEW_OBJECT_API)
+      obj1 = matrixObj.arrayGet(i);
+#else
       matrixObj.arrayGet(i, &obj1);
+#endif
       m[i] = obj1.getNum();
+#if !defined(POPPLER_NEW_OBJECT_API)
       obj1.free();
+#endif
     }
   } else {
     m[0] = 1; m[1] = 0;
     m[2] = 0; m[3] = 1;
     m[4] = 0; m[5] = 0;
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   matrixObj.free();
+#endif
 
   // get resources
+#if defined(POPPLER_NEW_OBJECT_API)
+  resObj = dict->lookup(const_cast<char*>("Resources"));
+#else
   dict->lookup(const_cast<char*>("Resources"), &resObj);
+#endif
   resDict = resObj.isDict() ? resObj.getDict() : (Dict *)NULL;
 
   // check for a transparency group
   transpGroup = isolated = knockout = gFalse;
   blendingColorSpace = NULL;
+#if defined(POPPLER_NEW_OBJECT_API)
+  if ((obj1 = dict->lookup(const_cast<char*>("Group"))).isDict()) {
+    if ((obj2 = obj1.dictLookup(const_cast<char*>("S"))).isName(const_cast<char*>("Transparency"))) {
+#else
   if (dict->lookup(const_cast<char*>("Group"), &obj1)->isDict()) {
     if (obj1.dictLookup(const_cast<char*>("S"), &obj2)->isName(const_cast<char*>("Transparency"))) {
+#endif
       transpGroup = gTrue;
+#if defined(POPPLER_NEW_OBJECT_API)
+      if (!((obj3 = obj1.dictLookup(const_cast<char*>("CS"))).isNull())) {
+#else
       if (!obj1.dictLookup(const_cast<char*>("CS"), &obj3)->isNull()) {
+#endif
 #if defined(POPPLER_EVEN_NEWER_NEW_COLOR_SPACE_API)
 	blendingColorSpace = GfxColorSpace::parse(NULL, &obj3, NULL, NULL);
 #elif defined(POPPLER_EVEN_NEWER_COLOR_SPACE_API)
@@ -3015,19 +3495,32 @@ void PdfParser::doForm(Object *str) {
 	blendingColorSpace = GfxColorSpace::parse(&obj3, NULL);
 #endif
       }
+#if defined(POPPLER_NEW_OBJECT_API)
+      if ((obj3 = obj1.dictLookup(const_cast<char*>("I"))).isBool()) {
+#else
       obj3.free();
       if (obj1.dictLookup(const_cast<char*>("I"), &obj3)->isBool()) {
+#endif
 	isolated = obj3.getBool();
       }
+#if defined(POPPLER_NEW_OBJECT_API)
+      if ((obj3 = obj1.dictLookup(const_cast<char*>("K"))).isBool()) {
+#else
       obj3.free();
       if (obj1.dictLookup(const_cast<char*>("K"), &obj3)->isBool()) {
+#endif
 	knockout = obj3.getBool();
       }
+#if defined(POPPLER_NEW_OBJECT_API)
+    }
+  }
+#else
       obj3.free();
     }
     obj2.free();
   }
   obj1.free();
+#endif
 
   // draw it
   ++formDepth;
@@ -3038,7 +3531,9 @@ void PdfParser::doForm(Object *str) {
   if (blendingColorSpace) {
     delete blendingColorSpace;
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   resObj.free();
+#endif
 }
 
 void PdfParser::doForm1(Object *str, Dict *resDict, double *matrix, double *bbox,
@@ -3166,35 +3661,61 @@ Stream *PdfParser::buildImageStream() {
   Stream *str;
 
   // build dictionary
+#if defined(POPPLER_NEW_OBJECT_API)
+  dict = Object(new Dict(xref));
+  obj = parser->getObj();
+#else
   dict.initDict(xref);
   parser->getObj(&obj);
+#endif
   while (!obj.isCmd(const_cast<char*>("ID")) && !obj.isEOF()) {
     if (!obj.isName()) {
       error(errSyntaxError, getPos(), "Inline image dictionary key must be a name object");
+#if !defined(POPPLER_NEW_OBJECT_API)
       obj.free();
+#endif
     } else {
       key = copyString(obj.getName());
+#if defined(POPPLER_NEW_OBJECT_API)
+      obj = parser->getObj();
+#else
       obj.free();
       parser->getObj(&obj);
+#endif
       if (obj.isEOF() || obj.isError()) {
 	gfree(key);
 	break;
       }
+#if defined(POPPLER_NEW_OBJECT_API)
+      dict.dictAdd(key, std::move(obj));
+    }
+    obj = parser->getObj();
+#else
       dict.dictAdd(key, &obj);
     }
     parser->getObj(&obj);
+#endif
   }
   if (obj.isEOF()) {
     error(errSyntaxError, getPos(), "End of file in inline image");
+#if !defined(POPPLER_NEW_OBJECT_API)
     obj.free();
     dict.free();
+#endif
     return NULL;
   }
+#if !defined(POPPLER_NEW_OBJECT_API)
   obj.free();
+#endif
 
   // make stream
+#if defined(POPPLER_NEW_OBJECT_API)
+  str = new EmbedStream(parser->getStream(), dict.copy(), gFalse, 0);
+  str = str->addFilters(dict.getDict());
+#else
   str = new EmbedStream(parser->getStream(), &dict, gFalse, 0);
   str = str->addFilters(&dict);
+#endif
 
   return str;
 }
