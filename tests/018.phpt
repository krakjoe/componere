--TEST--
Definition non-existent interface
--FILE--
<?php
use Componere\Definition;

$definition = new Definition(A::class, [
	B::class
]);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: could not find interface B in %s:5
Stack trace:
#0 %s(5): Componere\Definition->__construct('A', Array)
#1 {main}
  thrown in %s on line 5
