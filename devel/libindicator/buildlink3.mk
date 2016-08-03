# $NetBSD: buildlink3.mk,v 1.2 2016/08/03 10:22:11 adam Exp $

BUILDLINK_TREE+=	libindicator

.if !defined(LIBINDICATOR_BUILDLINK3_MK)
LIBINDICATOR_BUILDLINK3_MK:=

BUILDLINK_API_DEPENDS.libindicator+=	libindicator>=12.10.1
BUILDLINK_ABI_DEPENDS.libindicator?=	libindicator>=12.10.1nb1
BUILDLINK_PKGSRCDIR.libindicator?=	../../devel/libindicator

.include "../../x11/gtk2/buildlink3.mk"
.endif	# LIBINDICATOR_BUILDLINK3_MK

BUILDLINK_TREE+=	-libindicator
