--TEST--
Composition with traits
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

trait T {
	public function test() {
		return true;
	}
}

$definition = new Definition(A::class);

$definition->addMethod("method", new Method(function() : bool {
	return self::test();
}));

$definition->addTrait(T::class);

$definition->register();

$a = new A();

var_dump($a->method());
?>
--EXPECT--
bool(true)
