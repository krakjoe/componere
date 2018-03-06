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

#include <php.h>
#include <zend_interfaces.h>

#include "src/common.h"

static inline zend_object* php_componere_cast_realloc(zend_object *clone, zend_class_entry *source, zend_class_entry *target) {
	if (source->default_properties_count) {
		zval *slot = clone->properties_table,
                     *end = slot + source->default_properties_count;

		do {
			if (Z_REFCOUNTED_P(slot)) {
				zval_ptr_dtor(slot);
			}
			slot++;
		} while (slot != end);
	}

	clone = (zend_object*) erealloc(clone, sizeof(zend_object) + zend_object_properties_size(target));

	if (target->default_properties_count) {
		zval *src = target->default_properties_table;
		zval *dst = clone->properties_table;
		zval *end = src + target->default_properties_count;

		do {
			ZVAL_COPY(dst, src);
			src++;
			dst++;
		} while (src != end);
	}

	if (clone->properties) {
		if (!(GC_FLAGS(clone->properties) & IS_ARRAY_IMMUTABLE)) {
#if PHP_VERSION_ID >= 70300
			if (GC_DELREF(clone->properties) == 0) {
#else
			if (--GC_REFCOUNT(clone->properties) == 0) {

#endif
				zend_array_destroy(clone->properties);
			}
		}

		clone->properties = NULL;
	}

	clone->ce = target;

	return clone;
}

zval* php_componere_cast(zval *return_value, zval *instance, zend_class_entry *target) {
	zend_class_entry *source = Z_OBJCE_P(instance);
	zend_object *clone;

	if (Z_OBJ_HT_P(instance) != zend_get_std_object_handlers() || target->type == ZEND_INTERNAL_CLASS) {
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

	ZVAL_OBJ(return_value, 
		php_componere_cast_realloc(
			Z_OBJ_HT_P(instance)->clone_obj(instance), source, target));
	{
		zend_string *k;

		ZEND_HASH_FOREACH_STR_KEY(Z_OBJ_HT_P(instance)->get_properties(instance), k) {
			zval key;

			ZVAL_STR(&key, k);

			if (Z_OBJ_HT_P(return_value)->has_property(return_value, &key, 2, NULL)) {
				zval v;
				zval *val = Z_OBJ_HT_P(instance)
						->read_property(instance, &key, BP_VAR_R, NULL, &v);
				
				Z_OBJ_HT_P(return_value)->write_property(return_value, &key, val, NULL);
			}
		} ZEND_HASH_FOREACH_END();
	}

	return return_value;
}
#endif
