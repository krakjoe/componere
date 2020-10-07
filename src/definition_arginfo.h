/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5949e70343f7a1555409ac8211f4440948e52b3d */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Componere_Abstract_Definition_addMethod, 0, 2, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, method, Componere\\Method, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Componere_Abstract_Definition_addTrait, 0, 1, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, trait, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Componere_Abstract_Definition_addInterface, 0, 1, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, interface, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Componere_Abstract_Definition_getReflector, 0, 0, ReflectionClass, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Componere_Definition___construct, 0, 0, 0)
	ZEND_ARG_VARIADIC_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Componere_Definition_addProperty, 0, 2, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, value, Componere\\Value, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Componere_Definition_addConstant arginfo_class_Componere_Definition_addProperty

#define arginfo_class_Componere_Definition_setConstant arginfo_class_Componere_Definition_addProperty

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Componere_Definition_getClosure, 0, 1, Closure, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Componere_Definition_getClosures, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Componere_Definition_register, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Componere_Definition_isRegistered, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(Componere_Abstract_Definition, addMethod);
ZEND_METHOD(Componere_Abstract_Definition, addTrait);
ZEND_METHOD(Componere_Abstract_Definition, addInterface);
ZEND_METHOD(Componere_Abstract_Definition, getReflector);
ZEND_METHOD(Componere_Definition, __construct);
ZEND_METHOD(Componere_Definition, addProperty);
ZEND_METHOD(Componere_Definition, addConstant);
ZEND_METHOD(Componere_Definition, setConstant);
ZEND_METHOD(Componere_Definition, getClosure);
ZEND_METHOD(Componere_Definition, getClosures);
ZEND_METHOD(Componere_Definition, register);
ZEND_METHOD(Componere_Definition, isRegistered);


static const zend_function_entry class_Componere_Abstract_Definition_methods[] = {
	ZEND_ME(Componere_Abstract_Definition, addMethod, arginfo_class_Componere_Abstract_Definition_addMethod, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Abstract_Definition, addTrait, arginfo_class_Componere_Abstract_Definition_addTrait, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Abstract_Definition, addInterface, arginfo_class_Componere_Abstract_Definition_addInterface, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Abstract_Definition, getReflector, arginfo_class_Componere_Abstract_Definition_getReflector, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_Componere_Definition_methods[] = {
	ZEND_ME(Componere_Definition, __construct, arginfo_class_Componere_Definition___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Definition, addProperty, arginfo_class_Componere_Definition_addProperty, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Definition, addConstant, arginfo_class_Componere_Definition_addConstant, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Definition, setConstant, arginfo_class_Componere_Definition_setConstant, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Definition, getClosure, arginfo_class_Componere_Definition_getClosure, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Definition, getClosures, arginfo_class_Componere_Definition_getClosures, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Definition, register, arginfo_class_Componere_Definition_register, ZEND_ACC_PUBLIC)
	ZEND_ME(Componere_Definition, isRegistered, arginfo_class_Componere_Definition_isRegistered, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
