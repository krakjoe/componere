--TEST--
Definition::patch disallow internal class patch
--FILE--
<?php
use Componere\Patch;
use Componere\Method;

$instance = new DateTime();

$a = new Patch($instance);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: cannot patch internal objects in %s:7
Stack trace:
#0 %s(7): Componere\Patch->__construct(Object(DateTime))
#1 {main}
  thrown in %s on line 7

