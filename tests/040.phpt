--TEST--
Method::setProtected
--FILE--
<?php
use Componere\Method;

$m = (new Method(function(){}))->setProtected();

$reflector = $m->getReflector();

var_dump($reflector->isProtected());
?>
--EXPECT--
bool(true)

