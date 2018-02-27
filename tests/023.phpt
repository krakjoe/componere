--TEST--
Disallow addTrait after registration
--FILE--
<?php
use Componere\Definition;

trait T {}

$a = new Definition(A::class);
$a->register();
$a->addTrait(T::class);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: A is already registered, cannot add trait T in %s:8
Stack trace:
#0 %s(8): Componere\Definition->addTrait('T')
#1 {main}
  thrown in %s on line 8
