--TEST--
Recomposition calling non-public method
--FILE--
<?php 
use Componere\Definition;
use Componere\Method;

class A {
	private function thing() {
		return true;
	}
}

$definition = new Definition(A::class);
$definition->addMethod("member", new Method(function(){
	return self::thing();
}));
$definition->register();

$a = new A();

var_dump($a->member());

$reflector = $definition->getReflector();
$reflector = $reflector->getMethod("thing");

var_dump($reflector->isPrivate());
?>
--EXPECT--
bool(true)
bool(true)
