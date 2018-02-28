--TEST--
Method::setStatic
--FILE--
<?php
use Componere\Definition;
use Componere\Method;
$a = new Definition(A::class);
$m = (new Method(function(){
	return true;
}))->setStatic();
$a->addMethod("name", $m);
$a->register();

$reflector = new ReflectionClass(A::class);
$reflector = $reflector->getMethod("name");

var_dump($reflector->isStatic());
?>
--EXPECT--
bool(true)

