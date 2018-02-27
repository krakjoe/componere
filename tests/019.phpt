--TEST--
Definition implements non-interface
--FILE--
<?php
use Componere\Definition;

trait B {}

$definition = new Definition(A::class, [
	B::class
]);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: B is not an interface in %s:7
Stack trace:
#0 %s(7): Componere\Definition->__construct('A', Array)
#1 {main}
  thrown in %s on line 7
