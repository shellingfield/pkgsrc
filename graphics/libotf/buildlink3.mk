# $NetBSD: buildlink3.mk,v 1.1.1.1 2008/02/26 11:47:35 obache Exp $

BUILDLINK_DEPTH:=	${BUILDLINK_DEPTH}+
LIBOTF_BUILDLINK3_MK:=	${LIBOTF_BUILDLINK3_MK}+

.if ${BUILDLINK_DEPTH} == "+"
BUILDLINK_DEPENDS+=	libotf
.endif

BUILDLINK_PACKAGES:=	${BUILDLINK_PACKAGES:Nlibotf}
BUILDLINK_PACKAGES+=	libotf
BUILDLINK_ORDER:=	${BUILDLINK_ORDER} ${BUILDLINK_DEPTH}libotf

.if ${LIBOTF_BUILDLINK3_MK} == "+"
BUILDLINK_API_DEPENDS.libotf+=	libotf>=0.9.7
BUILDLINK_PKGSRCDIR.libotf?=	../../graphics/libotf
.endif	# LIBOTF_BUILDLINK3_MK

.include "../../graphics/freetype2/buildlink3.mk"

BUILDLINK_DEPTH:=	${BUILDLINK_DEPTH:S/+$//}
