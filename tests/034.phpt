--TEST--
Patch with interfaces non-existent
--FILE--
<?php
use Componere\Patch;
use Componere\Method;

class A {}

$instance = new A();

$a = new Patch($instance, [IFace::class]);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: could not find interface IFace in %s:9
Stack trace:
#0 %s(9): Componere\Patch->__construct(Object(A), Array)
#1 {main}
  thrown in %s on line 9
