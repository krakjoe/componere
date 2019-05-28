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
#ifndef HAVE_COMPONERE_COMMON_H
#define HAVE_COMPONERE_COMMON_H

#ifndef HAVE_COMPONERE_COMMON_HEADERS_H
#define HAVE_COMPONERE_COMMON_HEADERS_H
#      include <ext/spl/spl_exceptions.h>
#      include <zend_exceptions.h>
#endif

#ifndef GC_ADDREF
# define GC_ADDREF(g) ++GC_REFCOUNT(g)
#endif

#define php_componere_parse_parameters(s, ...) \
	zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), s, ##__VA_ARGS__)
#define php_componere_throw_ex(e, s, ...) \
	zend_throw_exception_ex(spl_ce_##e, 0, s, ##__VA_ARGS__)
#define php_componere_throw(s, ...) \
	php_componere_throw_ex(RuntimeException, s, ##__VA_ARGS__)
#define php_componere_wrong_parameters(s, ...) \
	php_componere_throw_ex(InvalidArgumentException, s, ##__VA_ARGS__);
#define php_componere_no_parameters() do { \
	if (php_componere_parse_parameters("") != SUCCESS) { \
		php_componere_wrong_parameters("no parameters expected"); \
		return; \
	} \
} while(0)

static inline void php_componere_destroy_class(zend_class_entry *ce) {
	zval tmp;

	ZVAL_PTR(&tmp, ce);

	destroy_zend_class(&tmp);
}

#define php_componere_deny_throw(o, r) do { \
	php_componere_throw("objects of type %s do not support " #r, ZSTR_VAL((o)->ce->name)); \
} while(0)

#if PHP_VERSION_ID >= 80000
static inline zend_object* php_componere_deny_clone(zend_object *object) {
#else
static inline zend_object* php_componere_deny_clone(zval *zo) {
    zend_object *object = Z_OBJ_P(zo);
#endif
	php_componere_deny_throw(object, cloning);
	GC_ADDREF(object);
	return object;
}

#if PHP_VERSION_ID >= 80000
static inline void php_componere_deny_write(zend_object *object, zend_string *member) {
#else
static inline void php_componere_deny_write(zval *zo, zval *member) {
    zend_object *object = Z_OBJ_P(zo);
#endif
	php_componere_deny_throw(object, properties);
}

#if PHP_VERSION_ID >= 80000
static inline zval* php_componere_deny_ptr(zend_object *object) {
#else
static inline zval* php_componere_deny_ptr(zval *zo) {
    zend_object *object = Z_OBJ_P(zo);
#endif
	php_componere_deny_throw(object, properties);
	return NULL;
}

#if PHP_VERSION_ID >= 80000
static inline zval* php_componere_deny_read(zend_object *object, zend_string *member) {
#else
static inline zval* php_componere_deny_read(zval *zo, zval *member) {
    zend_object *object = Z_OBJ_P(zo);
#endif
	php_componere_deny_throw(object, properties);
	return &EG(uninitialized_zval);
}

#if PHP_VERSION_ID >= 80000
static inline int php_componere_deny_isset(zend_object *object, zend_string *member, int has_set_exists) {
#else
static inline int php_componere_deny_isset(zval *zo, zval *member, int has_set_exists) {
    zend_object *object = Z_OBJ_P(zo);
#endif
	if (has_set_exists != 2) {
		php_componere_deny_throw(object, properties);
	}
	return 0;
}

#if PHP_VESRION_ID >= 80000
static inline void php_componere_deny_unset(zend_object *object, zend_string *member) {
#else
static inline void php_componere_deny_unset(zval *zo, zval *member) {
    zend_object *object = Z_OBJ_P(zo);
#endif
	php_componere_deny_throw(object, properties);
}

#if PHP_VERSION_ID >= 80000
static inline HashTable* php_componere_deny_debug(zend_object *object, int *temp) {
#else
static inline HashTable* php_componere_deny_debug(zval *zo, int *temp) {
    zend_object *object = Z_OBJ_P(zo);
#endif
	HashTable *table;

	ALLOC_HASHTABLE(table);

	zend_hash_init(table, 8, ZVAL_PTR_DTOR, NULL, 0);

	*temp = 1;

	return table;
}

#if PHP_VERSION_ID >= 80000
static inline HashTable* php_componere_deny_collect(zend_object *object, zval **table, int *num) {
#else
static inline HashTable* php_componere_deny_collect(zval *object, zval **table, int *num) {
#endif
	*table = NULL;
	*num   = 0;

	return NULL;
}

#undef php_componere_deny_throw

static inline void php_componere_setup_handlers(
		zend_object_handlers *handlers, 
		zend_object_get_debug_info_t debug_obj, 
		zend_object_get_gc_t collect_obj,
		zend_object_clone_obj_t clone_obj, 
		zend_object_free_obj_t free_obj, 
		int offset) {
	memcpy(handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	handlers->read_property = (zend_object_read_property_t) php_componere_deny_read;
	handlers->write_property = (zend_object_write_property_t) php_componere_deny_write;
	handlers->read_dimension = (zend_object_read_dimension_t) php_componere_deny_read;
	handlers->write_dimension = (zend_object_write_dimension_t) php_componere_deny_write;
	handlers->get_property_ptr_ptr = (zend_object_get_property_ptr_ptr_t) php_componere_deny_ptr;
	handlers->has_property = (zend_object_has_property_t) php_componere_deny_isset;
	handlers->unset_property = (zend_object_unset_property_t) php_componere_deny_unset;

	handlers->free_obj = free_obj;
	handlers->get_debug_info = debug_obj;
	handlers->get_gc = collect_obj;
	handlers->clone_obj = clone_obj;
	handlers->offset = offset;
}

ZEND_BEGIN_ARG_INFO(php_componere_no_arginfo, 0)
ZEND_END_ARG_INFO()
#endif
