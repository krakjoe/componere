--TEST--
Method access (1)
--FILE--
<?php
use Componere\Method;

$m =  new Method(function(){});

$m->setPrivate();
$m->setProtected();
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: access level already set in %s:7
Stack trace:
#0 %s(7): Componere\Method->setProtected()
#1 {main}
  thrown in %s on line 7

