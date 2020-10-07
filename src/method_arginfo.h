/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: bec40f676dddd349354623c052007d03fd5bd97b */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Method___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, closure, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Method_setProtected, 0, 0, Componere\\Method, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Method_setPrivate arginfo_class_Method_setProtected

#define arginfo_class_Method_setStatic arginfo_class_Method_setProtected

#define arginfo_class_Method_setFinal arginfo_class_Method_setProtected

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Method_getReflector, 0, 0, ReflectionMethod, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(Method, __construct);
ZEND_METHOD(Method, setProtected);
ZEND_METHOD(Method, setPrivate);
ZEND_METHOD(Method, setStatic);
ZEND_METHOD(Method, setFinal);
ZEND_METHOD(Method, getReflector);


static const zend_function_entry class_Method_methods[] = {
	ZEND_ME(Method, __construct, arginfo_class_Method___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Method, setProtected, arginfo_class_Method_setProtected, ZEND_ACC_PUBLIC)
	ZEND_ME(Method, setPrivate, arginfo_class_Method_setPrivate, ZEND_ACC_PUBLIC)
	ZEND_ME(Method, setStatic, arginfo_class_Method_setStatic, ZEND_ACC_PUBLIC)
	ZEND_ME(Method, setFinal, arginfo_class_Method_setFinal, ZEND_ACC_PUBLIC)
	ZEND_ME(Method, getReflector, arginfo_class_Method_getReflector, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
