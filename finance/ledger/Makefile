# $NetBSD: Makefile,v 1.30 2023/12/29 18:24:44 adam Exp $

DISTNAME=	ledger-3.3.2
PKGREVISION=	2
CATEGORIES=	finance
MASTER_SITES=	${MASTER_SITE_GITHUB:=ledger/}
GITHUB_TAG=	v${PKGVERSION_NOREV}

MAINTAINER=	base@atomicules.co.uk
HOMEPAGE=	https://ledger-cli.org/
COMMENT=	Double-entry accounting system for command-line
LICENSE=	modified-bsd

USE_CMAKE=	yes
USE_LANGUAGES=	c++ c

USE_CXX_FEATURES+=	c++11

.include "options.mk"

REPLACE_INTERPRETER+=	python
REPLACE.python.old=	/usr/bin/env python
REPLACE.python.new=	${PYTHONBIN}
REPLACE_FILES.python+=	contrib/getquote-uk.py acprep python/demo.py
REPLACE_FILES.python+=	test/DocTests.py test/LedgerHarness.py
REPLACE_FILES.python+=	test/convert.py test/RegressTests.py test/convert.py
REPLACE_FILES.python+=	tools/genuuid

TEST_TARGET=	check
TEST_ENV+=	LD_LIBRARY_PATH=${WRKSRC}

.include "../../devel/boost-headers/buildlink3.mk"
.include "../../devel/boost-libs/buildlink3.mk"
.include "../../devel/editline/buildlink3.mk"
.include "../../devel/gmp/buildlink3.mk"
.include "../../lang/python/tool.mk"
.include "../../math/mpfr/buildlink3.mk"
.include "../../textproc/utf8-cpp/buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
