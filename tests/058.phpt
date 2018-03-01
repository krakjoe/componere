--TEST--
Definition incorrect construction (4)
--FILE--
<?php
use Componere\Definition;

new Definition(new stdClass, new stdClass, new stdClass, new stdClass);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: name, parent, and interfaces expected in %s:4
Stack trace:
#0 %s(4): Componere\Definition->__construct(Object(stdClass), Object(stdClass), Object(stdClass), Object(stdClass))
#1 {main}
  thrown in %s on line 4


