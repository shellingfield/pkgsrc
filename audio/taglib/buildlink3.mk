# $NetBSD: buildlink3.mk,v 1.6 2004/10/06 00:09:02 chris Exp $

BUILDLINK_DEPTH:=	${BUILDLINK_DEPTH}+
TAGLIB_BUILDLINK3_MK:=	${TAGLIB_BUILDLINK3_MK}+

.if !empty(BUILDLINK_DEPTH:M+)
BUILDLINK_DEPENDS+=	taglib
.endif

BUILDLINK_PACKAGES:=	${BUILDLINK_PACKAGES:Ntaglib}
BUILDLINK_PACKAGES+=	taglib

.if !empty(TAGLIB_BUILDLINK3_MK:M+)
BUILDLINK_DEPENDS.taglib+=	taglib>=1.0
BUILDLINK_RECOMMENDED.taglib+=	taglib>=1.3
BUILDLINK_PKGSRCDIR.taglib?=	../../audio/taglib
.endif	# TAGLIB_BUILDLINK3_MK

BUILDLINK_DEPTH:=     ${BUILDLINK_DEPTH:S/+$//}
