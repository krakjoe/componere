--TEST--
Method::setPrivate
--FILE--
<?php
use Componere\Method;

$m = (new Method(function(){}))->setPrivate();

$reflector = $m->getReflector();

var_dump($reflector->isPrivate());
?>
--EXPECT--
bool(true)

