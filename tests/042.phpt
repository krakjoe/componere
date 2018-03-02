--TEST--
Method::setStatic
--FILE--
<?php
use Componere\Method;

$m = (new Method(function(){}))->setStatic();

$reflector = $m->getReflector();

var_dump($reflector->isStatic());
?>
--EXPECT--
bool(true)

