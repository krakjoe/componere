dnl $Id$
dnl config.m4 for extension componere

PHP_ARG_ENABLE(componere, whether to enable componere support,
[  --enable-componere          Enable componere support], no)
PHP_ARG_ENABLE(componere-coverage,      whether to enable componere coverage support,
[  --enable-componere-coverage          Enable componere coverage support], no, no)

if test "$PHP_COMPONERE" != "no"; then
  AC_DEFINE(HAVE_COMPONERE, 1, [ Have componere support ])

  PHP_ADD_BUILD_DIR($ext_builddir/src)
  PHP_ADD_INCLUDE($ext_builddir)

  PHP_NEW_EXTENSION(componere, componere.c src/cast.c src/definition.c src/patch.c src/method.c src/reflection.c src/value.c, $ext_shared)

  AC_MSG_CHECKING([componere coverage])
  if test "$PHP_COMPONERE_COVERAGE" != "no"; then
    AC_MSG_RESULT([enabled])

    PHP_ADD_MAKEFILE_FRAGMENT
  else
    AC_MSG_RESULT([disabled])
  fi
fi
