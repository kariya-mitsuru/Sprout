#!/usr/bin/perl
# =============================================================================
# Copyright (c) 2011-2013 Bolero MURAKAMI
# https://github.com/bolero-MURAKAMI/Sprout
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
# =============================================================================

use strict;
use warnings;

use Getopt::Long qw(:config posix_default no_ignore_case permute bundling);
use File::Basename;
use File::Path;


sub print_usage {
	print <<EOD;
help:

  -s, --source=<file>         Indicates the source file.
                              Default; '../../example/darkroom/two_spheres.hpp'

  -S, --stagedir=<directory>  Output files here.
                              Default; 'darkroom'

  -o, --output=<file>         Output file of the result.
                              Default; 'out.ppm'

  -C, --compiler=<command>    Compiler to use.
                              Default; 'g++'

  -w, --width=<value>         Output width of rendering.
                              Default; 16

  -h, --height=<value>        Output height of rendering.
                              Default; 16

  -W, --tile-width=<value>    Output width of divided rendering.
                              Default; 16

  -H, --tile-height=<value>   Output height of divided rendering.
                              Default; 16

  -D, --define=<identifier>   Define macro for preprocessor.

  -I, --include=<directory>   Add system include path.

  -P, --max-procs=<value>     The maximum number of process use.
                              If other than 1, processing in parallel mode.
                              If 0, using the number of CPUs in the system.
                              Default; 1

  -f, --force                 Allow overwrite of <stagedir>.

      --help                  This message.
EOD
	exit 0
}


sub get_options {
	my %options = (
		source => '../../example/darkroom/two_spheres.hpp',
		stagedir => 'darkroom',
		output => 'out.ppm',
		compiler => 'g++',
		width => 16,
		height => 16,
		'tile-width' => 16,
		'tile-height' => 16,
		define => [],
		include => [],
		max_procs => 1,
		force => 0,
		help => 0,
	);

	GetOptions(
		\%options,
		"source|s=s",
		"stagedir|S=s",
		"output|o=s",
		"compiler|C=s",
		"width|w=i",
		"height|h=i",
		"tile-width|W=i",
		"tile-height|H=i",
		"define|D=s",
		"include|I=s",
		"max_procs|P=i",
		"force|f",
		"help",
	) or die "error: options parse error. See 'darkcult.pl --help'\n";
	return \%options;
}


sub print_settings {
	my ($options) = @_;

	print <<EOD;
settings:
  source = '$options->{source}'
  stagedir = '$options->{stagedir}'
  output = '$options->{output}'
  compiler = '$options->{compiler}'
  width = $options->{width}
  height = $options->{height}
  tile-width = $options->{'tile-width'}
  tile-height = $options->{'tile-height'}
  user-macros = (@{$options->{define}})
  include-paths = (@{$options->{include}})
  max_procs = $options->{max_procs}
  force = $options->{force}
EOD
}


sub check_source {
	my ($source, $scriptdir) = @_;

	if (not -f "$scriptdir/$source") {
		die "error: source($source) not exists.\n";
	}
}


sub create_stagedir {
	my ($stagedir, $force) = @_;

	if (-d $stagedir) {
		if (not $force) {
			die "error: stagedir($stagedir) already exists.\n";
		} else {
			rmtree $_ for glob("$stagedir/*");
		}
	} else {
		mkpath $stagedir;
	}
}


{
	my ($max_procs, %pids);

	sub set_parallel {
		$max_procs = $_[0];
		if ($max_procs == 0) {
			if (open(my $nproc, '-|', 'nproc')) {
				$max_procs = <$nproc>;
				chomp $max_procs;
				close $nproc;
			} else {
				warn "cannot determine number of processors. assume 1.\n";
				$max_procs = 1;
			}
		}
		return $max_procs;
	}

	sub compile {
		my ($options, $binname, $x, $y, $scriptdir) = @_;

		while (keys %pids >= $max_procs) {
			my $pid = waitpid(-1, 0);
			delete $pids{$pid} if (exists $pids{$pid});
		}

		my $pid = fork();
		if ($pid < 0) {
			die "cannot create child process.($!)\n";
		} elsif ($pid == 0) {
			exec
				$options->{compiler}, '-o', $binname, '-std=c++11',
				map("-D$_", @{$options->{define}}),
				map("-I$_", @{$options->{include}}),
				"-DDARKROOM_SOURCE=\"$options->{source}\"",
				"-DDARKROOM_TOTAL_WIDTH=$options->{width}",
				"-DDARKROOM_TOTAL_HEIGHT=$options->{height}",
				"-DDARKROOM_TILE_WIDTH=$options->{'tile-width'}",
				"-DDARKROOM_TILE_HEIGHT=$options->{'tile-height'}",
				"-DDARKROOM_OFFSET_X=$x",
				"-DDARKROOM_OFFSET_Y=$y",
				"$scriptdir/darkcult.cpp" or die "Cannot exec compiler '$options->{compiler}'.($!)";
		}
		$pids{$pid} = 1;
	}

	sub wait_all {
		for my $pid (keys %pids) {
			while (waitpid($pid, 0) != $pid) {
			}
			$? == 0 or die "error: compile failed($!).\n";
		}
		%pids = ();
	}
}


sub readdata {
	my ($binname, $data, $tile_width, $tile_height) = @_;

	open(my $file, '-|', "'$binname'") or die "error: cannot execute '$binname'.($!)\n";
	my $format = <$file>;
	my $xy = <$file>;
	my ($x, $y) = split / /, $xy;
	if ($x != $tile_width or $y != $tile_height) {
		die "error: unexpected error.\n";
	}
	my $depth = <$file>;
	for (my $i = 0; my $line = <$file>; $i++) {
		$data->[$i / $x] .= pack("C3", split / /, $line);
	}
	close $file;
}


sub render {
	my ($options, $scriptdir) = @_;

	if ($options->{max_procs} != 1) {
		print "  processing in parallel mode.\n";
		print '  ';
	}

	for (my $y = 0; $y < $options->{height}; $y += $options->{'tile-height'}) {
		if ($options->{max_procs} == 1) {
			print "  y = ($y/$options->{height})...\n";
			print '    x = ';
		}

		my $y_start = time;
		mkpath "$options->{stagedir}/$y";
		for (my $x = 0; $x < $options->{width}; $x += $options->{'tile-width'}) {
			print "($x/$options->{height})." if $options->{max_procs} == 1;
			compile $options, "$options->{stagedir}/$y/$x.out", $x, $y, $scriptdir;
			if ($options->{max_procs} == 1) {
				wait_all;
			} else {
				print '.';
			}
		}

		if ($options->{max_procs} == 1) {
			print "\n";
			print '    elapsed = ', time - $y_start, "s\n"
		}
	}
	if ($options->{max_procs} != 1) {
		wait_all;
		print "\n";
	}
}


sub output {
	my ($options) = @_;

	open(my $file, '>', $options->{output}) or die "error: cannot open $options->{output}.($!)\n";

	print $file "P6\n";
	print $file "$options->{width} $options->{height}\n";
	print $file "255\n";

	for (my $y = 0; $y < $options->{height}; $y += $options->{'tile-height'}) {
		my @data;
		for (my $x = 0; $x < $options->{width}; $x += $options->{'tile-width'}) {
			my $binname = "$options->{stagedir}/$y/$x.out";
			readdata $binname, \@data, $options->{'tile-width'}, $options->{'tile-height'};
		}
		print $file @data;
	}

	close $file;
}


local $| = 1;
my ($scriptname, $scriptdir) = fileparse($0);
my $options = get_options;
print_usage if $options->{help};
print_settings $options;
check_source $options->{source}, $scriptdir;
create_stagedir $options->{stagedir}, $options->{force};
$options->{max_procs} = set_parallel($options->{max_procs});

print "rendering:\n";
my $start = time;

render $options, $scriptdir;
output $options;

print '  elapsed(total) = ', time - $start, "s\n";
print "finished.\n";
