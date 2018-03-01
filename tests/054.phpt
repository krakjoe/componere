--TEST--
Method incorrect construction
--FILE--
<?php
use Componere\Method;

new Method(0);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: closure expected in %s:4
Stack trace:
#0 %s(4): Componere\Method->__construct(0)
#1 {main}
  thrown in %s on line 4


