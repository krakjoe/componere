--TEST--
Value::setStatic
--FILE--
<?php
use Componere\Definition;
use Componere\Value;

$a = new Definition(A::class);
$a->addProperty("prop", 
	(new Value(10))->setStatic());
$a->register();

var_dump(A::$prop);

?>
--EXPECT--
int(10)

