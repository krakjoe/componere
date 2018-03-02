--TEST--
Value incorrect construction (1)
--FILE--
<?php
use Componere\Value;

new Value(new stdClass, new stdClass);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: value expected in %s:4
Stack trace:
#0 %s(4): Componere\Value->__construct(Object(stdClass), Object(stdClass))
#1 {main}
  thrown in %s on line 4


