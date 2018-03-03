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
#ifndef HAVE_COMPONERE_REFLECTION
#define HAVE_COMPONERE_REFLECTION

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <php.h>

#include "src/reflection.h"

zend_class_entry* php_componere_reflection_class_ce;
zend_class_entry* php_componere_reflection_method_ce;

typedef struct {
	zval dummy;
	zval obj;
	void *ptr;
	zend_class_entry *ce;
	php_reflection_type_t ref_type;
	unsigned int ignore_visibility:1;
	zend_object zo;
} php_reflection_object_t;

#define php_reflection_object_from(o) \
	(php_reflection_object_t*) \
		(((char*) o) - XtOffsetOf(php_reflection_object_t, zo))
#define php_reflection_object_fetch(z) php_reflection_object_from(Z_OBJ_P(z))

void php_componere_reflection_object_factory(
		zval *return_value, 
		zend_class_entry *ce, 
		php_reflection_type_t type, 
		void *ptr,
		zend_string *named) {
	php_reflection_object_t *ro;
#if PHP_VERSION_ID < 70300
	zend_string *name = zend_string_init(ZEND_STRL("name"), 0);
#else
	zend_string *name = ZSTR_KNOWN(ZEND_STR_NAME);
#endif
	zval key, value;

	object_init_ex(return_value, ce);

	ro = php_reflection_object_fetch(return_value);
	ro->ptr = ptr;
	ro->ref_type = type;

	ZVAL_STR(&key, name);
	ZVAL_STR(&value, named);

	zend_std_write_property(return_value, &key, &value, NULL);

#if PHP_VERSION_ID < 70300
	zend_string_release(name);
#endif
}

static inline zend_class_entry* php_componere_reflection_class(char *chars, size_t len) {
	zend_string *name = zend_string_init(chars, len, 0);
	zend_class_entry *ce;

	if (!(ce = zend_lookup_class(name))) {
		/* do something, this is bad */
	}

	zend_string_release(name);
	
	return ce;
}

PHP_RINIT_FUNCTION(Componere_Reflection) {
	php_componere_reflection_class_ce = 
		php_componere_reflection_class(ZEND_STRL("ReflectionClass"));
	php_componere_reflection_method_ce =
		php_componere_reflection_class(ZEND_STRL("ReflectionMethod"));

	return SUCCESS;
}
#endif
