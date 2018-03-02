--TEST--
Reflector singletons
--FILE--
<?php
use Componere\Definition;
use componere\Method;
$d = new Definition(A::class);

var_dump($d->getReflector(), $d->getReflector());

$m = new Method(function(){});

var_dump($m->getReflector(), $m->getReflector());
?>
--EXPECT--
object(ReflectionClass)#2 (1) {
  ["name"]=>
  string(1) "A"
}
object(ReflectionClass)#2 (1) {
  ["name"]=>
  string(1) "A"
}
object(ReflectionMethod)#4 (2) {
  ["name"]=>
  string(9) "{closure}"
  ["class"]=>
  string(0) ""
}
object(ReflectionMethod)#4 (2) {
  ["name"]=>
  string(9) "{closure}"
  ["class"]=>
  string(0) ""
}





