# $NetBSD: buildlink3.mk,v 1.13 2010/01/18 09:58:59 wiz Exp $

BUILDLINK_TREE+=	fnlib

.if !defined(FNLIB_BUILDLINK3_MK)
FNLIB_BUILDLINK3_MK:=

BUILDLINK_API_DEPENDS.fnlib+=	fnlib>=0.5nb6
BUILDLINK_ABI_DEPENDS.fnlib+=	fnlib>=0.5nb12
BUILDLINK_PKGSRCDIR.fnlib?=	../../graphics/fnlib

.include "../../graphics/imlib/buildlink3.mk"
.endif # FNLIB_BUILDLINK3_MK

BUILDLINK_TREE+=	-fnlib
