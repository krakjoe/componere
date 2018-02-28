--TEST--
Method::setProtected
--FILE--
<?php
use Componere\Definition;
use Componere\Method;
$a = new Definition(A::class);
$m = (new Method(function(){
	return true;
}))->setProtected();
$a->addMethod("name", $m);
$a->register();

$reflector = new ReflectionClass(A::class);
$reflector = $reflector->getMethod("name");

var_dump($reflector->isProtected());
?>
--EXPECT--
bool(true)

