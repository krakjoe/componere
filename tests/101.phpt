--TEST--
added properties must fit when replacing classes
--FILE--
<?php
use Componere\Definition;
use Componere\Value;

class A {}

$object = new A();

$def = new Definition(A::class);

try {
	$def->addProperty("property", new Value(true));
} catch (RuntimeException $ex) {
	echo "OK";
}
?>
--EXPECT--
OK
