--TEST--
Method access (2)
--FILE--
<?php
use Componere\Method;

$m =  new Method(function(){});

$m->setProtected();
$m->setPrivate();
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: access level already set in %s:7
Stack trace:
#0 %s(7): Componere\Method->setPrivate()
#1 {main}
  thrown in %s on line 7

