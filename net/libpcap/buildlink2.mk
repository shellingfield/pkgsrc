# $NetBSD: buildlink2.mk,v 1.4 2004/02/05 06:58:03 jlam Exp $

.if !defined(LIBPCAP_BUILDLINK2_MK)
LIBPCAP_BUILDLINK2_MK=	# defined

BUILDLINK_DEPENDS.libpcap?=	libpcap>=0.7.2
BUILDLINK_PKGSRCDIR.libpcap?=	../../net/libpcap

.if exists(/usr/include/pcap.h)
_NEED_LIBPCAP=		NO
.else
_NEED_LIBPCAP=		YES
.endif

.if defined(BUILDLINK_PREFER_PKGSRC)
.  if empty(BUILDLINK_PREFER_PKGSRC) || \
      !empty(BUILDLINK_PREFER_PKGSRC:M[yY][eE][sS]) || \
      !empty(BUILDLINK_PREFER_PKGSRC:Mlibpcap)
_NEED_LIBPCAP=	YES
.  endif
.endif

.if ${_NEED_LIBPCAP} == "YES"
BUILDLINK_PACKAGES+=	libpcap
EVAL_PREFIX+=		BUILDLINK_PREFIX.libpcap=libpcap
BUILDLINK_PREFIX.libpcap_DEFAULT=	${LOCALBASE}
.else
BUILDLINK_PREFIX.libpcap=	/usr
.endif

BUILDLINK_FILES.libpcap=	include/pcap.h
BUILDLINK_FILES.libpcap+=	include/pcap-namedb.h
BUILDLINK_FILES.libpcap+=	include/net/bpf.h
BUILDLINK_FILES.libpcap+=	lib/libpcap.*

BUILDLINK_TARGETS+=	libpcap-buildlink

libpcap-buildlink: _BUILDLINK_USE

.endif	# LIBPCAP_BUILDLINK2_MK
