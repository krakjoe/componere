--TEST--
Value incorrect construction (2)
--FILE--
<?php
use Componere\Value;

new Value(new stdClass);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: values of type object cannot be declared with default values in %s:4
Stack trace:
#0 %s(4): Componere\Value->__construct(Object(stdClass))
#1 {main}
  thrown in %s on line 4


