--TEST--
Definition::getClosure
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

class A {
	public function thing() {
		echo __METHOD__ . "\n";	
	}
}

$definition = new Definition(A::class);

$original = $definition->getClosure("thing");

$definition->addMethod("thing", new Method(function() use($original) {
	echo "before\n";
	$original();
	echo "after\n";
}));

$definition->register();

$a = new A();
$a->thing();
?>
--EXPECT--
before
A::thing
after

