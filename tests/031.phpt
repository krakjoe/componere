--TEST--
Definition::patch disallow internal class patch
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

$instance = new DateTime();

$a = new Definition(A::class, DateTime::class);

$a->patch($instance);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: cannot patch internal objects in %s:9
Stack trace:
#0 %s(9): Componere\Definition->patch(Object(DateTime))
#1 {main}
  thrown in %s on line 9
