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
#ifndef HAVE_COMPONERE_COMMON_H
#define HAVE_COMPONERE_COMMON_H

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

ZEND_BEGIN_ARG_INFO(php_componere_no_arginfo, 0)
ZEND_END_ARG_INFO()
#endif
