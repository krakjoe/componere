/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 6612c7a30bcfe6b00fe0bc93c1313778a916e9f4 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Value___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, value, IS_MIXED, 0, "NULL")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Value_setProtected, 0, 0, Componere\\Value, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Value_setPrivate arginfo_class_Value_setProtected

#define arginfo_class_Value_setStatic arginfo_class_Value_setProtected

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Value_isPrivate, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Value_isProtected arginfo_class_Value_isPrivate

#define arginfo_class_Value_isStatic arginfo_class_Value_isPrivate

#define arginfo_class_Value_hasDefault arginfo_class_Value_isPrivate


ZEND_METHOD(Value, __construct);
ZEND_METHOD(Value, setProtected);
ZEND_METHOD(Value, setPrivate);
ZEND_METHOD(Value, setStatic);
ZEND_METHOD(Value, isPrivate);
ZEND_METHOD(Value, isProtected);
ZEND_METHOD(Value, isStatic);
ZEND_METHOD(Value, hasDefault);


static const zend_function_entry class_Value_methods[] = {
	ZEND_ME(Value, __construct, arginfo_class_Value___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Value, setProtected, arginfo_class_Value_setProtected, ZEND_ACC_PUBLIC)
	ZEND_ME(Value, setPrivate, arginfo_class_Value_setPrivate, ZEND_ACC_PUBLIC)
	ZEND_ME(Value, setStatic, arginfo_class_Value_setStatic, ZEND_ACC_PUBLIC)
	ZEND_ME(Value, isPrivate, arginfo_class_Value_isPrivate, ZEND_ACC_PUBLIC)
	ZEND_ME(Value, isProtected, arginfo_class_Value_isProtected, ZEND_ACC_PUBLIC)
	ZEND_ME(Value, isStatic, arginfo_class_Value_isStatic, ZEND_ACC_PUBLIC)
	ZEND_ME(Value, hasDefault, arginfo_class_Value_hasDefault, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
