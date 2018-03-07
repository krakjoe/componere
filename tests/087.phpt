--TEST--
Componere\cast incorrect invocation (1)
--FILE--
<?php
use function Componere\cast;

class Child extends DateTime {}

cast(Child::class, new DateTime);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: cannot cast between internal types in %s:6
Stack trace:
#0 %s(6): Componere\cast('Child', Object(DateTime))
#1 {main}
  thrown in %s on line 6
