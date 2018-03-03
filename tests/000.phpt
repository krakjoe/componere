--TEST--
Detect O+ Level
--SKIPIF--
<?php
if (!ini_get("opcache.enable_cli"))
	die("skip opcache.enable_cli is not set");
if (!ini_get("opcache.optimization_level"))
	die("skip opcache optimmization level not set");
--FILE--
OK
--EXPECT--
OK
