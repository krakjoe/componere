--TEST--
Disallow addProperty duplicates
--FILE--
<?php
use Componere\Definition;
use Componere\Property;
use Componere\Value;

$a = new Definition(A::class);
$a->addProperty("name", new Value());
$a->addProperty("name", new Value());
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: cannot redeclare A::$name in %s:8
Stack trace:
#0 %s(8): Componere\Definition->addProperty('name', Object(Componere\Value))
#1 {main}
  thrown in %s on line 8
