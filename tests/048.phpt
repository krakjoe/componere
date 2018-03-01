--TEST--
Value final access
--FILE--
<?php
use Componere\Value;

$v =  new Value(0);

$v->setPrivate();
$v->setProtected();
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: access level already set in %s:7
Stack trace:
#0 %s(7): Componere\Value->setProtected()
#1 {main}
  thrown in %s on line 7

