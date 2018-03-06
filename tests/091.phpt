--TEST--
Componere\cast incorrect invocation (5)
--FILE--
<?php
use function Componere\cast;

class A {}
class B {}

$instance = new A;

cast(B::class, $instance);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: B is not compatible with A in %s:9
Stack trace:
#0 %s(9): Componere\cast('B', Object(A))
#1 {main}
  thrown in %s on line 9

