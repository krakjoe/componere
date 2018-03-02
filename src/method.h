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
#ifndef HAVE_COMPONERE_METHOD_H
#define HAVE_COMPONERE_METHOD_H

typedef struct _php_componere_method_t {
	zend_function *function;
	zval reflector;
	zend_object std;
} php_componere_method_t;

#define php_componere_method_from(o) \
	((php_componere_method_t*) \
		((char*) o - XtOffsetOf(php_componere_method_t, std)))
#define php_componere_method_fetch(z) php_componere_method_from(Z_OBJ_P(z))
#define php_componere_method_function(z) (php_componere_method_fetch(z)->function)

extern PHP_MINIT_FUNCTION(Componere_Method);
extern PHP_RINIT_FUNCTION(Componere_Method);

extern zend_class_entry *php_componere_method_ce;

#endif
