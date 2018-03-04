--TEST--
Patch::derive
--FILE--
<?php
use Componere\Patch;
use Componere\Method;

$instance = new DateTime();

$first = new Patch($instance);
$first->addMethod("__toString", new Method(function(){
	return "ok";
}));
$first->apply();

$second = $first->derive($instance);
$second->addMethod("__toString", new Method(function(){
	return "ko";
}));

printf("first=%d second=%d ", $first->isApplied(), $second->isApplied());
echo (string) $instance;
echo "\n";

$second->apply();

printf("first=%d second=%d ", $first->isApplied(), $second->isApplied());
echo (string) $instance;
echo "\n";

$second->revert();

printf("first=%d second=%d ", $first->isApplied(), $second->isApplied());
echo (string) $instance;
echo "\n";

$first->revert();

printf("first=%d second=%d ", $first->isApplied(), $second->isApplied());
echo (string) $instance;
echo "\n";
?>
--EXPECTF--
first=1 second=0 ok
first=0 second=1 ko
first=1 second=0 ok
first=0 second=0 
%s fatal error: Object of class DateTime could not be converted to string in %s on line 37
