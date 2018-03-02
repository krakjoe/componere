--TEST--
Value::setPrivate
--FILE--
<?php
use Componere\Definition;
use Componere\Value;

$a = new Definition(A::class);
$a->addProperty("prop", (new Value())->setPrivate());

$reflector = $a->getReflector();
$reflector = $reflector->getProperty("prop");
var_dump($reflector->isPrivate());
?>
--EXPECT--
bool(true)

