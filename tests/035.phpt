--TEST--
Patch with interfaces non-interface
--FILE--
<?php
use Componere\Patch;
use Componere\Method;

class A {}

trait ITrait {}

$instance = new A();

$a = new Patch($instance, [ITrait::class]);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: ITrait is not an interface in %s:11
Stack trace:
#0 %s(11): Componere\Patch->__construct(Object(A), Array)
#1 {main}
  thrown in %s on line 11
