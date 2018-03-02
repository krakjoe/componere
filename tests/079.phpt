--TEST--
Value flags
--FILE--
<?php
use Componere\Value;

$d = new Value();

$d->setPrivate();

var_dump($d->isPrivate());

$d = new Value();

$d->setProtected();
$d->setStatic();

var_dump($d->isProtected(), $d->isStatic());
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)




