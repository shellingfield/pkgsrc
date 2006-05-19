# $NetBSD: buildlink3.mk,v 1.12 2006/05/19 16:14:14 rillig Exp $

BUILDLINK_DEPTH:=	${BUILDLINK_DEPTH}+
PY_OGG_BUILDLINK3_MK:=	${PY_OGG_BUILDLINK3_MK}+

.if !empty(BUILDLINK_DEPTH:M+)
BUILDLINK_DEPENDS+=	pyogg
.endif

BUILDLINK_PACKAGES:=	${BUILDLINK_PACKAGES:Npyogg}
BUILDLINK_PACKAGES+=	pyogg

.if !empty(PY_OGG_BUILDLINK3_MK:M+)
.include "../../lang/python/pyversion.mk"
BUILDLINK_API_DEPENDS.pyogg+=	${PYPKGPREFIX}-ogg>=1.3
BUILDLINK_ABI_DEPENDS.pyogg?=	${PYPKGPREFIX}-ogg>=1.3nb2
BUILDLINK_PKGSRCDIR.pyogg?=	../../audio/py-ogg
.endif	# PY_OGG_BUILDLINK3_MK

.include "../../multimedia/libogg/buildlink3.mk"

BUILDLINK_DEPTH:=	${BUILDLINK_DEPTH:S/+$//}
