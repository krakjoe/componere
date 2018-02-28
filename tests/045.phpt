--TEST--
Value::setPrivate
--FILE--
<?php
use Componere\Definition;
use Componere\Value;

$a = new Definition(A::class);
$a->addProperty("prop", 
	(new Value(10))->setPrivate());
$a->register();

$reflector = new ReflectionClass(A::class);
$reflector = $reflector->getProperty("prop");
var_dump($reflector->isPrivate());
?>
--EXPECT--
bool(true)

