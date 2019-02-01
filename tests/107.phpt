--TEST--
setConstant API
--FILE--
<?php
class Foo {
	const BAR = 10;
}

$def = new \Componere\Definition(Foo::class);
try {
	$def->setConstant("QUX", new \Componere\Value(42)); /* set non existent */
} catch (Exception $ex) {
	echo "OK\n";
}

try {
	$def->setConstant("BAR", new \Componere\Value()); /* set undefined */
} catch (Exception $ex) {
	echo "OK\n";
}

try {
	$value = new \Componere\Value();
	$value->setStatic();
	$def->setConstant("BAR", $value); /* set static */
} catch (Exception $ex) {
	echo "OK\n";
}

$def->setConstant("BAR", new \Componere\Value(42));
$def->register();

var_dump(FOO::BAR);

try {
	$def->setConstant("BAR", new \Componere\Value(42)); /* set after register */
} catch (Exception $ex) {
	echo "OK\n";
}
?>
--EXPECT--
OK
OK
OK
int(42)
OK


