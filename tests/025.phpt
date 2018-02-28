--TEST--
Disallow addProperty after registration
--FILE--
<?php
use Componere\Definition;
use Componere\Property;
use Componere\Value;

$a = new Definition(A::class);
$a->register();
$a->addProperty("name", new Value(0));
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: A is already registered, cannot add property name in %s:8
Stack trace:
#0 %s(8): Componere\Abstract\Definition->addProperty('name', Object(Componere\Value))
#1 {main}
  thrown in %s on line 8
