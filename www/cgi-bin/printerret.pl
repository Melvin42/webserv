#!/usr/bin/perl

use CGI qw(:standardi);

$q = new CGI;

$num = $q->param('name');
$msg = $q->param('email');

print "<!DOCTYPE html>
<html>
<body>

<h1> ERROR $num</h1>

<p>$msg</p>
</body>
</html>";

1;
