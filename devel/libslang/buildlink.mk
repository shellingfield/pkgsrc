# $NetBSD: buildlink.mk,v 1.1 2001/06/26 14:26:03 zuntum Exp $
#
# This Makefile fragment is included by packages that use libslang.
#
# To use this Makefile fragment, simply:
#
# (1) Optionally define BUILDLINK_DEPENDS.libslang to the dependency pattern
#     for the version of libslang desired.
# (2) Include this Makefile fragment in the package Makefile,
# (3) Add ${BUILDLINK_DIR}/include to the front of the C preprocessor's header
#     search path, and
# (4) Add ${BUILDLINK_DIR}/lib to the front of the linker's library search
#     path.

.if !defined(LIBSLANG_BUILDLINK_MK)
LIBSLANG_BUILDLINK_MK=	# defined

BUILDLINK_DEPENDS.libslang?=	libslang>=1.4.4
DEPENDS+=	${BUILDLINK_DEPENDS.libslang}:../../devel/libslang

BUILDLINK_PREFIX.libslang=	${LOCALBASE}
BUILDLINK_FILES.libslang=	include/slcurses.h
BUILDLINK_FILES.libslang+=	include/slang.h
BUILDLINK_FILES.libslang+=	lib/libslang.*

BUILDLINK_TARGETS.libslang=	libslang-buildlink
BUILDLINK_TARGETS+=	${BUILDLINK_TARGETS.libslang}

pre-configure: ${BUILDLINK_TARGETS.libslang}
libslang-buildlink: _BUILDLINK_USE

.include "../../mk/bsd.buildlink.mk"

.endif	# LIBSLANG_BUILDLINK_MK
