--TEST--
Definition incorrect addMethod
--FILE--
<?php
use Componere\Definition;

$d = new Definition(E::class);
$d->addMethod("name", function(){});
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: name and method expected in %s:5
Stack trace:
#0 %s(5): Componere\Abstract\Definition->addMethod('name', Object(Closure))
#1 {main}
  thrown in %s on line 5


