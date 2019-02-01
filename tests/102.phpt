--TEST--
Method::setFinal
--FILE--
<?php
use Componere\Method;

$method = new Method(function(){});
$method->setFinal();
$reflector = $method->getReflector();

if ($reflector->isFinal()){
	echo "OK1\n";
}

$method = new Method(function(){});
$reflector = $method->getReflector();

if (!$reflector->isFinal()){
	echo "OK2";
}

?>
--EXPECT--
OK1
OK2
