--TEST--
Method::setPrivate
--FILE--
<?php
use Componere\Definition;
use Componere\Method;
$a = new Definition(A::class);
$m = (new Method(function(){
	return true;
}))->setPrivate();
$a->addMethod("name", $m);
$a->register();

$reflector = new ReflectionClass(A::class);
$reflector = $reflector->getMethod("name");

var_dump($reflector->isPrivate());
?>
--EXPECT--
bool(true)

