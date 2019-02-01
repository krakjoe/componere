--TEST--
relink classes and methods and objects
--FILE--
<?php
class Foo {
	public function method() : Foo {
		echo __METHOD__ . "\n";
		return $this;
	}
}

$foo = new Foo();

var_dump($foo instanceof Foo, $foo->method());

$definition = new \Componere\Definition(Foo::class);
$definition->addMethod("method", new \Componere\Method(function() : Foo {
	echo "modified\n";
	return $this;
}));
$definition->register();

var_dump($foo instanceof Foo, $foo->method());
?>
--EXPECT--
Foo::method
bool(true)
object(Foo)#1 (0) {
}
modified
bool(true)
object(Foo)#1 (0) {
}

