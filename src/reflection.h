/*
  +----------------------------------------------------------------------+
  | componere                                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) Joe Watkins 2018-2020                                  |
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
#ifndef HAVE_COMPONERE_REFLECTION_H
#define HAVE_COMPONERE_REFLECTION_H

extern PHP_RINIT_FUNCTION(Componere_Reflection);

extern zend_class_entry* php_componere_reflection_class_ce;
extern zend_class_entry* php_componere_reflection_method_ce;

typedef enum {
	PHP_REF_TYPE_OTHER,
	PHP_REF_TYPE_FUNCTION,
	PHP_REF_TYPE_GENERATOR,
	PHP_REF_TYPE_PARAMETER,
	PHP_REF_TYPE_TYPE,
	PHP_REF_TYPE_PROPERTY,
	PHP_REF_TYPE_DYNAMIC_PROPERTY,
	PHP_REF_TYPE_CLASS_CONSTANT
} php_reflection_type_t;

void php_componere_reflection_object_factory(
	zval *return_value, 
	zend_class_entry *ce, 
	php_reflection_type_t type, 
	void *ptr,
	zend_string *named);
#endif
