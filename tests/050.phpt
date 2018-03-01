--TEST--
Value final access (2)
--FILE--
<?php
use Componere\Value;

$v =  new Value(0);

$v->setProtected();
$v->setPrivate();
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: access level already set in %s:7
Stack trace:
#0 %s(7): Componere\Value->setPrivate()
#1 {main}
  thrown in %s on line 7

