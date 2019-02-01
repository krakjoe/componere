/*
  +----------------------------------------------------------------------+
  | componere                                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) Joe Watkins 2018-2019                                  |
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

#include <src/common.h>

zval* php_componere_cast(zval *return_value, zval *instance, zend_class_entry *target, zend_bool references) {
	zend_class_entry *source = Z_OBJCE_P(instance);
	zend_object *co, *zo = Z_OBJ_P(instance);

	if (source->create_object || target->create_object) {
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

	co = zend_objects_new(target);

	if (co->ce->default_properties_count) {
		int slot = 0, end = co->ce->default_properties_count;

		do {
			if (slot < zo->ce->default_properties_count) {
				if (references && !Z_ISREF(zo->properties_table[slot])) {
					ZVAL_MAKE_REF(&zo->properties_table[slot]);
				}

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

	if (zo->properties && instanceof_function(target, source)) {
		zend_string *key;
		zval *val;

		ZEND_HASH_FOREACH_STR_KEY_VAL(zo->properties, key, val) {
			zend_property_info *info = 
				zend_hash_find_ptr(
					&co->ce->properties_info, key);

			if (!info || info->flags & ZEND_ACC_STATIC) {
				continue;
			}

			if (Z_TYPE_P(val) == IS_INDIRECT) {
				val = Z_INDIRECT_P(val);
			}

			if (references && !Z_ISREF_P(val)) {
				ZVAL_MAKE_REF(val);
			}

			ZVAL_COPY(OBJ_PROP(co, info->offset), val);
		} ZEND_HASH_FOREACH_END();
	}

	ZVAL_OBJ(return_value, co);

	return return_value;
}
#endif
