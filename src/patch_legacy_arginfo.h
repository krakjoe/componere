/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7bba7522aa6f3666cde11ddeb0dbcb4401871a0a */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Componere_Patch___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, instance)
	ZEND_ARG_VARIADIC_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Componere_Patch_getClosure, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Componere_Patch_getClosures, 0, 0, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Componere_Patch_apply arginfo_class_Componere_Patch_getClosures

#define arginfo_class_Componere_Patch_revert arginfo_class_Componere_Patch_getClosures

#define arginfo_class_Componere_Patch_isApplied arginfo_class_Componere_Patch_getClosures

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Componere_Patch_derive, 0, 0, 1)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()


ZEND_METHOD(Componere_Patch, __construct);
ZEND_METHOD(Componere_Patch, getClosure);
ZEND_METHOD(Componere_Patch, getClosures);
ZEND_METHOD(Componere_Patch, apply);
ZEND_METHOD(Componere_Patch, revert);
ZEND_METHOD(Componere_Patch, isApplied);
ZEND_METHOD(Componere_Patch, derive);


static const zend_function_entry class_Componere_Patch_methods[] = {
	ZEND_ME(Componere_Patch, __construct, arginfo_class_Componere_Patch___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Patch, getClosure, arginfo_class_Componere_Patch_getClosure, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Patch, getClosures, arginfo_class_Componere_Patch_getClosures, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Patch, apply, arginfo_class_Componere_Patch_apply, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Patch, revert, arginfo_class_Componere_Patch_revert, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Patch, isApplied, arginfo_class_Componere_Patch_isApplied, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Patch, derive, arginfo_class_Componere_Patch_derive, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
