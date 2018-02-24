/*
  +----------------------------------------------------------------------+
  | componere                                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) Joe Watkins 2018                                       |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe <krakjoe@php.net>                                    |
  +----------------------------------------------------------------------+
 */
#ifndef HAVE_COMPONERE_VALUE_H
#define HAVE_COMPONERE_VALUE_H

typedef struct _php_componere_value_t {
	zval value;
	zend_long access;
	zend_object std;
} php_componere_value_t;

#define php_componere_value_from(o) \
	((php_componere_value_t*) \
		((char*) o - XtOffsetOf(php_componere_value_t, std)))
#define php_componere_value_fetch(z) php_componere_value_from(Z_OBJ_P(z))
#define php_componere_value_default(z) (&php_componere_value_fetch(z)->value)
#define php_componere_value_access(z) (php_componere_value_fetch(z)->access)

extern PHP_MINIT_FUNCTION(Componere_Value);

extern zend_class_entry *php_componere_value_ce;

#endif
