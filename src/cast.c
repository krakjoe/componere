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
#ifndef HAVE_COMPONERE_CAST
#define HAVE_COMPONERE_CAST

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <php.h>
#include <zend_interfaces.h>

#include "src/common.h"

zval* php_componere_cast(zval *return_value, zval *instance, zend_class_entry *target) {
	zend_class_entry *source = Z_OBJCE_P(instance);
	zend_object *co, *zo;

	if (Z_OBJCE_P(instance)->create_object || target->create_object) {
		php_componere_throw_ex(InvalidArgumentException,
			"cannot cast between internal types");
		return NULL;
	}

	if (target->ce_flags & ZEND_ACC_INTERFACE) {
		php_componere_throw_ex(InvalidArgumentException,
			"cannot cast to interface %s", 
			ZSTR_VAL(target->name));
		return NULL;
	}

	if (target->ce_flags & ZEND_ACC_TRAIT) {
		php_componere_throw_ex(InvalidArgumentException,
			"cannot cast to trait %s", 
			ZSTR_VAL(target->name));
		return NULL;
	}

	if (target->ce_flags & ZEND_ACC_EXPLICIT_ABSTRACT_CLASS) {
		php_componere_throw_ex(InvalidArgumentException,
			"cannot cast to abstract %s", 
			ZSTR_VAL(target->name));
		return NULL;
	}

	if (!instanceof_function(target, source) && !instanceof_function(source, target)) {
		php_componere_throw_ex(InvalidArgumentException, 
			"%s is not compatible with %s",
			ZSTR_VAL(target->name),
			ZSTR_VAL(source->name));
		return NULL;
	}

	zo = Z_OBJ_P(instance);
	co = zend_objects_new(target);

	if (co->ce->default_properties_count) {
		int slot = 0, end = co->ce->default_properties_count;

		do {
			if (slot < zo->ce->default_properties_count) {
				ZVAL_COPY(
					&co->properties_table[slot], 
					&zo->properties_table[slot]);
			} else {
				ZVAL_COPY(&co->properties_table[slot],
					  &co->ce->default_properties_table[slot]);
			}
			slot++;
		} while (slot < end);
	}

	ZVAL_OBJ(return_value, co);

	return return_value;
}
#endif
