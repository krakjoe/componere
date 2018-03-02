dnl $Id$
dnl config.m4 for extension componere

PHP_ARG_ENABLE(componere, whether to enable componere support,
[  --enable-componere          Enable componere support], no)

if test "$PHP_COMPONERE" != "no"; then
  AC_DEFINE(HAVE_COMPONERE, 1, [ Have componere support ])

  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_INCLUDE($ext_builddir)

  PHP_NEW_EXTENSION(componere, componere.c src/definition.c src/patch.c src/method.c src/reflection.c src/value.c, $ext_shared)
fi
