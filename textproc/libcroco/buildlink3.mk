# $NetBSD: buildlink3.mk,v 1.21 2009/02/20 20:02:30 reed Exp $

BUILDLINK_DEPTH:=		${BUILDLINK_DEPTH}+
LIBCROCO_BUILDLINK3_MK:=	${LIBCROCO_BUILDLINK3_MK}+

.if !empty(BUILDLINK_DEPTH:M+)
BUILDLINK_DEPENDS+=	libcroco
.endif

BUILDLINK_PACKAGES:=	${BUILDLINK_PACKAGES:Nlibcroco}
BUILDLINK_PACKAGES+=	libcroco
BUILDLINK_ORDER:=	${BUILDLINK_ORDER} ${BUILDLINK_DEPTH}libcroco

.if !empty(LIBCROCO_BUILDLINK3_MK:M+)
BUILDLINK_API_DEPENDS.libcroco+=	libcroco>=0.6.0
BUILDLINK_ABI_DEPENDS.libcroco+=	libcroco>=0.6.1nb7
BUILDLINK_PKGSRCDIR.libcroco?=	../../textproc/libcroco
.endif	# LIBCROCO_BUILDLINK3_MK

.include "../../devel/glib2/buildlink3.mk"
.include "../../textproc/libxml2/buildlink3.mk"

BUILDLINK_DEPTH:=		${BUILDLINK_DEPTH:S/+$//}
