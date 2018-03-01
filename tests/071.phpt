--TEST--
Patch get non-existent closure
--FILE--
<?php
use Componere\Patch;

class A {}

$a = new A();

$p = new Patch($a);
$p->getClosure("none");
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: could not find A::none in %s:9
Stack trace:
#0 %s(9): Componere\Patch->getClosure('none')
#1 {main}
  thrown in %s on line 9


