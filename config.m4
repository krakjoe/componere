dnl $Id$
dnl config.m4 for extension componere

PHP_ARG_ENABLE(componere, whether to enable componere support,
[  --enable-componere          Enable componere support], no)

if test "$PHP_COMPONERE" != "no"; then
  AC_DEFINE(HAVE_COMPONERE, 1, [ Have componere support ])

  PHP_NEW_EXTENSION(componere, componere.c, $ext_shared)
fi
