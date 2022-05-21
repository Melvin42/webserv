#!/usr/bin/perl

$dir = $ARGV[0];

opendir(my $dh, $dir) || die "Error in open dir $dir\n";

print " 200 OK\r
<!DOCTYPE html>
<html>
\t<head>
\t\t<meta charset=\"utf-8\" />
\t\t<link rel=\"stylesheet\" href=\"style.css\" />
\t\t<title>Melval Kingdom</title>
\t</head>
<body>

<h1>Welcome to the melval</h1>";
while ($filename = readdir($dh)) {
	print "<p><a href=\"$dir/$filename\">$filename</a></p>\n";
}
closedir $dh;
print "</body>
</html>";

1;