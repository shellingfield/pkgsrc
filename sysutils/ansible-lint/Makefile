# $NetBSD: Makefile,v 1.50 2024/02/09 14:47:34 adam Exp $

DISTNAME=	ansible-lint-24.2.0
CATEGORIES=	sysutils python
MASTER_SITES=	${MASTER_SITE_PYPI:=a/ansible-lint/}

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	https://ansible-lint.readthedocs.io/
COMMENT=	Checks playbooks for practices and behaviour that could be improved
LICENSE=	gnu-gpl-v3 AND mit

TOOL_DEPENDS+=	${PYPKGPREFIX}-setuptools>=65.3.0:../../devel/py-setuptools
TOOL_DEPENDS+=	${PYPKGPREFIX}-setuptools_scm>=7.0.5:../../devel/py-setuptools_scm
TOOL_DEPENDS+=	${PYPKGPREFIX}-wheel-[0-9]*:../../devel/py-wheel
DEPENDS+=	ansible-core>=2.12.0:../../sysutils/ansible-core
DEPENDS+=	${PYPKGPREFIX}-ansible-compat>=4.1.11:../../sysutils/py-ansible-compat
DEPENDS+=	${PYPKGPREFIX}-black>=22.8.0:../../textproc/py-black
DEPENDS+=	${PYPKGPREFIX}-filelock>=3.3.0:../../devel/py-filelock
DEPENDS+=	${PYPKGPREFIX}-jsonschema>=4.10.0:../../textproc/py-jsonschema
DEPENDS+=	${PYPKGPREFIX}-packaging>=21.3:../../devel/py-packaging
DEPENDS+=	${PYPKGPREFIX}-pathspec>=0.10.3:../../devel/py-pathspec
DEPENDS+=	${PYPKGPREFIX}-requests>=2.31.0:../../devel/py-requests
DEPENDS+=	${PYPKGPREFIX}-rich>=12.0.0:../../comms/py-rich
DEPENDS+=	${PYPKGPREFIX}-ruamel-yaml>=0.18.5:../../devel/py-ruamel-yaml
DEPENDS+=	${PYPKGPREFIX}-subprocess-tee>=0.4.1:../../sysutils/py-subprocess-tee
DEPENDS+=	${PYPKGPREFIX}-wcmatch>=8.1.2:../../textproc/py-wcmatch
DEPENDS+=	${PYPKGPREFIX}-yaml>=5.4.1:../../textproc/py-yaml
DEPENDS+=	${PYPKGPREFIX}-yamllint>=1.30.0:../../textproc/py-yamllint
TEST_DEPENDS+=	${PYPKGPREFIX}-jmespath-[0-9]*:../../textproc/py-jmespath
TEST_DEPENDS+=	${PYPKGPREFIX}-mypy-[0-9]*:../../lang/py-mypy
TEST_DEPENDS+=	${PYPKGPREFIX}-netaddr-[0-9]*:../../net/py-netaddr
TEST_DEPENDS+=	${PYPKGPREFIX}-psutil-[0-9]*:../../sysutils/py-psutil
TEST_DEPENDS+=	${PYPKGPREFIX}-test-mock-[0-9]*:../../devel/py-test-mock
TEST_DEPENDS+=	${PYPKGPREFIX}-test-xdist>=2.1.0:../../devel/py-test-xdist

USE_LANGUAGES=	# none

PYTHON_VERSIONS_INCOMPATIBLE=	27 38 39

.include "../../lang/python/wheel.mk"
.include "../../mk/bsd.pkg.mk"
