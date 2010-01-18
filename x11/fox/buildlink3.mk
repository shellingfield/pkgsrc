# $NetBSD: buildlink3.mk,v 1.19 2010/01/18 09:59:33 wiz Exp $

BUILDLINK_TREE+=	fox

.if !defined(FOX_BUILDLINK3_MK)
FOX_BUILDLINK3_MK:=

BUILDLINK_API_DEPENDS.fox+=	fox>=1.6.25
BUILDLINK_ABI_DEPENDS.fox?=	fox>=1.6.33nb2
BUILDLINK_PKGSRCDIR.fox?=	../../x11/fox
BUILDLINK_INCDIRS.fox?=		include/fox-1.6

.include "../../archivers/bzip2/buildlink3.mk"
.include "../../graphics/glu/buildlink3.mk"
.include "../../graphics/jpeg/buildlink3.mk"
.include "../../graphics/png/buildlink3.mk"
.include "../../graphics/tiff/buildlink3.mk"
.include "../../x11/libX11/buildlink3.mk"
.include "../../x11/libXcursor/buildlink3.mk"
.include "../../x11/libXext/buildlink3.mk"
.include "../../x11/libXft/buildlink3.mk"
.include "../../x11/libXrandr/buildlink3.mk"
.include "../../x11/xextproto/buildlink3.mk"
.endif # FOX_BUILDLINK3_MK

BUILDLINK_TREE+=	-fox
