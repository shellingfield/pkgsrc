# $NetBSD: buildlink3.mk,v 1.1 2004/06/05 22:19:53 rh Exp $

BUILDLINK_DEPTH:=	${BUILDLINK_DEPTH}+
MPFR_BUILDLINK3_MK:=	${MPFR_BUILDLINK3_MK}+

.if !empty(BUILDLINK_DEPTH:M+)
BUILDLINK_DEPENDS+=	mpfr
.endif

BUILDLINK_PACKAGES:=	${BUILDLINK_PACKAGES:Nmpfr}
BUILDLINK_PACKAGES+=	mpfr

.if !empty(MPFR_BUILDLINK3_MK:M+)
BUILDLINK_DEPENDS.mpfr+=	mpfr>=2.0.3
BUILDLINK_DEPMETHOD.mpfr?=	build
BUILDLINK_PKGSRCDIR.mpfr?=	../../math/mpfr
.endif	# MPFR_BUILDLINK3_MK

.include "../../devel/gmp/buildlink3.mk"

BUILDLINK_DEPTH:=     ${BUILDLINK_DEPTH:S/+$//}
