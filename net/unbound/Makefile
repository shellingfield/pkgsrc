# $NetBSD: Makefile,v 1.105 2024/02/13 13:53:26 he Exp $

DISTNAME=	unbound-1.19.1
CATEGORIES=	net
MASTER_SITES=	https://nlnetlabs.nl/downloads/unbound/

MAINTAINER=	pettai@NetBSD.org
HOMEPAGE=	https://nlnetlabs.nl/projects/unbound/
COMMENT=	DNS resolver and recursive server
LICENSE=	modified-bsd

BUILD_DEFS+=	VARBASE UNBOUND_USER UNBOUND_GROUP
FILES_SUBST+=	UNBOUND_USER=${UNBOUND_USER} UNBOUND_GROUP=${UNBOUND_GROUP}

USE_LIBTOOL=		yes
CONFIGURE_ARGS+=	--enable-allsymbols
CONFIGURE_ARGS+=	--with-libexpat=${BUILDLINK_PREFIX.expat}
CONFIGURE_ARGS+=	--with-libevent=${BUILDLINK_PREFIX.libevent}
CONFIGURE_ARGS+=	--enable-event-api
CONFIGURE_ARGS+=	--with-ssl=${BUILDLINK_PREFIX.openssl}
CONFIGURE_ARGS+=	--with-pidfile=${VARBASE}/run/unbound/unbound.pid
CONFIGURE_ARGS+=	--sysconfdir=${PKG_SYSCONFBASE}
GNU_CONFIGURE=		yes
TEST_TARGET=		test

.include "options.mk"

# unbound uses some OpenBSD libc functions such as reallocarray(3).
# The existing tests just look for the symbol in libc regardless
# of anything in stdlib.h
CPPFLAGS.NetBSD+=		-D_OPENBSD_SOURCE

# Add the same logic as for ldns, so sha2/gost is configured automatically
CHECK_BUILTIN.openssl=	yes
.include "../../security/openssl/builtin.mk"
CHECK_BUILTIN.openssl=	no
.include "../../security/openssl/buildlink3.mk"

PLIST_VARS+=		sha2 gost
.if defined(USE_BUILTIN.openssl) && ${USE_BUILTIN.openssl:tl} == yes
PLIST_VARS.gost!=	\
	if ${PKG_ADMIN} pmatch 'openssl>=1.0.0' ${BUILTIN_PKG.openssl}; then \
		${ECHO} "yes";                                          \
	else                                                            \
		${ECHO} "no";                                           \
	fi
PLIST_VARS.sha2!=	\
	if ${PKG_ADMIN} pmatch 'openssl>=0.9.8' ${BUILTIN_PKG.openssl}; then \
		${ECHO} "yes";                                          \
	else                                                            \
		${ECHO} "no";                                           \
	fi
.else
PLIST_VARS.gost!=	\
	if ${PKG_INFO} -qe 'openssl>=1.0.0'; then                       \
		${ECHO} yes;                                            \
	else                                                            \
		${ECHO} no;                                             \
	fi
PLIST_VARS.sha2!=	\
	if ${PKG_INFO} -qe 'openssl>=0.9.8'; then                       \
		${ECHO} yes;                                            \
	else                                                            \
		${ECHO} no;                                             \
	fi
.endif
.if ${PLIST_VARS.gost} == "yes"
CONFIGURE_ARGS+=	--enable-gost
.else
CONFIGURE_ARGS+=	--disable-gost
.endif
.if ${PLIST_VARS.sha2} == "yes"
CONFIGURE_ARGS+=	--enable-sha2
.else
CONFIGURE_ARGS+=	--disable-sha2
.endif

INSTALL_MAKE_FLAGS+=	\
	configfile=${PREFIX}/share/examples/unbound/unbound.conf

PKG_SYSCONFSUBDIR=	unbound

CONF_FILES+=	share/examples/unbound/unbound.conf \
		${PKG_SYSCONFDIR}/unbound.conf

RCD_SCRIPTS=	unbound
SMF_METHODS=	unbound
SMF_NAME=	unbound

UNBOUND_USER?=	unbound
UNBOUND_GROUP?=	unbound

PKG_GROUPS=	${UNBOUND_GROUP}
PKG_USERS=	${UNBOUND_USER}:${UNBOUND_GROUP}

.include "../../devel/libevent/buildlink3.mk"
.include "../../textproc/expat/buildlink3.mk"
BUILDLINK_API_DEPENDS.flex+=	flex>=2.6.4
.include "../../devel/flex/buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
