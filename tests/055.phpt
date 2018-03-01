--TEST--
Definition incorrect construction (1)
--FILE--
<?php
use Componere\Definition;

new Definition(new stdClass);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: name expected as single argument in %s:4
Stack trace:
#0 %s(4): Componere\Definition->__construct(Object(stdClass))
#1 {main}
  thrown in %s on line 4


