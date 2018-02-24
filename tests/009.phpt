--TEST--
Composition with interfaces
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

interface IFace {
	public function method() : bool;
}

$definition = new Definition(A::class, [
	IFace::class
]);

$definition->addMethod("method", new Method(function() : bool {
	return true;
}));

$definition->register();

$a = new A();

var_dump($a instanceof IFace);
?>
--EXPECT--
bool(true)
