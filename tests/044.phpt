--TEST--
Value::setProtected
--FILE--
<?php
use Componere\Definition;
use Componere\Value;

$a = new Definition(A::class);
$a->addProperty("prop", 
	(new Value(10))->setProtected());
$a->register();

$reflector = new ReflectionClass(A::class);
$reflector = $reflector->getProperty("prop");
var_dump($reflector->isProtected());
?>
--EXPECT--
bool(true)

